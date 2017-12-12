#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <memory.h>
#include <errno.h>
#include<stdlib.h>

#include "ion/ion.h"
#include "rk_fb.h"

struct fb *g_fb;

static int need_flip = 0;
static int disp_w = 0;
static int disp_h = 0;
static int disp_w_max = 0;
static int disp_h_max = 0;

static pthread_mutex_t rkfb_mutex;

#if 0
/* hdmi max support 1920x1080 */
#define HDMI_OUT_W_MAX  1920
#define HDMI_OUT_H_MAX  1080
#else
/* hdmi max support 1280x720 */
#define HDMI_OUT_W_MAX  1280
#define HDMI_OUT_H_MAX  720
#endif

static int yuv_range = CSC_BT601;

void rk_fb_set_yuv_range(int range)
{
	yuv_range = range;
}

static void rk_fb_set_yuv_range_to_format(unsigned char *format)
{
	*format &= ~0xC0;
	*format |= (yuv_range << 6);
}

static void rkfb_init_lock(void)
{
    pthread_mutex_init(&rkfb_mutex, NULL);
}

static void rkfb_destroy_lock(void)
{
    pthread_mutex_destroy(&rkfb_mutex);
}

static void rkfb_lock(void)
{
    pthread_mutex_lock(&rkfb_mutex);
}

static void rkfb_unlock(void)
{
    pthread_mutex_unlock(&rkfb_mutex);
}

int rk_fb_open(struct fb *fb)
{
    printf("%s\n", __func__);
    if (fb == 0)
        return -1;

    fb->fd = open(FB0, O_RDWR);
    if (fb->fd < 0) {
        printf("open %s failed.\n", FB0);
        return -1;
    }
    return 0;
}

void rk_fb_close(struct fb *fb)
{
    printf("%s\n", __func__);
    if (fb == 0)
        return;

    if (fb->fd >= 0)
        close(fb->fd);
    fb->fd = 0;
}

int rk_fb_config_deinit(struct fb *fb)
{
    int j;
    struct win *video_win;

    if (fb == 0)
        return -1;

    video_win = fb->video_win_disp;
    fb->win_cfg.wait_fs = 0;
    fb->win_cfg.win_par[1].win_id = 0;
    fb->win_cfg.win_par[1].g_alpha_val = 0;
    fb->win_cfg.win_par[1].z_order = 0;
    fb->win_cfg.win_par[1].area_par[0].acq_fence_fd = -1;
    fb->win_cfg.win_par[1].area_par[0].data_format = fb->win0.format;
    fb->win_cfg.win_par[1].area_par[0].ion_fd = fb->win0.video_ion.fd;
    fb->win_cfg.win_par[1].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].xpos = 0;
    fb->win_cfg.win_par[1].area_par[0].ypos = 0;
    fb->win_cfg.win_par[1].area_par[0].xsize = fb->win0.width;
    fb->win_cfg.win_par[1].area_par[0].ysize = fb->win0.height;
    fb->win_cfg.win_par[1].area_par[0].xact = fb->win0.width;
    fb->win_cfg.win_par[1].area_par[0].yact = fb->win0.height;
    fb->win_cfg.win_par[1].area_par[0].xvir = fb->win0.width;
    fb->win_cfg.win_par[1].area_par[0].yvir = fb->win0.height;

    if (ioctl(fb->fd, RK_FBIOSET_CONFIG_DONE,
              (unsigned long)(&fb->win_cfg)) < 0) {
        printf("FBIOSET_CONFIG failed(%s)\n", strerror(errno));
        return -1;
    }

    for (j = 0; j < RK_MAX_BUF_NUM; j++)
        if (fb->win_cfg.rel_fence_fd[j] > 0)
            close(fb->win_cfg.rel_fence_fd[j]);

    if (fb->win_cfg.ret_fence_fd > 0)
        close(fb->win_cfg.ret_fence_fd);

    return 0;
}

int rk_fb_config_init(struct fb *fb)
{
    int j;
    struct win *video_win;

    if (fb == 0)
        return -1;

    video_win = fb->video_win_disp;
    fb->win_cfg.wait_fs = 0;
    fb->win_cfg.win_par[0].win_id = 1;
    fb->win_cfg.win_par[0].g_alpha_val = 0;
    fb->win_cfg.win_par[0].z_order = fb->win0.z_order;
    fb->win_cfg.win_par[0].area_par[0].acq_fence_fd = -1;
    fb->win_cfg.win_par[0].area_par[0].data_format = fb->win0.format;
    fb->win_cfg.win_par[0].area_par[0].ion_fd = fb->win0.video_ion.fd;
    fb->win_cfg.win_par[0].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[0].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[0].area_par[0].xpos = 0;
    fb->win_cfg.win_par[0].area_par[0].ypos = 0;
    fb->win_cfg.win_par[0].area_par[0].xsize = fb->win0.width;
    fb->win_cfg.win_par[0].area_par[0].ysize = fb->win0.height;
    fb->win_cfg.win_par[0].area_par[0].xact = fb->win0.width;
    fb->win_cfg.win_par[0].area_par[0].yact = fb->win0.height;
    fb->win_cfg.win_par[0].area_par[0].xvir = fb->win0.width;
    fb->win_cfg.win_par[0].area_par[0].yvir = fb->win0.height;

    fb->win_cfg.win_par[1].win_id = 0;
    fb->win_cfg.win_par[1].g_alpha_val = 0;
    fb->win_cfg.win_par[1].z_order = video_win->z_order;
    fb->win_cfg.win_par[1].area_par[0].acq_fence_fd = -1;
    rk_fb_set_yuv_range_to_format(&video_win->format);
    fb->win_cfg.win_par[1].area_par[0].data_format = video_win->format;
    fb->win_cfg.win_par[1].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].xpos = 0;
    fb->win_cfg.win_par[1].area_par[0].ypos = 0;
    fb->win_cfg.win_par[1].area_par[0].xsize = fb->win0.width;
    fb->win_cfg.win_par[1].area_par[0].ysize = fb->win0.height;

    if (video_win->video_ion.buffer) {
        fb->win_cfg.win_par[1].area_par[0].ion_fd = video_win->video_ion.fd;
        fb->win_cfg.win_par[1].area_par[0].xact = fb->win0.width;
        fb->win_cfg.win_par[1].area_par[0].yact = fb->win0.height;
        fb->win_cfg.win_par[1].area_par[0].xvir = fb->win0.width;
        fb->win_cfg.win_par[1].area_par[0].yvir = fb->win0.height;
    } else {
        fb->win_cfg.win_par[1].area_par[0].ion_fd = video_win->buf_ion_fd;
        fb->win_cfg.win_par[1].area_par[0].xact = video_win->buf_width;
        fb->win_cfg.win_par[1].area_par[0].yact = video_win->buf_height;
        fb->win_cfg.win_par[1].area_par[0].xvir = (video_win->buf_width + 15) & (~15);
        fb->win_cfg.win_par[1].area_par[0].yvir = (video_win->buf_height + 15) & (~15);
    }

    if (ioctl(fb->fd, RK_FBIOSET_CONFIG_DONE,
              (unsigned long)(&fb->win_cfg)) < 0) {
        printf("FBIOSET_CONFIG failed(%s)\n", strerror(errno));
        return -1;
    }

    for (j = 0; j < RK_MAX_BUF_NUM; j++)
        if (fb->win_cfg.rel_fence_fd[j] > 0)
            close(fb->win_cfg.rel_fence_fd[j]);

    if (fb->win_cfg.ret_fence_fd > 0)
        close(fb->win_cfg.ret_fence_fd);

    return 0;
}

int rk_fb_ui_config(struct fb *fb)
{
    int j;
    struct win *video_win;
    int w, h;
    if (fb == 0)
        return -1;

    if (rk_fb_get_screen_state() == 0)
        return 0;

    w = disp_w;
    h = disp_h;

    rkfb_lock();

    video_win = fb->video_win_disp;

    fb->win_cfg.wait_fs = 0;
    fb->win_cfg.win_par[0].win_id = 1;
    fb->win_cfg.win_par[0].g_alpha_val = 0;
    fb->win_cfg.win_par[0].z_order = fb->win0.z_order;
    fb->win_cfg.win_par[0].area_par[0].acq_fence_fd = -1;
    fb->win_cfg.win_par[0].area_par[0].data_format = fb->win0.format;
    fb->win_cfg.win_par[0].area_par[0].ion_fd = fb->win0.video_ion.fd;
    fb->win_cfg.win_par[0].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[0].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[0].area_par[0].xpos = 0;
    fb->win_cfg.win_par[0].area_par[0].ypos = 0;
    fb->win_cfg.win_par[0].area_par[0].xsize = w;
    fb->win_cfg.win_par[0].area_par[0].ysize = h;
    fb->win_cfg.win_par[0].area_par[0].xact = w;
    fb->win_cfg.win_par[0].area_par[0].yact = h;
    fb->win_cfg.win_par[0].area_par[0].xvir = w;
    fb->win_cfg.win_par[0].area_par[0].yvir = h;

    if (ioctl(fb->fd, RK_FBIOSET_CONFIG_DONE,
              (unsigned long)(&fb->win_cfg)) < 0) {
        rkfb_unlock();
        printf("FBIOSET_CONFIG failed(%s)\n", strerror(errno));
        return -1;
    }

    for (j = 0; j < RK_MAX_BUF_NUM; j++)
        if (fb->win_cfg.rel_fence_fd[j] > 0)
            close(fb->win_cfg.rel_fence_fd[j]);

    if (fb->win_cfg.ret_fence_fd > 0)
        close(fb->win_cfg.ret_fence_fd);

    rkfb_unlock();

    return 0;
}

int rk_fb_video_config(struct fb *fb, struct win *video_win)
{
    int j;
    int xsize, ysize;

    if (fb == 0 || video_win == 0)
        return -1;

    if (rk_fb_get_screen_state() == 0)
        return 0;

    xsize = disp_w;
    ysize = disp_h;

    rkfb_lock();

    fb->win_cfg.wait_fs = 1;
    fb->win_cfg.win_par[1].win_id = 0;
    fb->win_cfg.win_par[1].g_alpha_val = 0;
    fb->win_cfg.win_par[1].z_order = video_win->z_order;
    fb->win_cfg.win_par[1].area_par[0].acq_fence_fd = -1;
    rk_fb_set_yuv_range_to_format(&video_win->format);
    fb->win_cfg.win_par[1].area_par[0].data_format = video_win->format;
    fb->win_cfg.win_par[1].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].xpos = 0;
    fb->win_cfg.win_par[1].area_par[0].ypos = 0;
    fb->win_cfg.win_par[1].area_par[0].xsize = xsize;
    fb->win_cfg.win_par[1].area_par[0].ysize = ysize;

    if (video_win->video_ion.buffer) {
        fb->win_cfg.win_par[1].area_par[0].ion_fd = video_win->video_ion.fd;
        fb->win_cfg.win_par[1].area_par[0].xact = xsize;
        fb->win_cfg.win_par[1].area_par[0].yact = ysize;
        fb->win_cfg.win_par[1].area_par[0].xvir = xsize;
        fb->win_cfg.win_par[1].area_par[0].yvir = ysize;
    } else {
        fb->win_cfg.win_par[1].area_par[0].ion_fd = video_win->buf_ion_fd;
        fb->win_cfg.win_par[1].area_par[0].xact = video_win->buf_width;
        fb->win_cfg.win_par[1].area_par[0].yact = video_win->buf_height;
        fb->win_cfg.win_par[1].area_par[0].xvir = (video_win->buf_width + 15) & (~15);
        fb->win_cfg.win_par[1].area_par[0].yvir = (video_win->buf_height + 15) & (~15);
    }

    if (ioctl(fb->fd, RK_FBIOSET_CONFIG_DONE,
              (unsigned long)(&fb->win_cfg)) < 0) {
        rkfb_unlock();
        printf("FBIOSET_CONFIG failed(%s)\n", strerror(errno));
        return -1;
    }

    for (j = 0; j < RK_MAX_BUF_NUM; j++)
        if (fb->win_cfg.rel_fence_fd[j] > 0)
            close(fb->win_cfg.rel_fence_fd[j]);

    if (fb->win_cfg.ret_fence_fd > 0)
        close(fb->win_cfg.ret_fence_fd);

    rkfb_unlock();

    return 0;
}

int rk_fb_video_config_ex(struct fb *fb, struct win *video_win, int w, int h)
{
    int j;
    int xsize, ysize;

    if (fb == 0 || video_win == 0)
        return -1;

    xsize = disp_w;
    ysize = disp_h;

    fb->win_cfg.wait_fs = 0;
    fb->win_cfg.win_par[1].win_id = 0;
    fb->win_cfg.win_par[1].g_alpha_val = 0;
    fb->win_cfg.win_par[1].z_order = video_win->z_order;
    fb->win_cfg.win_par[1].area_par[0].acq_fence_fd = -1;
    rk_fb_set_yuv_range_to_format(&video_win->format);
    fb->win_cfg.win_par[1].area_par[0].data_format = video_win->format;
    fb->win_cfg.win_par[1].area_par[0].x_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].y_offset = 0;
    fb->win_cfg.win_par[1].area_par[0].xpos = 0;
    fb->win_cfg.win_par[1].area_par[0].ypos = 0;
    fb->win_cfg.win_par[1].area_par[0].xsize = xsize;
    fb->win_cfg.win_par[1].area_par[0].ysize = ysize;

    fb->win_cfg.win_par[1].area_par[0].ion_fd = video_win->video_ion.fd;

    fb->win_cfg.win_par[1].area_par[0].xact = w;
    fb->win_cfg.win_par[1].area_par[0].yact = h;
    fb->win_cfg.win_par[1].area_par[0].xvir = (w + 15) & (~15);
    fb->win_cfg.win_par[1].area_par[0].yvir = (h + 15) & (~15);

    if (ioctl(fb->fd, RK_FBIOSET_CONFIG_DONE,
              (unsigned long)(&fb->win_cfg)) < 0) {
        printf("FBIOSET_CONFIG failed(%s)\n", strerror(errno));
        return -1;
    }

    for (j = 0; j < RK_MAX_BUF_NUM; j++)
        if (fb->win_cfg.rel_fence_fd[j] > 0)
            close(fb->win_cfg.rel_fence_fd[j]);

    if (fb->win_cfg.ret_fence_fd > 0)
        close(fb->win_cfg.ret_fence_fd);

    return 0;
}

int rk_fb_alloc_video_win_buff(struct fb *fb, struct win *win)
{
    int ret = 0;
    int w = disp_w_max;
    int h = disp_h_max;
    if (fb == 0 || win == 0)
        return -1;

    win->video_ion.client = ion_open();
    if (win->video_ion.client <= 0) {
        printf("%s:open /dev/ion failed!\n", __func__);
        return -1;
    }

    win->video_ion.size = w * h * 3 / 2;

    ret = ion_alloc(win->video_ion.client, win->video_ion.size, 0,
                    ION_HEAP_TYPE_DMA_MASK, 0, &win->video_ion.handle);
    if (ret) {
        printf("ion_alloc failed!\n");
        return -1;
    }
    ret = ion_share(win->video_ion.client, win->video_ion.handle, &win->video_ion.fd);
    if (ret) {
        printf("ion_share failed!\n");
        return -1;
    }
    win->buffer = mmap(NULL, win->video_ion.size, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_LOCKED, win->video_ion.fd, 0);
    if (!win->buffer) {
        printf("%s mmap failed!\n", __func__);
        return -1;
    }

    win->video_ion.buffer = win->buffer;
    win->video_ion.width = w;
    win->video_ion.height = h;

    return 0;
}

int rk_fb_ui_dispblack(struct fb * fb)
{
    if (fb == 0)
        return -1;
//在video层初始化的时候,不要让其将UI层的内容刷新掉.
#if 0
    memset(fb->win0.buffer,
           0, fb->win0.width * fb->win0.height * 4);
#endif

    return 0;
}

int rk_fb_video_dispblack(struct fb * fb)
{
    int w, h;
    if (fb == 0)
        return -1;
    rk_fb_get_out_device(&w, &h);
    // YUV BLACK : 16 128 128
    memset(fb->video_win0.buffer,
           16, w * h);
    memset(fb->video_win0.buffer + w * h,
           128, w * h / 2);

    memset(fb->video_win1.buffer,
           16, w * h);
    memset(fb->video_win1.buffer + w * h,
           128, w * h / 2);

    return 0;
}

int rk_fb_ui_init(struct fb * fb, int video_width, int video_height, int pixel_format)
{
    int share_fd = -1;

    if (fb == 0)
        return -1;

    if (ioctl(fb->fd, /*FBIOGET_DMABUF*/0x5003, &share_fd) != 0) {
        printf("FBIOGET_DMABUF fail!\n");
        return -1;
    }

    switch (pixel_format) {
    case FB_FORMAT_RGB_565:
        fb->win0.pixel = 2;
        fb->win0.format = HAL_PIXEL_FORMAT_RGB_565;
        break;
    case FB_FORMAT_BGRA_8888:
    default:
        fb->win0.pixel = 4;
        fb->win0.format = HAL_PIXEL_FORMAT_BGRA_8888;
        break;
    }

    fb->win0.width = video_width;
    fb->win0.height = video_height;
    fb->win0.z_order = 1;

    fb->win0.video_ion.fd = share_fd;
    fb->win0.buffer = mmap(0, fb->win0.width * fb->win0.height * 4,
                           PROT_READ | PROT_WRITE, MAP_SHARED, share_fd, 0);
    if (!fb->win0.buffer) {
        printf("%s mmap fail!\n", __func__);
        return -1;
    }


    rk_fb_ui_dispblack(fb);

    return 0;
}

int rk_fb_ui_deinit(struct fb * fb)
{
    if (fb == 0)
        return -1;

    munmap(fb->win0.buffer, fb->win0.width * fb->win0.height * fb->win0.pixel);

    return 0;
}

int rk_fb_video_init(struct fb * fb, int video_width, int video_height)
{
    if (fb == 0)
        return -1;
    fb->video_win0.width = video_width;
    fb->video_win0.height = video_height;
    fb->video_win0.pixel = 2;
    fb->video_win0.format = HAL_PIXEL_FORMAT_YCrCb_NV12;
    fb->video_win0.z_order = 0;

    fb->video_win1.width = video_width;
    fb->video_win1.height = video_height;
    fb->video_win1.pixel = 2;
    fb->video_win1.format = HAL_PIXEL_FORMAT_YCrCb_NV12;
    fb->video_win1.z_order = 0;

    fb->video_win_disp = &fb->video_win0;
    fb->video_win_write = &fb->video_win1;

    if (rk_fb_alloc_video_win_buff(fb, &fb->video_win0) < 0) {
        printf("Error:alloc win1 failed.\n");
        return -1;
    }

    if (rk_fb_alloc_video_win_buff(fb, &fb->video_win1) < 0) {
        printf("Error:alloc win1 failed.\n");
        return -1;
    }

    // YUV BLACK : 16 128 128
    rk_fb_video_dispblack(fb);

    return 0;
}

int rk_fb_video_deinit(struct fb * fb)
{
    int ret = 0;

    if (fb == 0)
        return -1;
    if (fb->video_win0.buffer) {
        munmap(fb->video_win0.buffer, fb->video_win0.video_ion.size);
        fb->video_win0.buffer = NULL;
        fb->video_win0.video_ion.buffer = NULL;
    }

    if (fb->video_win0.video_ion.fd) {
        close(fb->video_win0.video_ion.fd);
        fb->video_win0.video_ion.fd = 0;
    }

    if (fb->video_win0.video_ion.client) {
        if (fb->video_win0.video_ion.handle) {
            ret = ion_free(fb->video_win0.video_ion.client,
                           fb->video_win0.video_ion.handle);
            if (ret)
                printf("ion_free failed!\n");
            fb->video_win0.video_ion.handle = 0;
        }

        ion_close(fb->video_win0.video_ion.client);
        fb->video_win0.video_ion.client = 0;
    }

    /******************************************************************************************/

    if (fb->video_win1.buffer) {
        munmap(fb->video_win1.buffer, fb->video_win1.video_ion.size);
        fb->video_win1.buffer = NULL;
        fb->video_win1.video_ion.buffer = NULL;
    }

    if (fb->video_win1.video_ion.fd) {
        close(fb->video_win1.video_ion.fd);
        fb->video_win1.video_ion.fd = 0;
    }

    if (fb->video_win1.video_ion.client) {
        if (fb->video_win1.video_ion.handle) {
            ret = ion_free(fb->video_win1.video_ion.client,
                           fb->video_win1.video_ion.handle);
            if (ret)
                printf("ion_free failed!\n");
            fb->video_win1.video_ion.handle = 0;
        }

        ion_close(fb->video_win1.video_ion.client);
        fb->video_win1.video_ion.client = 0;
    }

    return 0;
}

static unsigned int rk_fb_color_key_888to565(struct color_key key)
{
    printf("%s\n", __func__);
    key.red &= 0xF8;
    key.green &= 0xFC;
    key.blue &= 0xF8;

    return ((key.enable << 24) | ((key.red & 0xFF) << 16)
            | ((key.green & 0xFF) << 8) | (key.blue & 0xFF));
}


int rk_fb_set_color_key(struct color_key color_key)
{
    struct color_key_cfg key;

    printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;
    key.win0_color_key_cfg = 0;
    key.win1_color_key_cfg = rk_fb_color_key_888to565(color_key);
    key.win2_color_key_cfg = 0;
    if (ioctl(g_fb->fd, RK_FBIOPUT_COLOR_KEY_CFG, (unsigned long)(&key), 0) < 0) {
        printf("RK_FBIOPUT_COLOR_KEY_CFG failed.\n");
        return -1;
    }
    return 0;
}

struct win *rk_fb_getuiwin(void)
{
    printf("%s\n", __func__);
    if (g_fb == 0)
        return NULL;

    return &g_fb->win0;
}

struct win *rk_fb_getvideowin(void)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return NULL;

    return g_fb->video_win_write;
}

int rk_fb_getvideowins(struct win **win_write, struct win **win_disp)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;

    *win_write = g_fb->video_win_write;
    *win_disp = g_fb->video_win_disp;

    return 0;
}

int rk_fb_video_disp(struct win * winmsg)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;

    if (g_fb->video_win_disp != winmsg && g_fb->video_win_write == winmsg) {
        g_fb->video_win_write = g_fb->video_win_disp;
        g_fb->video_win_disp = winmsg;
    }
    if (!winmsg->video_ion.buffer && winmsg->buf_ion_fd <= 0) {
        return 0;
    }
    return rk_fb_video_config(g_fb, winmsg);
}

int rk_fb_video_disp_ex(struct win * winmsg, int w, int h)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;

    if (g_fb->video_win_disp != winmsg && g_fb->video_win_write == winmsg) {
        g_fb->video_win_write = g_fb->video_win_disp;
        g_fb->video_win_disp = winmsg;
    }
    if (!winmsg->video_ion.buffer && winmsg->buf_ion_fd <= 0) {
        return 0;
    }
    return rk_fb_video_config(g_fb, winmsg);
}

int rk_fb_ui_disp(struct win * win)
{
    //printf("%s\n", __func__);
//Ui and video config can not be called on different threads,
//so close it first

    if (g_fb == 0)
        return -1;

    //rk_fb_ui_config(g_fb);

    return 0;
}

int rk_fb_ui_disp_ext(void)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;
    return rk_fb_ui_disp(&g_fb->win0);
}

int rk_fb_get_pixel_format(void)
{
    return g_fb->pixel_format;
}

int rk_fb_get_cvbsout_mode(void)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);

    read_fp = open("/sys/class/display/TV/mode", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer, 11);

        if (!memcmp(buffer, "720x576i-50", 11)) {
            ret = OUT_DEVICE_CVBS_PAL;
        } else {
            ret = OUT_DEVICE_CVBS_NTSC;
        }
        close(read_fp);
    }
    printf("%s cvbsout enable = %d\n", __func__, ret);
    return ret;
}

struct fb* rk_fb_init(int pixel_format)
{
    int out_type = OUT_DEVICE_LCD;
    printf("%s\n", __func__);

    if (g_fb) {
        return g_fb;
    }

    rkfb_init_lock();

    g_fb = (struct fb*)calloc(1, sizeof(struct fb));
    rk_fb_open(g_fb);

    if (ioctl(g_fb->fd, FBIOGET_VSCREENINFO, &g_fb->vi) < 0) {
        printf("Error:fb read vi info failed.\n");
        return 0;
    }
    g_fb->screen_state = 1;//rk_fb_get_screen_state_ex();

    g_fb->pixel_format = pixel_format;
    g_fb->vi.reserved[0] = 0;
    g_fb->vi.reserved[1] = 0;
    g_fb->vi.reserved[2] = 0;
    g_fb->vi.xoffset = 0;
    g_fb->vi.yoffset = 0;
    g_fb->vi.activate = FB_ACTIVATE_NOW;

    if (pixel_format == FB_FORMAT_RGB_565) {
        /*
         * Explicitly request 5/6/5
         */
        g_fb->vi.bits_per_pixel = 16;
        g_fb->vi.red.offset     = 11;
        g_fb->vi.red.length     = 5;
        g_fb->vi.green.offset   = 5;
        g_fb->vi.green.length   = 6;
        g_fb->vi.blue.offset    = 0;
        g_fb->vi.blue.length    = 5;
        g_fb->vi.transp.offset  = 0;
        g_fb->vi.transp.length  = 0;
        g_fb->vi.nonstd &= 0xffffff00;
        g_fb->vi.nonstd |= HAL_PIXEL_FORMAT_RGB_565;
    } else { /* FB_FORMAT_BGRA_8888 */
        /*
         * Explicitly request 8/8/8
         */
        g_fb->vi.bits_per_pixel = 32;
        g_fb->vi.red.offset     = 16;
        g_fb->vi.red.length     = 8;
        g_fb->vi.green.offset   = 8;
        g_fb->vi.green.length   = 8;
        g_fb->vi.blue.offset    = 0;
        g_fb->vi.blue.length    = 8;
        g_fb->vi.transp.offset  = 24;
        g_fb->vi.transp.length  = 8;
        g_fb->vi.grayscale      &= 0xff;
        g_fb->vi.grayscale      |= (g_fb->vi.xres << 8) + (g_fb->vi.yres << 20);
        g_fb->vi.nonstd &= 0xffffff00;
        g_fb->vi.nonstd |= HAL_PIXEL_FORMAT_BGRA_8888;
    }

    /*
     * Request NUM_BUFFERS screens (at lest 2 for page flipping)
     */
    g_fb->vi.yres_virtual = g_fb->vi.yres * 1;

    if (ioctl(g_fb->fd, FBIOPUT_VSCREENINFO, &g_fb->vi) == -1)
        g_fb->vi.yres_virtual = g_fb->vi.yres;
    disp_w = g_fb->vi.xres;
    disp_h = g_fb->vi.yres;
    disp_w_max = g_fb->vi.xres;
    disp_h_max = g_fb->vi.yres;
    if (rk_fb_get_hdmi_connect() == 1) {
        out_type = OUT_DEVICE_HDMI;
    } else if ((rk_fb_get_cvbsout_connect() == 1) &&
               (rk_fb_get_cvbsout_enabled() == 1)) {
        out_type = rk_fb_get_cvbsout_mode();
    }
    rk_fb_ui_init(g_fb, g_fb->vi.xres, g_fb->vi.yres, pixel_format);
    rk_fb_video_init(g_fb, g_fb->vi.xres, g_fb->vi.yres);
    rk_fb_config_init(g_fb);
    rk_fb_set_out_device(out_type);
    return g_fb;
}

int rk_fb_deinit(void)
{
    printf("%s\n", __func__);
    if (g_fb) {
        rk_fb_ui_dispblack(g_fb);
        rk_fb_config_deinit(g_fb);
        rk_fb_ui_deinit(g_fb);
        rk_fb_video_deinit(g_fb);
        rk_fb_close(g_fb);
        memset(g_fb, 0, sizeof(struct fb));
        free(g_fb);
        g_fb = 0;
    }

    rkfb_destroy_lock();
    return 0;
}

int rk_fb_runcmd(char* cmd)
{
    char buffer[BUFSIZ];
    FILE* read_fp;
    int chars_read;
    int ret;

    memset(buffer, 0, BUFSIZ);
    read_fp = popen(cmd, "r");
    if (read_fp != NULL) {
        chars_read = fread(buffer, sizeof(char), BUFSIZ - 1, read_fp);
        if (chars_read > 0) {
            printf("%s, %s\n", __func__, buffer);
            ret = 1;
        } else {
            ret = -1;
        }
        pclose(read_fp);
    } else {
        ret = -1;
    }

    return ret;
}

int rk_fb_get_screen_state(void)
{
#if 0
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];

    read_fp = open("/sys/class/graphics/fb0/enable", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer,  7);
        if (!strcmp(buffer, "disable")) {
            ret = 0;
        } else {
            ret = 1;
        }
        close(read_fp);
    }
    return ret;
#else
    return g_fb->screen_state;
#endif
}

int rk_fb_get_screen_state_ex(void)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];

    read_fp = open("/sys/class/graphics/fb0/enable", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer,  7);
        if (!strcmp(buffer, "disable")) {
            ret = 0;
        } else {
            ret = 1;
        }
        close(read_fp);
    }
    return ret;
}

void rk_fb_screen_on(void)
{
    g_fb->screen_state = 1;
    rk_fb_runcmd("echo 0 > /sys/class/graphics/fb0/blank");
}

void rk_fb_screen_off(void)
{
    g_fb->screen_state = 0;
    rk_fb_ui_dispblack(g_fb);
    rk_fb_video_dispblack(g_fb);
    rk_fb_runcmd("echo 4 > /sys/class/graphics/fb0/blank");
}

int rk_fb_set_lcd_backlight(int level)
{
    int fd;
    int ret;
    char brightness[5];
    char* blpath = "/sys/class/backlight/rk28_bl/brightness";

    if (level < 0 || level > 255) {
        printf("unsupport this backlight level: %d\n", level);
        return -1;
    }

    sprintf(brightness, "%d\n", level);
    fd = open(blpath, O_RDWR);
    if (fd < 0) {
        printf("open backlight device failed\n");
        return -1;
    }

    ret = write(fd, brightness, sizeof(int));
    if (ret <= 0) {
        printf("write failed :%d\n", ret);
    }

    close(fd);

    return 0;
}

int rk_fb_get_hdmi_mode(int *w, int *h)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);

    read_fp = open("/sys/class/display/HDMI/mode", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer, 13);
        printf("%s mode = %s\n", __func__, buffer);

        if (buffer[0] == '1' && buffer[1] == '9' && buffer[5] == '1' && buffer[6] == '0') {
            *w = 1920;
            *h = 1080;
        }
        if (buffer[0] == '1' && buffer[1] == '2' && buffer[5] == '7' && buffer[6] == '2') {
            *w = 1280;
            *h = 720;
        }
        if (buffer[0] == '7' && buffer[1] == '2' && buffer[4] == '5' && buffer[5] == '7') {
            *w = 720;
            *h = 576;
        }
        if (buffer[0] == '7' && buffer[1] == '2' && buffer[4] == '4' && buffer[5] == '8') {
            *w = 720;
            *h = 480;
        }
        if (((*w) * (*h)) > (HDMI_OUT_W_MAX * HDMI_OUT_H_MAX)) {
            *w = HDMI_OUT_W_MAX;
            *h = HDMI_OUT_H_MAX;
        }
        ret = 0;
        close(read_fp);
    }
    printf("%s ret = %d\n", __func__, ret);
    return ret;
}

int rk_fb_set_out_device(int device)
{
    //printf("%s, device = %d\n", __func__, device);
    if (g_fb == 0)
        return -1;
    if (device > OUT_DEVICE_CVBS_NTSC || device < OUT_DEVICE_LCD)
        return -1;
    g_fb->out_device = device;
    if (g_fb->out_device == OUT_DEVICE_LCD) {
        disp_w = g_fb->win0.width;
        disp_h = g_fb->win0.height;
    } else if (g_fb->out_device == OUT_DEVICE_HDMI) {
        rk_fb_get_hdmi_mode(&disp_w, &disp_h);
    } else if (g_fb->out_device == OUT_DEVICE_CVBS_PAL) {
        disp_w = 720;
        disp_h = 576;
    } else if (g_fb->out_device == OUT_DEVICE_CVBS_NTSC) {
        disp_w = 720;
        disp_h = 480;
    }
    rk_fb_ui_dispblack(g_fb);
    rk_fb_ui_config(g_fb);
}

int rk_fb_get_out_device(int *w, int *h)
{
    //printf("%s\n", __func__);
    if (g_fb == 0)
        return -1;

    *w = disp_w;
    *h = disp_h;

    return g_fb->out_device;
}

int rk_fb_get_resolution(int *width, int *height)
{
    if (g_fb == 0)
        return -1;
    *width = g_fb->win0.width;
    *height = g_fb->win0.height;
    return 0;
}

int rk_fb_get_hdmi_connect(void)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);

    read_fp = open("/sys/class/display/HDMI/connect", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer,  7);

        if (disp_w_max * disp_h_max < HDMI_OUT_W_MAX * HDMI_OUT_H_MAX) {
            disp_w_max = HDMI_OUT_W_MAX;
            disp_h_max = HDMI_OUT_H_MAX;
        }

        if (buffer[0] == '0') {
            ret = 0;
        } else {
            ret = 1;
        }
        close(read_fp);
    }
    printf("%s hdmi connect = %d\n", __func__, ret);
    return ret;
}

int rk_fb_get_cvbsout_connect(void)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);

    read_fp = open("/sys/class/display/TV/connect", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer,  7);
        if (disp_w_max * disp_h_max < 720 * 576) {
            disp_w_max = 720;
            disp_h_max = 576;
        }
        if (buffer[0] == '0') {
            ret = 0;
        } else {
            ret = 1;
        }
        close(read_fp);
    }
    printf("%s cvbsout connect = %d\n", __func__, ret);
    return ret;
}

int rk_fb_get_cvbsout_enabled(void)
{
    int ret = -1;
    int read_fp;
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);

    read_fp = open("/sys/class/display/TV/enable", O_RDONLY);
    if (read_fp != -1) {
        int size = read(read_fp, buffer,  7);

        if (buffer[0] == '0') {
            ret = 0;
        } else {
            ret = 1;
        }
        close(read_fp);
    }
    printf("%s cvbsout enable = %d\n", __func__, ret);
    return ret;
}

void rk_fb_set_flip(int flip)
{
    need_flip = flip;
}

int rk_fb_get_flip(void)
{
    return need_flip;
}

int rk_fb_recalc_rotate(int rotate)
{
    if (rk_fb_get_flip()) {
        rotate += 180;
        if (rotate >= 360)
            rotate -= 360;
    }

    return rotate;
}

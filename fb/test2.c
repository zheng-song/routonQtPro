#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include "rk_fb.h"

struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;

static struct fb *fb0 = NULL;
unsigned char * framebuffer0;
unsigned char * framebuffer1;

void drawRect (int x0, int y0, int width, int height, int color,unsigned char * framebuffer);

int main(int argc, char const *argv[])
{
	long int screensize = 0;
	int console_fd,ret,fdfb;
    int i = 0;

    fb0 = rk_fb_init(FB_FORMAT_BGRA_8888);
    if(!fb0) {
        fprintf(stderr,"struct fb alloc failed.\n");
        return -1;
    }

    framebuffer0 = fb0->win0.buffer;
    // framebuffer0 = fb0->video_win0.buffer;
    // framebuffer1 = fb0->video_win1.buffer;

    // while ( i < 20)
    // {
        // struct win *show = rk_fb_getvideowin();
        // framebuffer0 = show->buffer;
        // drawRect (vinfo.xres / 8, vinfo.yres / 8,
                 // vinfo.xres / 4, vinfo.yres / 4, 0x00000000,framebuffer0);
       // drawRect (vinfo.xres * 5 / 8, vinfo.yres * 5 / 8,
       //          vinfo.xres / 4, vinfo.yres / 4, 0xff0000ff,framebuffer0);
       drawRect (0, 0,
                vinfo.xres, vinfo.yres, 0xff0000ff,framebuffer0);

        // rk_fb_video_disp(&(fb0->video_win0));
        // sleep(2);
        // i++;
    // }

    rk_fb_deinit();

	return 0;
}

void drawRect (int x0, int y0, int width, int height, int color,unsigned char * framebuffer)
{
    const int bytesPerPixel = 4;
    const int stride = finfo.line_length / bytesPerPixel;

    int *dest = (int *) (framebuffer)
        + (y0 + vinfo.yoffset) * stride + (x0 + vinfo.xoffset);

    int x, y;
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            dest[x] = color;
        }
        dest += stride;
    }
}
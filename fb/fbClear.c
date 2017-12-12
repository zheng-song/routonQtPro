#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/mman.h>
#include <string.h>
int main()
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;
    fbfd = open("/dev/fb0", O_RDWR);
// Open the file for reading and writing
    if (!fbfd)
    {
        printf("Error: cannot open framebuffer device.\n");
        exit(0);
    }
    printf("The framebuffer device was opened successfully.\n");


// Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information.\n");
        exit(0);
    }


    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information.\n");
        exit(0);
    }
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
            // Figure out the size of the screen in bytes


    vinfo.bits_per_pixel = 32;
    vinfo.red.offset = 16;
    vinfo.red.length = 8;
    vinfo.green.offset = 8;
    vinfo.green.length = 8;
    vinfo.blue.offset = 0;
    vinfo.blue.length = 8;
    vinfo.transp.offset = 24;
    vinfo.transp.length = 8;
    vinfo.grayscale &= 0xff;
    vinfo.grayscale |= (vinfo.xres<<8) + (vinfo.yres << 20);
    vinfo.nonstd &= 0xffffff00;
    vinfo.nonstd |= 5;//HAL_PIXEL_FORMAT_BGRA_8888
    vinfo.yres_virtual = vinfo.yres * 1;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo) == -1)
        vinfo.yres_virtual = vinfo.yres;


    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information.\n");
        exit(0);
    }
    printf(" after reset %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
            // Figure out the size of the screen in bytes

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
            // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);


    if(fbp == NULL)
    {
        printf("Error: failed to map framebuffer device to memory.\n");
        exit(0);
    }


    printf("The framebuffer device was mapped to memory successfully.\n");

//    printf(" read a point of framebuffer:\n %d:%d:%d:%d\n %d:%d:%d:%d\n %d:%d:%d:%d\n",*(fbp),*(fbp+1),*(fbp+2),*(fbp+3),*(fbp+4),*(fbp+5),*(fbp+6),*(fbp+7),*(fbp+8),*(fbp+9),*(fbp+10),*(fbp+11));





//    memset(fbp,0, screensize);
    for(int i = 0;i<screensize;i++)
    {
        if( i&1 == 1)
        {
            *(fbp+i)=0;
        }
        else
        {
            *(fbp+i)=1;
        }
    }



#if 0
    x = 100; y = 100;
// Where we are going to put the pixel
// Figure out where in memory to put the pixel
    for (y = 100; y < 300; y++)
        for (x = 100; x < 300; x++)
        {
            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +(y+vinfo.yoffset) * finfo.line_length;
            if (vinfo.bits_per_pixel == 32)
            {
//                *(fbp + location) = 100; // Some blue
//                *(fbp + location + 1) = 15+(x-100)/2;// A little green
//                *(fbp + location + 2) = 200-(y-100)/5;// A lot of red
//                *(fbp + location + 3) = 0;// No transparency

                *(fbp + location) = 0; // Some blue
                *(fbp + location + 1) = 0;// A little green
                *(fbp + location + 2) = 0;// A lot of red
                *(fbp + location + 3) = 0;// No transparency
            }
            else
            {
//                int b = 10;//assume 16bpp
//                int g = (x-100)/6;// A little green
//                int r = 31-(y-100)/16;// A lot of red
                int b = 0;//assume 16bpp
                int g = 0;// A little green
                int r = 0;// A lot of red
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
        }
#endif
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}

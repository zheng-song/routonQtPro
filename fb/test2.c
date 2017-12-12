#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "rk_fb.h"

int main(int argc, char const *argv[])
{
    struct fb *fb0 = NULL;

    fb0 = rk_fb_init(FB_FORMAT_RGB_565);
    if(!fb0) {
        fprintf(stderr,"struct fb alloc failed.\n");
        return -1;
    }
    rk_fb_deinit();

	return 0;
}

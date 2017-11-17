#include <iostream>

using namespace std;

#define __STDC_CONSTANT_MACROS

extern "C
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}


int main()
{

    cout << "Hello FFmpeg!" << endl;
    av_register_all();
    unsigned version = avcodec_version();
    cout << "version is:" << version;
    return 0;
}

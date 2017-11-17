#!/bin/sh

# if QT_QPA_FB_ROTATION90=1 then /dev/input/event1:rotate=270
# if QT_QPA_FB_ROTATION270=1 then /dev/input/event1:rotate=90

export QT_QPA_FB_ROTATION90=0
export QT_QPA_FB_ROTATION270=1
export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=/dev/input/event1:rotate=90


export QT_ROOT=/data/zhengsong
export LD_LIBRARY_PATH=/lib:$LD_LIBRARY_PATH:$TSLIB_ROOT/lib
export LD_LIBRARY_PATH=$QT_ROOT/lib:$LD_LIBRARY_PATH
export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0:mmsize=62x110
export QT_QPA_FONTDIR=$QT_ROOT/lib/fonts
export QT_QPA_GENERIC_PLUGINS=evdevtouch:/dev/input/event1

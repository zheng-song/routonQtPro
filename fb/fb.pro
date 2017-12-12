TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ion.c \
    rk_fb.c \
    test2.c \
    ion.c \
    rk_fb.c \
    test2.c \
    fbClear.c

HEADERS += \
    ion/ion.h \
    linux/ion.h \
    linux/rockchip_ion.h \
    uapi/ion.h \
    rk_fb.h

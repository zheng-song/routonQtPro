#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T21:49:52
#
#-------------------------------------------------

QT      += core gui   \
      network \
      xml \
      multimedia \
      multimediawidgets \
      widgets \
      sql



TARGET = test_qmplay
TEMPLATE = app


SOURCES += main.cpp\
        Widget.cpp \
    WWWNOTE.cpp \
    VideoWidget.cpp \
    MainSlider.cpp \
    VolumeSlider.cpp \
    ListWidget.cpp \
    BottomWidget.cpp



HEADERS  += Widget.h \
    VideoWidget.h \
    MainSlider.h \
    VolumeSlider.h \
    ListWidget.h \
    BottomWidget.h


FORMS    += Widget.ui

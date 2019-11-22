#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../garlic-lib/

QT     += core gui sql xml widgets webenginewidgets
CONFIG += warn_on c++11
#temporary ToDO
QMAKE_CXXFLAGS += -Wno-deprecated-declarations

#Delete or comment out the next line when you want to use QTMultimedia
CONFIG    += support_qtav # alternatives: support_qtav support_qtmm support_libvlc

#DEFINES += SUPPORT_EMBEDDED
#DEFINES += SUPPORT_RPI

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += avwidgets
    CONFIG  += avwidgets
    unix{    # QtAV has "this use of defined may not be portable" issues with newer gcc
        QMAKE_CXXFLAGS += -Wno-expansion-to-defined
    }
}
support_libvlc{
    DEFINES += SUPPORT_LIBVLC
    LIBS += -lvlc -lX11
}
support_qtmm {
     DEFINES += SUPPORT_QTMM
     QT += multimedia multimediawidgets
}

DEFINES += QT_DEPRECATED_WARNINGS QUAZIP_STATIC

TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../bin
INCLUDEPATH+=../ext/zlib/includes

include(../player-common/common.pri)

SOURCES += \
    media/base_media.cpp \
    media/image.cpp \
    media/video.cpp \
    media/web.cpp \
    media/audio.cpp \
    media/media_factory.cpp \
    main.cpp \
    mainwindow.cpp \
    mediaplayer_wrapper.cpp \
    mediawidget_wrapper.cpp \
    mm_libs/qtav_decoder.cpp \
    mm_libs/qtav_widget.cpp \
    mm_libs/qtmm_decoder.cpp \
    mm_libs/qtmm_widget.cpp \
    mm_libs/vlc_decoder.cpp \
    mm_libs/vlc_widget.cpp \
    region.cpp

HEADERS  += \
    media/base_media.h \
    media/image.h \
    media/video.h \
    media/web.h \
    media/audio.h \
    media/media_factory.h \
    mainwindow.h \
    mediaplayer_wrapper.h \
    mediawidget_wrapper.h \
    mm_libs/qtav_decoder.h \
    mm_libs/qtav_widget.h \
    mm_libs/qtmm_decoder.h \
    mm_libs/qtmm_widget.h \
    mm_libs/vlc_decoder.h \
    mm_libs/vlc_widget.h \
    region.h

unix{
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

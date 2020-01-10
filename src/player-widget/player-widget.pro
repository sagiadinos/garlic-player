#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../garlic-lib/

QT     += core gui sql xml widgets webenginewidgets
CONFIG += warn_on c++11

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
    HEADERS  += \
        mm_libs/qtav_decoder.cpp \
        mm_libs/qtav_widget.cpp
    SOURCES += \
        mm_libs/qtav_decoder.cpp \
        mm_libs/qtav_widget.cpp
}
support_libvlc{
    DEFINES += SUPPORT_LIBVLC
    LIBS += -lvlc -lX11
    HEADERS  += \
        mm_libs/vlc_decoder.cpp \
        mm_libs/vlc_widget.cpp
    SOURCES += \
        mm_libs/vlc_decoder.cpp \
        mm_libs/vlc_widget.cpp
}
support_qtmm {
     DEFINES += SUPPORT_QTMM
     QT += multimedia multimediawidgets
    HEADERS  += \
        mm_libs/qtmm_decoder.cpp \
        mm_libs/qtmm_widget.cpp
    SOURCES += \
        mm_libs/qtmm_decoder.cpp \
        mm_libs/qtmm_widget.cpp
}

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../bin
INCLUDEPATH+=../ext/zlib/includes

include(../player-common/common.pri)

SOURCES += \
    media/image.cpp \
    media/player_base_media.cpp \
    media/video.cpp \
    media/web.cpp \
    media/audio.cpp \
    media/media_factory.cpp \
    main.cpp \
    mainwindow.cpp \
    mediaplayer_wrapper.cpp \
    mediawidget_wrapper.cpp \
    region.cpp

HEADERS  += \
    media/image.h \
    media/player_base_media.h \
    media/video.h \
    media/web.h \
    media/audio.h \
    media/media_factory.h \
    mainwindow.h \
    mediaplayer_wrapper.h \
    mediawidget_wrapper.h \
    region.h

# workaround for >Qt5.11 https://github.com/wang-bin/QtAV/issues/1231
unix:!mac {
    LIBS += -L$$QT.core.libs -lQtAV
}
unix:mac {
    INCLUDEPATH += $$QT.core.libs/QtAV.framework/Versions/1/Headers
    QMAKE_LFLAGS += -F$$QT.core.libs
    LIBS += -framework QtAV
}
win32{
    LIBS += -L$$QT.core.libs -lQtAV1
}

unix{
    LIBS += -L../lib -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
    #temporary ToDO
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
}
win32 {
    #libcarlic is static compiled in windows
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

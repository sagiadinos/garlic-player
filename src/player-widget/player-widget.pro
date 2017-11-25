#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../garlic-lib/

QT     += core gui xml widgets webenginewidgets
CONFIG += warn_on c++11
Release:DEFINES += QT_NO_DEBUG_OUTPUT

#Delete or comment out the next line when you want to use QTMultimedia
CONFIG    += support_qtav

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += avwidgets
    # QtAV has "this use of defined may not be portable" issues with newer gcc
    QMAKE_CXXFLAGS += -Wno-expansion-to-defined
} else {
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
    mediadecoderwrapper.cpp \
    mediaviewwrapper.cpp \
    region.cpp

HEADERS  += \
    media/base_media.h \
    media/image.h \
    media/video.h \
    media/web.h \
    media/audio.h \
    media/media_factory.h \
    mainwindow.h \
    mediadecoderwrapper.h \
    mediaviewwrapper.h \
    region.h

unix{
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../parser/smilparser

QT     += core gui xml widgets webenginewidgets
CONFIG += warn_on c++11

#Comment the next line in when you want to use QTMultimedia
CONFIG    += support_qtav

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += avwidgets
} else {
    QT += multimedia multimediawidgets
}
DEFINES += QUAZIP_STATIC

TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../../bin
INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes

SOURCES += main.cpp\
        mainwindow.cpp \
    region.cpp \
    mediadecoderwrapper.cpp \
    mediaviewwrapper.cpp

HEADERS  += mainwindow.h \
    region.h \
    mediadecoderwrapper.h \
    mediaviewwrapper.h

LIBS += -L../lib -lgarlic -lquazip -lzlib

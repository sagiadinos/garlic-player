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
DESTDIR = ../bin
INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes

SOURCES += main.cpp\
        mainwindow.cpp \
    region.cpp \
    mediadecoderwrapper.cpp \
    mediaviewwrapper.cpp \
    configdialog.cpp \
    screen.cpp \
    cmdparser.cpp

HEADERS  += mainwindow.h \
    region.h \
    mediadecoderwrapper.h \
    mediaviewwrapper.h \
    configdialog.h \
    screen.h \
    cmdparser.h

unix{
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

DISTFILES +=

FORMS += \
    configdialog.ui

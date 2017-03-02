#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)


QT     += core gui xml widgets webenginewidgets
CONFIG += warn_on c++11

#Comment the next line in when you want to use QTMultimedia
CONFIG    += support_qtav

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += avwidgets
    CONFIG  += avwidgets
} else {
    QT += multimedia multimediawidgets
}


TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../bin

SOURCES += main.cpp\
        mainwindow.cpp \
    region.cpp

HEADERS  += mainwindow.h \
    region.h

LIBS += -L../lib -lgarlic

#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)

QT     += core gui xml widgets avwidgets webkitwidgets
CONFIG += warn_on avwidgets c++11

TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../bin

SOURCES += main.cpp\
        mainwindow.cpp \
    region.cpp

HEADERS  += mainwindow.h \
    region.h

LIBS += -L../lib -lgarlic

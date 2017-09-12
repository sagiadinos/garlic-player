#-------------------------------------------------
#
# Project created by QtCreator 2017-09-09T19:59:00
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../parser/smilparser

QT     += core widgets gui xml quick qml
CONFIG += warn_on c++11

DEFINES += QUAZIP_STATIC

TARGET = garlic-player
TEMPLATE = app
INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes

SOURCES += main.cpp\
        mainwindow.cpp \
    region.cpp \
    configdialog.cpp \
    screen.cpp \
    cmdparser.cpp

HEADERS  += mainwindow.h \
    region.h \
    configdialog.h \
    screen.h \
    cmdparser.h

unix:!android {
    QT += webengine
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
android {
    QT += androidextras
    LIBS += -L../libandroid -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

RESOURCES += qml.qrc

FORMS += \
    configdialog.ui

DISTFILES +=

#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T05:47:20
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml testlib
QT       -= gui
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app
LIBS += -L../../lib -lgarlic

TARGET = tst_timings
SOURCES += \
    tst_timings.cpp

HEADERS +=

#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T06:03:43
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml testlib
QT       -= gui
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app
LIBS += -L../../lib -lgarlic

TARGET = tst_testtconfiguration
SOURCES += tst_testtconfiguration.cpp

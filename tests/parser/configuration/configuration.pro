#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T06:03:43
#
#-------------------------------------------------
include(../../../src/defaults.pri)

QT       += xml testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_configuration
SOURCES += tst_configuration.cpp

include(../../tests.pri)

RESOURCES += config.xml

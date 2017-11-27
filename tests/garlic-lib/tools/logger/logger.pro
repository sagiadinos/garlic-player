#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T19:12:06
#
#-------------------------------------------------
include(../../../../src/defaults.pri)

QT       += xml testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += tst_logger.cpp

include(../../../tests.pri)

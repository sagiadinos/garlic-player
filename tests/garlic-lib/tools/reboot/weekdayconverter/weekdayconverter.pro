include(../../../../../src/defaults.pri)

QT       += xml testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += tst_weekdayconverter.cpp

include(../../../../tests.pri)

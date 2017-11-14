#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T05:47:20
#
#-------------------------------------------------
include(../../../../src/defaults.pri)

QT       += xml testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_timings
SOURCES += tst_timings.cpp

include(../../../tests.pri)

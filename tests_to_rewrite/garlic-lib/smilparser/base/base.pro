#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T06:28:30
#
#-------------------------------------------------
include(../../../../src/defaults.pri)

QT       += xml testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_base
SOURCES += tst_base.cpp

include(../../../tests.pri)

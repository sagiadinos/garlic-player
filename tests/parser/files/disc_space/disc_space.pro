#-------------------------------------------------
#
# Project created by QtCreator 2017-09-18T13:15:12
#
#-------------------------------------------------
include(../../../../src/defaults.pri)

QT       += network testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_disco_space
SOURCES += \
    tst_disc_space.cpp


include(../../../tests.pri)

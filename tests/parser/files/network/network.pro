#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T15:40:45
#
#-------------------------------------------------
include(../../../../src/defaults.pri)

QT       += xml network testlib
QT       -= gui

CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_network
SOURCES += tst_network.cpp

include(../../../tests.pri)
RESOURCES     = resources.qrc

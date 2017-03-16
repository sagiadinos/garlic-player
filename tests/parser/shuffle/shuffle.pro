#-------------------------------------------------
#
# Project created by QtCreator 2016-03-26T02:43:55
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml testlib
QT       -= gui

TARGET = tst_shuffle
CONFIG   += console testcase c++11
CONFIG   -= app_bundle

TEMPLATE = app
SOURCES += tst_shuffle.cpp

include(../tests.pri)

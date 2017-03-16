#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T20:27:37
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml testlib
QT       -= gui
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app

TARGET = tst_excl
SOURCES += tst_excl.cpp

include(../tests.pri)

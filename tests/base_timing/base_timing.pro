#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T22:35:28
#
#-------------------------------------------------

include(../../defaults.pri)

QT       += xml testlib
QT       -= gui
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app
LIBS += -L../../lib -lgarlic

TARGET = tst_basetiming
SOURCES += tst_basetiming.cpp

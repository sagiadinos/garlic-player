#-------------------------------------------------
#
# Project created by QtCreator 2016-03-05T19:38:17
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml network testlib
QT       -= gui
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app
LIBS += -L../../lib -lgarlic

TARGET = tst_downloader
SOURCES += tst_downloader.cpp

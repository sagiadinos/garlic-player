#-------------------------------------------------
#
# Project created by QtCreator 2016-03-21T22:47:38
#
#-------------------------------------------------
include(../../defaults.pri)

QT       += xml testlib
QT       -= gui

TARGET = tst_tseq
CONFIG   += console testcase c++11
CONFIG   -= app_bundle
TEMPLATE = app
SOURCES += \
    tst_seq.cpp

include(../tests.pri)

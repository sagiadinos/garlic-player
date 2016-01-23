include(../defaults.pri)
QT += testlib core network xml
QT -= gui

TARGET = tests
CONFIG += warn_on console c++11
CONFIG -= app_bundle
TEMPLATE = app
LIBS += -L../lib -lgarlic

SOURCES += main.cpp \
    test_configuration.cpp \
    test_smil_head.cpp \
    test_smil_base.cpp \
    test_smil_playlist.cpp \
    test_smil_timings.cpp \
    test_smil_image.cpp

HEADERS += \
    AutoTest.h \
    test_configuration.h \
    test_smil_head.h \
    test_smil_base.h \
    test_smil_playlist.h \
    test_smil_timings.h \
    test_smil_image.h

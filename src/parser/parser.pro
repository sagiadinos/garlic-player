include(../defaults.pri)

QT       += core network xml widgets
TEMPLATE  = lib
CONFIG    += warn_on staticlib c++11 stl
DEFINES += QUAZIP_STATIC
Release:DEFINES += QT_NO_DEBUG_OUTPUT

TARGET = garlic
LIBS += -L../ext -lquazip -lzlib

!android:DESTDIR = ../lib/
android:DESTDIR = ../libandroid/

INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes


SOURCES += \
    smilparser/head.cpp \
    smilparser/media.cpp \
    smilparser/image.cpp \
    smilparser/seq.cpp \
    smilparser/video.cpp \
    smilparser/web.cpp \
    smilparser/audio.cpp \
    smilparser/body.cpp \
    smilparser/par.cpp \
    smilparser/excl.cpp \
    files/downloader.cpp \
    smilparser/timings/timing.cpp \
    smilparser/timings/clock_value.cpp \
    smilparser/priorityclass.cpp \
    smilparser/factory.cpp \
    smilparser/container.cpp \
    smilparser/base.cpp \
    smilparser/base_timing.cpp \
    tools/configuration.cpp \
    files/file_manager.cpp \
    smilparser.cpp \
    smilparser/prefetch.cpp \
    smilparser/tools/shuffle.cpp \
    files/index.cpp \
    tools/logging_categories.cpp \
    files/network.cpp
HEADERS += tools/configuration.h \
    smilparser.h \
    smilparser/head.h \
    smilparser/image.h \
    smilparser/media.h \
    smilparser/seq.h \
    smilparser/video.h \
    smilparser/web.h \
    smilparser/audio.h \
    smilparser/body.h \
    smilparser/par.h \
    smilparser/excl.h \
    files/downloader.h \
    smilparser/timings/timing.h \
    smilparser/timings/clock_value.h \
    smilparser/priorityclass.h \
    smilparser/factory.h \
    smilparser/container.h \
    smilparser/base.h \
    smilparser/base_timing.h \
    files/file_manager.h \
    smilparser/prefetch.h \
    smilparser/tools/shuffle.h \
    files/index.h \
    tools/logging_categories.h \
    files/network.h

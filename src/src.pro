include(../defaults.pri)

QT       += core network xml gui avwidgets webkitwidgets
TEMPLATE  = lib
CONFIG   += warn_on avwidgets lib c++11

TARGET = garlic
DESTDIR = ../lib

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
    smilparser/index_file.cpp \
    tools/configuration.cpp \
    files/file_manager.cpp \
    smilparser.cpp
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
    smilparser/index_file.h \
    files/file_manager.h

DISTFILES +=

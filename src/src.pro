include(../defaults.pri)

QT       += core network xml gui avwidgets webkitwidgets
TEMPLATE  = lib
CONFIG   += warn_on avwidgets lib c++11

TARGET = garlic
DESTDIR = ../lib

SOURCES += downloader.cpp \
    configuration.cpp \
    smilparser/file.cpp \
    smilparser/head.cpp \
    smilparser/media.cpp \
    smilparser/image.cpp \
    smilparser/seq.cpp \
    smilparser.cpp \
    smilparser/video.cpp \
    smilparser/web.cpp \
    smilparser/audio.cpp \
    smilparser/body.cpp \
    smilparser/par.cpp \
    smilparser/excl.cpp \
    smilparser/base.cpp \
    smilparser/playlist.cpp \
    smilparser/timings/timing.cpp \
    smilparser/timings/clock_value.cpp \
    smilparser/priorityclass.cpp
HEADERS += downloader.h \
    configuration.h \
    smilparser.h \
    smilparser/file.h \
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
    smilparser/base.h \
    smilparser/playlist.h \
    smilparser/timings/timing.h \
    smilparser/timings/clock_value.h \
    smilparser/priorityclass.h

DISTFILES +=

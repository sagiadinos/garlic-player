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
    files/base_manager.cpp \
    files/disc_space.cpp \
    files/index_manager.cpp \
    files/index_model.cpp \
    files/media_manager.cpp \
    files/media_model.cpp \
    files/network.cpp \
    files/network_queue.cpp \
    files/wgt.cpp \
    smilparser/timings/clock_value.cpp \
    smilparser/timings/timing.cpp \
    smilparser/tools/shuffle.cpp \
    smilparser/audio.cpp \
    smilparser/base.cpp \
    smilparser/base_timing.cpp \
    smilparser/body.cpp \
    smilparser/container.cpp \
    smilparser/excl.cpp \
    smilparser/factory.cpp \
    smilparser/head.cpp \
    smilparser/image.cpp \
    smilparser/media.cpp \
    smilparser/par.cpp \
    smilparser/prefetch.cpp \
    smilparser/priorityclass.cpp \
    smilparser/seq.cpp \
    smilparser/video.cpp \
    smilparser/web.cpp \
    tools/configuration.cpp \
    tools/logging_categories.cpp \
    smilparser.cpp \

HEADERS += \
    files/base_manager.h \
    files/disc_space.h \
    files/index_manager.h \
    files/index_model.h \
    files/media_manager.h \
    files/media_model.h \
    files/network.h \
    files/network_queue.h \
    files/wgt.h \
    smilparser/timings/clock_value.h \
    smilparser/timings/timing.h \
    smilparser/tools/shuffle.h \
    smilparser/audio.h \
    smilparser/body.h \
    smilparser/base.h \
    smilparser/base_timing.h \
    smilparser/container.h \
    smilparser/excl.h \
    smilparser/factory.h \
    smilparser/head.h \
    smilparser/image.h \
    smilparser/media.h \
    smilparser/par.h \
    smilparser/prefetch.h \
    smilparser/priorityclass.h \
    smilparser/seq.h \
    smilparser/video.h \
    smilparser/web.h \
    tools/configuration.h \
    tools/logging_categories.h \
    smilparser.h \

DISTFILES += \
    reports/system.xml

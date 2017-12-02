include(../defaults.pri)

QT       += core network xml widgets
TEMPLATE  = lib
CONFIG    += warn_on staticlib c++11 stl

DEFINES += QUAZIP_STATIC
Release:DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = garlic
LIBS += -L../ext -lquazip -lzlib

!android:DESTDIR = ../lib/
android:DESTDIR = ../libandroid/

INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes


SOURCES += \
    files/base_manager.cpp \
    files/disc_space.cpp \
    files/downloader.cpp \
    files/download_queue.cpp \
    files/index_manager.cpp \
    files/index_model.cpp \
    files/media_manager.cpp \
    files/media_model.cpp \
    files/network_access.cpp \
    files/webdav.cpp \
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
    smilparser/head/subscription.cpp \
    system_infos/memory.cpp \
    system_infos/network.cpp \
    system_infos/cpu.cpp \
    system_infos/general.cpp \
    lib_facade.cpp \
    reports/create/base_reports.cpp \
    reports/create/system_report.cpp \
    reports/system_report_manager.cpp \
    files/file_downloader.cpp \
    tools/logger.cpp \
    reports/event_logs_manager.cpp \
    reports/create/event_logs.cpp \
    tools/log_file.cpp \
    reports/base_report_manager.cpp \
    reports/play_logs_manager.cpp \
    reports/create/play_logs.cpp

HEADERS += \
    files/base_manager.h \
    files/disc_space.h \
    files/downloader.h \
    files/download_queue.h \
    files/file_downloader.h \
    files/index_manager.h \
    files/index_model.h \
    files/media_manager.h \
    files/media_model.h \
    files/network_access.h \
    files/webdav.h \
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
    smilparser.h \
    smilparser/head/subscription.h \
    system_infos/memory.h \
    system_infos/network.h \
    system_infos/cpu.h \
    system_infos/general.h \
    lib_facade.h \
    reports/create/base_reports.h \
    reports/create/system_report.h \
    reports/system_report_manager.h \
    tools/configuration.h \
    tools/logging_categories.h \
    tools/logger.h \
    reports/event_logs_manager.h \
    reports/create/event_logs.h \
    tools/log_file.h \
    reports/base_report_manager.h \
    reports/play_logs_manager.h \
    reports/create/play_logs.h


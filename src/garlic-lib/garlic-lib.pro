include(../defaults.pri)

QT       += core sql network xml widgets
TEMPLATE  = lib
CONFIG    += warn_on c++11 stl

DEFINES += QUAZIP_STATIC QT_DEPRECATED_WARNINGS

TARGET = garlic
LIBS += -L../lib -lquazip -lzlib

!android:DESTDIR = ../lib/
android:DESTDIR = ../libandroid/

INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes

win32 {
    CONFIG += staticlib
    Release:LIBS += -L../lib -lquazip -lzlib
    Debug:LIBS += -L../lib  -lquazipd -lzlib
}

unix{
    #temporary ToDO
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
}

SOURCES += \
    files/base_manager.cpp \
    files/disc_space.cpp \
    files/download_factory.cpp \
    files/downloader.cpp \
    files/download_queue.cpp \
    files/index_manager.cpp \
    files/index_model.cpp \
    files/media_manager.cpp \
    files/media_model.cpp \
    files/network_access.cpp \
    files/webdav.cpp \
    files/wgt.cpp \
    smilparser/base_timings.cpp \
    smilparser/body_parser.cpp \
    smilparser/current_playing_media.cpp \
    smilparser/elements_container.cpp \
    smilparser/head/firmware_update.cpp \
    smilparser/head/task_scheduler.cpp \
    smilparser/head/xml_configuration.cpp \
    smilparser/head_parser.cpp \
    smilparser/media/audio.cpp \
    smilparser/media/base_media.cpp \
    smilparser/media/image.cpp \
    smilparser/media/prefetch.cpp \
    smilparser/media/unknown.cpp \
    smilparser/media/video.cpp \
    smilparser/media/web.cpp \
    smilparser/timings/enhanced_timer.cpp \
    smilparser/trigger/clock_value.cpp \
    smilparser/timings/simple_timer.cpp \
    smilparser/trigger/wallclock.cpp \
    smilparser/base.cpp \
    smilparser/container/tools/shuffle.cpp \
    smilparser/container/body.cpp \
    smilparser/container/container.cpp \
    smilparser/container/excl.cpp \
    smilparser/container/par.cpp \
    smilparser/container/priorityclass.cpp \
    smilparser/container/seq.cpp \
    smilparser/factory.cpp \
    tools/configuration.cpp \
    tools/logging_categories.cpp \
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
    reports/create/play_logs.cpp \
    reports/inventory_report_manager.cpp \
    db/inventory_table.cpp \
    reports/create/inventory_report.cpp \
    tools/resource_monitor.cpp

HEADERS += \
    files/base_manager.h \
    files/disc_space.h \
    files/download_factory.h \
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
    smilparser/body_parser.h \
    smilparser/current_playing_media.h \
    smilparser/elements_container.h \
    smilparser/head/firmware_update.h \
    smilparser/head/task_scheduler.h \
    smilparser/head/xml_configuration.h \
    smilparser/head_parser.h \
    smilparser/media/base_media.h \
    smilparser/media/audio.h \
    smilparser/media/image.h \
    smilparser/media/prefetch.h \
    smilparser/media/unknown.h \
    smilparser/media/video.h \
    smilparser/media/web.h \
    smilparser/timings/enhanced_timer.h \
    smilparser/trigger/clock_value.h \
    smilparser/timings/simple_timer.h \
    smilparser/trigger/wallclock.h \
    smilparser/container/tools/shuffle.h \
    smilparser/container/body.h \
    smilparser/container/par.h \
    smilparser/container/priorityclass.h \
    smilparser/container/seq.h \
    smilparser/container/container.h \
    smilparser/container/excl.h \
    smilparser/factory.h \
    smilparser/base_timings.h \
    smilparser/base.h \
    smilparser/head/subscription.h \
    system_infos/memory.h \
    system_infos/network.h \
    system_infos/cpu.h \
    system_infos/general.h \
    tools/configuration.h \
    tools/logging_categories.h \
    tools/logger.h \
    tools/log_file.h \
    tools/resource_monitor.h \
    reports/create/base_reports.h \
    reports/create/system_report.h \
    reports/system_report_manager.h \
    reports/event_logs_manager.h \
    reports/create/event_logs.h \
    reports/base_report_manager.h \
    reports/play_logs_manager.h \
    reports/create/play_logs.h \
    reports/create/inventory_report.h \
    reports/inventory_report_manager.h \
    db/inventory_table.h \
    lib_facade.h \
    version.h


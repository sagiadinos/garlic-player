#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:22:23
#
#-------------------------------------------------
include(../defaults.pri)
include(../player-common/common.pri)

QT          += core gui sql xml xmlpatterns widgets webenginewidgets
CONFIG      += warn_on c++11
DEFINES     += QT_DEPRECATED_WARNINGS
TARGET       = garlic-player
TEMPLATE     = app
DESTDIR      = ../bin
INCLUDEPATH += ../ext/zlib/includes
INCLUDEPATH += ../garlic-lib/
Release:DEFINES += QT_NO_DEBUG_OUTPUT

# defines for using different default content_url per player
# remarks:
# protocol must be seperate from url cause the double slash // cut string and no escape sequence helped
# & and must be escaped => \&
#DEFINES += DEFAULT_CONTENT_URL_NAME=SmilControl
#DEFINES += DEFAULT_CONTENT_URL_PROTOCOL=http
#DEFINES += DEFAULT_CONTENT_URL=indexes.smil-control.com/index.php?site=get_index\&owner_id=12

# which media backend should be used: support_qtav support_qtmm support_libvlc
CONFIG   += support_qtav

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += avwidgets
    CONFIG  += avwidgets
    HEADERS += \
        mm_libs/qtav_decoder.h \
        mm_libs/qtav_widget.h
    SOURCES += \
        mm_libs/qtav_decoder.cpp \
        mm_libs/qtav_widget.cpp

    # workaround for >Qt5.11 https://github.com/wang-bin/QtAV/issues/1231
    linux {
       LIBS += -L$$QT.core.libs -lQtAV
    }
    macx {
        INCLUDEPATH  += $$QT.core.libs/QtAV.framework/Versions/1/Headers
        INCLUDEPATH  += $$QT.core.libs/QtAVWidgets.framework/Versions/1/Headers
        QMAKE_LFLAGS += -F$$QT.core.libs
        LIBS         += -framework QtAV
    }
    win32 {
        LIBS += -L$$QT.core.libs -lQtAV1
    }
}

support_libvlc{
    DEFINES += SUPPORT_LIBVLC
    LIBS += -lvlc -lX11
    HEADERS  += \
        mm_libs/vlc_decoder.h \
        mm_libs/vlc_widget.h
    SOURCES += \
        mm_libs/vlc_decoder.cpp \
        mm_libs/vlc_widget.cpp
}

support_qtmm {
     DEFINES += SUPPORT_QTMM
     QT += multimedia multimediawidgets
     HEADERS  += \
        mm_libs/qtmm_decoder.h \
        mm_libs/qtmm_widget.h
    SOURCES += \
        mm_libs/qtmm_decoder.cpp \
        mm_libs/qtmm_widget.cpp
}


linux {
    LIBS += -L../lib -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
    # hide some annoying GCC compiler warnings
    # Todo check if neccessary from time to time
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-expansion-to-defined QtAV has "this use of defined may not be portable" issues with newer gcc
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy -Wno-expansion-to-defined
}

win32 {
    #libgarlic is static compiled in windows
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

macx {
    #libgarlic release is static compiled in osx
    debug:CONFIG+=sdk_no_version_check # remove annoying SDK messages on Apple Silicon

    ICON = ../../deployment/macx/garlic-player.icns
    release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    debug:LIBS += -L../lib -lgarlic
}

SOURCES += \
    media/brush.cpp \
    media/image.cpp \
    media/player_base_media.cpp \
    media/ref_command.cpp \
    media/video.cpp \
    media/web.cpp \
    media/audio.cpp \
    media/media_factory.cpp \
    main.cpp \
    mainwindow.cpp \
    mediaplayer_wrapper.cpp \
    mediawidget_wrapper.cpp \
    region.cpp \
    regions_list.cpp

HEADERS  += \
    media/brush.h \
    media/image.h \
    media/player_base_media.h \
    media/ref_command.h \
    media/video.h \
    media/web.h \
    media/audio.h \
    media/media_factory.h \
    mainwindow.h \
    mediaplayer_wrapper.h \
    mediawidget_wrapper.h \
    region.h \
    regions_list.h

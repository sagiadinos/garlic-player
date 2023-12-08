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
INCLUDEPATH += ../ext/QtWebApp/httpserver
INCLUDEPATH += ../garlic-lib/
Release:DEFINES += QT_NO_DEBUG_OUTPUT

# defines for using different default content_url per player
# remarks:
# protocol must be seperate from url cause the double slash // cut string and no escape sequence helped
# & and must be escaped => \&
#DEFINES += DEFAULT_CONTENT_URL_NAME=SmilControl
#DEFINES += DEFAULT_CONTENT_URL_PROTOCOL=http
#DEFINES += DEFAULT_CONTENT_URL=indexes.smil-control.com/index.php?site=get_index\&owner_id=12

# possible media backends:
# support_qtav              currently default but deprecated as QtAV has mem leaks and development abandoned)
# support_qtmm              Qt Multimedia lib. Slow, laggy and platform dependend
# support_qtavplayer        replacement for support_qtav
# support_qffplay           experimental tiny ffmpeg wrapper. currently without sound output
# support_libvlc            For Raspberry Pi OS only. libvlc has much better hw acceleration than ffmpeg

linux {
    contains(QMAKE_CXX, .*raspbian.*arm.*):{
        CONFIG   += support_libvlc
    } else {
        CONFIG   += support_qtavplayer
    }
}

win32 {
    CONFIG   += support_qtavplayer
}

macx {
    CONFIG   += support_qtavplayer
}

support_qtavplayer {
    DEFINES += SUPPORT_QTAVPLAYER
    DEFINES+="QT_AVPLAYER_MULTIMEDIA"
    INCLUDEPATH += ../ext/
    include(../ext/QtAVPlayer/QtAVPlayer.pri)

    QT += multimedia multimediawidgets
    HEADERS  += \
        mm_libs/qtavplayer_decoder.h \
        mm_libs/qtavplayer_widget.h
    SOURCES += \
        mm_libs/qtavplayer_decoder.cpp \
        mm_libs/qtavplayer_widget.cpp
    win32 {
        # include ffmpeg  Use your own directories
        # alternatively you can put it in Qt paths.
        INCLUDEPATH += "C:\BuildEssentials\ffmpeg\include"
        LIBS += -LC:\BuildEssentials\ffmpeg\lib
        LIBS += -LC:\BuildEssentials\ffmpeg\bin

        LIBS += -L$$QT.core.libs
    }
    macx {
        # because someone is to silly to find ffmpeg includes even in Qt Dir
        # this is only for the build server. Change it to your values
        INCLUDEPATH += /Users/niko/Deployment/ffmpeg-6.1/include
        LIBS += -L/Users/niko/Deployment/ffmpeg-6.1/lib
    }
}

support_qffplay {
    DEFINES += SUPPORT_QFFPLAY
    HEADERS += \
        mm_libs/qffpl_decoder.h \
        mm_libs/qffpl_widget.h \
        mm_libs/qffplaylib/decoder.h \
        mm_libs/qffplaylib/mediaplayer.h \
        mm_libs/qffplaylib/ffmpeg.h \
        mm_libs/qffplaylib/videoout.h
    SOURCES += \
        mm_libs/qffpl_decoder.cpp \
        mm_libs/qffpl_widget.cpp \
        mm_libs/qffplaylib/decoder.cpp \
        mm_libs/qffplaylib/ffmpeg.cpp \
        mm_libs/qffplaylib/mediaplayer.cpp \
        mm_libs/qffplaylib/videoout.cpp

    linux {
       LIBS += -lavformat -lavdevice -lavcodec -lswresample -lswscale -lavutil
    }
    macx {
        # because someone is to silly to find ffmpeg includes in Qt Dir
        INCLUDEPATH += /Users/niko/Qt/5.12.12/clang_64/include
        LIBS += -L$$QT.core.libs -lavformat -lavdevice -lavcodec -lswresample -lswscale -lavutil
    }
    win32 {
        LIBS += -L$$QT.core.libs
    }
}

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
    linux {
        LIBS += -lvlc -lX11
    }
    macx {
        INCLUDEPATH  += $$QT.core.libs/vlcpp
        QMAKE_LFLAGS += -F$$QT.core.libs/vlcpp
        QMAKE_LFLAGS += -F$$QT.core.libs/vlcpp/plugins
        LIBS += -L$$QT.core.libs/vlcpp/plugins
        LIBS += -L$$QT.core.libs/vlcpp -lvlc -lvlccore
    }
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
    QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
    debug:CONFIG+=sdk_no_version_check # remove annoying SDK messages on Apple Silicon
    ICON = ../../deployment/macx/garlic-player.icns

    #libgarlic release is static compiled in osx
    release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    debug:LIBS += -L../lib -lgarlic -lquazip -lzlib
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

include(../defaults.pri)

QT          += core sql widgets gui xml quick qml quickcontrols2
CONFIG      += warn_on c++11
TARGET       = garlic-player
TEMPLATE     = app
DESTDIR      = ../bin
INCLUDEPATH +=../ext/zlib/includes
INCLUDEPATH +=../ext/quazip/includes
INCLUDEPATH += ../garlic-lib/
DEFINES     += QT_DEPRECATED_WARNINGS

Release:DEFINES += QT_NO_DEBUG_OUTPUT

#comment out the next line when you want to use QTMultimedia instead of QtAV
#CONFIG  += support_qtav

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += av
}

unix:!android {
    QT += webengine
    LIBS += -L../lib -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
}

android {
    QT        += androidextras webview
    LIBS      += -L../libandroid -lgarlic
    DISTFILES += \
       AndroidManifest.xml \
      res/values/libs.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD
    ANDROID_LIBS = $$dirname(QMAKE_QMAKE)/../lib

    ANDROID_EXTRA_LIBS += \
     $$OUT_PWD/../libandroid/libgarlic.so \

#    include(../ext/android_openssl/openssl.pri)
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

include(../player-common/common.pri)

SOURCES += \
    media/image.cpp \
    media/player_base_media.cpp \
    media/video.cpp \
    media/web.cpp \
    media/audio.cpp \
    media/media_factory.cpp \
    main.cpp \
    mainwindow.cpp \
    region.cpp

HEADERS  += \
    media/image.h \
    media/player_base_media.h \
    media/video.h \
    media/web.h \
    media/audio.h \
    media/media_factory.h \
    mainwindow.h \
    region.h
RESOURCES += qml.qrc



-------------------------------------------------
#
# Project created by QtCreator 2017-09-09T19:59:00
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../garlic-lib/

QT     += core sql widgets gui xml quick qml quickcontrols2
CONFIG += warn_on c++11
Release:DEFINES += QT_NO_DEBUG_OUTPUT

#comment out the next line when you want to use QTMultimedia instead of QtAV
#CONFIG  += support_qtav
#comment in to compile with RPI 3
#DEFINES += SUPPORT_RPI

support_qtav {
    DEFINES += SUPPORT_QTAV
    QT      += av
}

DEFINES += QT_DEPRECATED_WARNINGS QUAZIP_STATIC

TARGET = garlic-player
TEMPLATE = app
DESTDIR = ../bin
INCLUDEPATH+=../ext/zlib/includes
INCLUDEPATH+=../ext/quazip/includes

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

unix:!android {
    QT += webengine
    LIBS += -L../lib -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
}
android {
    QT += androidextras webview
    LIBS += -L../libandroid -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

RESOURCES += qml.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    AndroidManifest.xml \
    gradle/wrapper/gradle-wrapper.jar \
    gradlew \
    res/values/libs.xml \
    build.gradle \
    gradle/wrapper/gradle-wrapper.properties \
    gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD

android {
    ANDROID_LIBS = $$dirname(QMAKE_QMAKE)/../lib

    ANDROID_EXTRA_LIBS += \
        $$ANDROID_LIBS/libssl.so \
        $$ANDROID_LIBS/libcrypto.so
}

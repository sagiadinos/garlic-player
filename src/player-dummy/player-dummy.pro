#-------------------------------------------------
#
# Project created by QtCreator 2017-10-18T20:59:33
#
#-------------------------------------------------
include(../defaults.pri)
INCLUDEPATH += ../garlic-lib/
INCLUDEPATH += ../garlic-lib/smilparser

QT     += core gui xml widgets network
CONFIG += warn_on c++11
Release:DEFINES += QT_NO_DEBUG_OUTPUT

#Delete or comment out the next line when you want to use QTMultimedia
#CONFIG    += support_qtav

#support_qtav {
#    DEFINES += SUPPORT_QTAV
#    QT      += avwidgets
#} else {
#    QT += multimedia multimediawidgets
#}

DEFINES += QUAZIP_STATIC

TARGET = player-dummy
TEMPLATE = app
DESTDIR = ../bin
INCLUDEPATH+=../ext/zlib/includes

include(../player-common/common.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS +=

unix:!android {
    QT += webengine
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
android {
    QT += androidextras
    LIBS += -L../libandroid -lgarlic -lquazip -lzlib
}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

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

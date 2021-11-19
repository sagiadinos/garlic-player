include(../defaults.pri)

QT          += core sql widgets gui xml xmlpatterns quick qml quickcontrols2
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

macx {
    QT += webview
    #libgarlic is static compiled in osx
    ICON = ../../deployment/macx/garlic-player.icns
    LIBS += -L../lib -lgarlic -lquazip -lzlib
}
ios {
    QT += webview
    LIBS += -L../libios -lgarlic -lquazip -lzlib
   # XCODEBUILD_FLAGS += -allowProvisioningUpdates

  #  QMAKE_INFO_PLIST = path/to/your/Info.plist
  #  QMAKE_TARGET_BUNDLE_PREFIX = com.smilcontrol
   QMAKE_BUNDLE = garlic-player
    QMAKE_IOS_DEPLOYMENT_TARGET = 12.0
  #  QMAKE_APPLE_TARGETED_DEVICE_FAMILY = 1,2
}

linux:!android {
    QT += webview
    LIBS += -L../lib -lgarlic #quazip and lzlib are compiled as static libs into libgarlic.so
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
}

android {
    QT        += androidextras webview
    LIBS      += -L../libandroid -lgarlic
    DISTFILES += \
       android_brandings/GarlicPlayer/android/AndroidManifest.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android_brandings/GarlicPlayer/android/

    # release build crashs when start debugging
    # see https://github.com/KDAB/android_openssl/issues/10
    include(../ext/android_openssl/openssl.pri)

    ANDROID_EXTRA_LIBS += \
        $$OUT_PWD/../libandroid/libgarlic.so

    HEADERS  += \
        Java2Cpp.h \
        android_manager.h
    SOURCES += android_manager.cpp

}
win32 {
    Release:LIBS += -L../lib -lgarlic -lquazip -lzlib
    Debug:LIBS += -L../lib -lgarlic -lquazipd -lzlib
}

include(../player-common/common.pri)

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
    region.h \
    regions_list.h
RESOURCES += qml.qrc

DISTFILES += \
    android/src/com/sagiadinos/garlic/player/java/ConfigReceiver.java \
    android/src/com/sagiadinos/garlic/player/java/GarlicActivity.java \
    android/src/com/sagiadinos/garlic/player/java/GarlicLauncher.java \
    android/src/com/sagiadinos/garlic/player/java/LauncherInterface.java \
    android/src/com/sagiadinos/garlic/player/java/PhilipsLauncher.java \
    android/src/com/sagiadinos/garlic/player/java/SICPDef.java \
    android/src/com/sagiadinos/garlic/player/java/SmilIndexReceiver.java \
    android/src/com/sagiadinos/garlic/player/java/SocketClient.java

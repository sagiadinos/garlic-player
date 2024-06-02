TEMPLATE = lib
CONFIG += qt warn_on c++17 staticlib
QT -= gui
Release:DEFINES += QT_NO_DEBUG_OUTPUT

linux:!android {
    #temporary ToDO
    # -Wno-deprecated-copy is against the warnings floading with gcc 9 and Qt < 5.13
    # -Wno-deprecated-declarations is against the warnings floading with gcc 9 and Qt < 5.13
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-deprecated-copy
    DESTDIR = ../../lib/
}
win32 {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    target.path=$$PREFIX/lib
    INSTALLS += headers target
    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX
    DESTDIR = ../../lib/
}
macx{
    DESTDIR = ../../lib/
    QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
}

android {
     DESTDIR = ../../libandroid/

    equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 14) {
        TARGET = quazip
    }
    else {
        ANDROID_ABIS=armeabi-v7a x86 x86_64 arm64-v8a
        equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
             TARGET = quazip_armeabi-v7a
         }
         equals(ANDROID_TARGET_ARCH, arm64-v8a) {
             TARGET = quazip_arm64-v8a
         }
         equals(ANDROID_TARGET_ARCH, x86_64) {
            TARGET = quazip_x86_64
         }
         equals(ANDROID_TARGET_ARCH, x86) {
             TARGET = quazip_x86
         }
     }
}
ios:DESTDIR = ../../libios/
INCLUDEPATH += ../zlib/includes
LIBS += -L../../lib/ -lzlib

# Creating pkgconfig .pc file
#CONFIG += create_prl no_install_prl create_pc

QMAKE_PKGCONFIG_PREFIX = $$PREFIX
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_REQUIRES = Qt5Core

# The ABI version.

!win32:VERSION = 1.0.0

# 1.0.0 is the first stable ABI.
# The next binary incompatible change will be 2.0.0 and so on.
# The existing QuaZIP policy on changing ABI requires to bump the
# major version of QuaZIP itself as well. Note that there may be
# other reasons for chaging the major version of QuaZIP, so
# in case where there is a QuaZIP major version bump but no ABI change,
# the VERSION variable will stay the same.

# For example:

# QuaZIP 1.0 is released after some 0.x, keeping binary compatibility.
# VERSION stays 1.0.0.
# Then some binary incompatible change is introduced. QuaZIP goes up to
# 2.0, VERSION to 2.0.0.
# And so on.

greaterThan(QT_MAJOR_VERSION, 4) {
    # disable all the Qt APIs deprecated before Qt 6.0.0
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
}

# This one handles dllimport/dllexport directives.
DEFINES += QUAZIP_BUILD
DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += QT_NO_CAST_TO_ASCII
# You'll need to define this one manually if using a build system other
# than qmake or using QuaZIP sources directly in your project.
CONFIG(staticlib): DEFINES += QUAZIP_STATIC

# Input
include(quazip.pri)


CONFIG(debug, debug|release) {
#     mac: TARGET = $$join(TARGET,,,_debug)
     win32: TARGET = $$join(TARGET,,,d)
}

unix {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    target.path=$$PREFIX/lib/$${LIB_ARCH}
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
    INSTALLS += headers target

	OBJECTS_DIR=.obj
	MOC_DIR=.moc
	
}

win32 {
    headers.path=$$PREFIX/include/quazip
    headers.files=$$HEADERS
    INSTALLS += headers target
    CONFIG(staticlib){
        target.path=$$PREFIX/lib
        QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/lib/
    } else {
        target.path=$$PREFIX/bin
        QMAKE_PKGCONFIG_LIBDIR = $$PREFIX/bin/
    }

    ## odd, this path seems to be relative to the
    ## target.path, so if we install the .dll into
    ## the 'bin' dir, the .pc will go there as well,
    ## unless have hack the needed path...
    ## TODO any nicer solution?
    QMAKE_PKGCONFIG_DESTDIR = ../lib/pkgconfig
    # workaround for qdatetime.h macro bug
    DEFINES += NOMINMAX
}

#!/bin/bash

set -e

SCRIPTDIR=$(dirname "$0")
source $SCRIPTDIR/env.sh

BASE_PATH=/home/niko/Anwendungen            # Your full basic path to your develop tools do not use ~/ 

QT_PATH_RUNTIME=$QT_BASE_PATH/$QT_VERSION/android_armv7

 
BUILD_DIR=android_armv7-$SHADOW_BUILD_DIR
BUILD_TARGET=android-build

export ANDROID_SDK_ROOT=$BASE_PATH/Android/Sdk
export ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/ndk/20.1.5948944 # export to overwrite
export ANDROID_API_VERSION=android-19  


# build runs without but not know if neccesary
export ANDROID_HOME=$BASE_PATH/Android 
# not neccessary with clang
#export ANDROID_NDK_TOOLCHAIN_PREFIX=llvm
#export ANDROID_NDK_TOOLCHAIN_VERSION=
#export ANDROID_NDK_HOST=linux-x86_64  
#export ANDROID_NDK_PLATFORM=android-20

export JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64

echo 
echo ========== enter shadowbuild dir 
echo 
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo 
echo ========== prepare build 
echo 
$QT_PATH_RUNTIME/bin/qmake -r -spec android-clang  $GARLIC_DIR/src/complete_c2qml.pro CONFIG+=$CONFIG_DEBUG_RELEASE CONFIG+=qml_$CONFIG_DEBUG_RELEASE

echo 
echo ========== build 
echo 
make -j $DEV_JOBS --silent

echo 
echo ========== pack apk with gradle 
echo 
make INSTALL_ROOT=$BUILD_TARGET install
$QT_PATH_RUNTIME/bin/qmake -install qinstall -exe bin/libgarlic-player.so $BUILD_TARGET/libs/armeabi-v7a/libgarlic-player.so

$QT_PATH_RUNTIME/bin/androiddeployqt \
	--input player-c2qml/android-libgarlic-player.so-deployment-settings.json \
	--output $BUILD_TARGET \
	--deployment bundled \
	--android-platform $ANDROID_API_VERSION \
	--jdk $JDK_PATH  \
	--gradle 

mv android-build/build/outputs/apk/android-build-debug.apk garlic-player-android-armv7-$DATE.apk

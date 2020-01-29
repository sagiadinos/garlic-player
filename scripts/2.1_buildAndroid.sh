#!/bin/bash

set -e

SCRIPTDIR=$(dirname "$0")
source $SCRIPTDIR/env.sh

# =====================================================

export ANDROID_ARCH="NEED TO CUSTOMIZED"									# Qt 5.12 support  android_armv7 android_arm64_v8a android_x86
export ANDROID_SDK_ROOT="NEED TO CUSTOMIZED" 					 	# Your full basic path to your Android SDK tools do not use ~/ 
export ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/"NEED TO CUSTOMIZED"  	# path to Google NDK
export ANDROID_API_VERSION=android-19  								# e.g. 19 is for Android 4.4 
export QT_MKSPEC=android-clang  								    # android-clang or android-g++

# not neccessary with clang uncommnt if you build for Qt < 5.12
#export ANDROID_NDK_TOOLCHAIN_PREFIX=arm-linux-androideabi  
#export ANDROID_NDK_TOOLCHAIN_VERSION=4.9  
#export ANDROID_NDK_HOST=linux-x86_64  
#export ANDROID_NDK_PLATFORM=android-10  					

export JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64

# =====================================================

QT_PATH_RUNTIME=$QT_BASE_PATH/$QT_VERSION/$ANDROID_ARCH
BUILD_DIR=$ANDROID_ARCH-$SHADOW_BUILD_DIR
BUILD_TARGET=android-build

echo 
echo ========== enter shadowbuild dir 
echo 
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo 
echo ========== prepare build 
echo 
$QT_PATH_RUNTIME/bin/qmake -r -spec $QT_MKSPEC  $GARLIC_DIR/src/complete_c2qml.pro CONFIG+=$CONFIG_DEBUG_RELEASE CONFIG+=qml_$CONFIG_DEBUG_RELEASE

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

mv android-build/build/outputs/apk/debug/android-build-debug.apk garlic-player-$ANDROID_ARCH-$DEPLOY_SUFFIX.apk

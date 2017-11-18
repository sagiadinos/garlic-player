#!/bin/bash

# =============================================== 
# you had to configure before starting
# ===============================================

export BASE_PATH=                                   # Your full basic path to the develop tools in your user directory. Do not use ~/ 

export QT_VERSION=			         			    # The Qt Version 5.7, 5.8, 5.9.2 etc
export QT_API_VERSION=                              # The api version gcc_64 android_armv7 etc
export QT_BASE_PATH=$BASE_PATH/Qt                   # path to your qt base directory

export CONFIG_DEBUG_RELEASE=	                    # set if debug or release

export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)
export BUILD_DIR=								 	# set your build directory
export BUILD_TARGET=$BUILD_DIR/android-build

export ANDROID_BASE_PATH=$BASE_PATH  				# Set your Android developer Toll Bbase directory eg $BASE_PATH/Android

export ANDROID_SDK_ROOT=$ANDROID_BASE_PATH/SDK  
export ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/ndk-bundle
export ANDROID_API_VERSION=android-26  

# build runs without, but not know if neccesary somewhere 
export ANDROID_HOME=$ANDROID_BASE_PATH/SDK  
export ANDROID_NDK_TOOLCHAIN_PREFIX=arm-linux-androideabi  
export ANDROID_NDK_TOOLCHAIN_VERSION=4.9  
export ANDROID_NDK_HOST=linux-x86_64  
export ANDROID_NDK_PLATFORM=android-16  

export JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64  # path to JDK

# =============================================== 
# start build
# ===============================================

mkdir -p $BUILD_DIR
cd $BUILD_DIR

# build...
$QT_BASE_PATH/$QT_VERSION/$QT_API_VERSION/bin/qmake -r -spec android-g++  ../src/complete_c2qml.pro CONFIG+=$CONFIG_DEBUG_RELEASE CONFIG+=qml_$CONFIG_DEBUG_RELEASE
make -j $DEV_JOBS --silent

# pack apk with gradle
make INSTALL_ROOT=$BUILD_TARGET install
$QT_BASE_PATH/$QT_VERSION/$QT_API_VERSION/bin/qmake -install qinstall -exe $BUILD_DIR/bin/libgarlic-player.so $BUILD_TARGET/libs/armeabi-v7a/libgarlic-player.so

$QT_BASE_PATH/$QT_VERSION/$QT_API_VERSION/bin/androiddeployqt \
	--input $BUILD_DIR/player-c2qml/android-libgarlic-player.so-deployment-settings.json \
	--output $BUILD_TARGET \
	--deployment bundled \
	--android-platform $ANDROID_API_VERSION \
	--jdk $JDK_PATH  \
	--gradle

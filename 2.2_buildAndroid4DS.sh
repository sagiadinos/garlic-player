#!/bin/bash

set -e

SCRIPTDIR=$(dirname "$0")
source $SCRIPTDIR/env.sh

# =====================================================

export ANDROID_ARCH="android_armv7"							# Qt >= 5.12 supports android_armv7 android_arm64_v8a android_x86
export ANDROID_SDK_ROOT="/home/niko/Anwendungen/Android/Sdk/" 					 	# Your full basic path to your Android SDK tools do not use ~/ 
export ANDROID_NDK_ROOT="/home/niko/Anwendungen/Android/Sdk/ndk/21.3.6528147/"  	# path to Google NDK
export ANDROID_API_VERSION=android-19  								# e.g. 19 is for Android 4.4 
export QT_MKSPEC=android-clang  								    # android-clang or android-g+

# not neccessary with clang uncommnt if you build for Qt < 5.12
#export ANDROID_NDK_TOOLCHAIN_PREFIX=arm-linux-androideabi  
#export ANDROID_NDK_TOOLCHAIN_VERSION=4.9  
#export ANDROID_NDK_HOST=linux-x86_64  
#export ANDROID_NDK_PLATFORM=android-10  					

export JDK_PATH=/usr/lib/jvm/java-8-openjdk
export JAVA_HOME=$JDK_PATH 											# neccessary when there is more than one sdk 

# =====================================================

QT_PATH_RUNTIME=$QT_BASE_PATH/$QT_VERSION/$ANDROID_ARCH
BUILD_DIR=ds-player_android
BUILD_TARGET=android-build

echo 
echo ========== enter shadowbuild dir 
echo 
mkdir -p $BUILD_DIR
cd $BUILD_DIR

echo 
echo ========== build libs
echo 
$QT_PATH_RUNTIME/bin/qmake -r -spec $QT_MKSPEC  $GARLIC_DIR/src/libs_only.pro CONFIG+=$CONFIG_DEBUG_RELEASE CONFIG+=qml_$CONFIG_DEBUG_RELEASE
make -j $DEV_JOBS --silent


mkdir -p ds-player
cd ds-player

echo 
echo ========== build ds-player
echo 
$QT_PATH_RUNTIME/bin/qmake -r -spec $QT_MKSPEC $GARLIC_DIR/src/player-c2qml/ds-player.pro CONFIG+=$CONFIG_DEBUG_RELEASE CONFIG+=qml_$CONFIG_DEBUG_RELEASEs
make -j $DEV_JOBS --silent

cd ..

echo 
echo ========== pack apk with gradle 
echo 
make INSTALL_ROOT=$BUILD_TARGET install
$QT_PATH_RUNTIME/bin/qmake -install qinstall -exe bin/libgarlic-player.so $BUILD_TARGET/libs/armeabi-v7a/libgarlic-player.so

KEYFILE=/home/niko/keys/garlic-launcher-key.jks
STORE_PASSWORD=gahd5PhiaChio8haien9ie0IuFur5Eel
KEY_ALIAS=key0
KEY_PASSWORD=oeru5LoEiz8gae3ailoo8IeRaeg2eer

echo 
echo ========== deploy 
echo 

$QT_PATH_RUNTIME/bin/androiddeployqt \
	--input ds-player/android-libgarlic-player.so-deployment-settings.json \
	--output $BUILD_TARGET \
	--deployment bundled \
	--android-platform $ANDROID_API_VERSION \
	--jdk $JDK_PATH  \
	--gradle \
	--release \
	--sign $KEYFILE $KEY_ALIAS --storepass $STORE_PASSWORD --keypass $KEY_PASSWORD
	
mv android-build/build/outputs/apk/release/android-build-release-signed.apk ../ds-player-$ANDROID_ARCH-$DEPLOY_SUFFIX.apk



#curl -u niko:117578d97df7bdab4e361e737c977288ef http://wbuildgarlic.smil-admin.com:8080/job/garlic-player_windows/build?token=se1uJuezOc9eec1doDohphu5iCh6Aineid0Enei1Do5aeh8uZe9nuGeeohie3Ier

#!/bin/bash

set -e

SCRIPTDIR=$(dirname "$0")
FULL_SCRIPTDIR=$(pwd)
source $SCRIPTDIR/../env.sh

# =====================================================

export ANDROID_SDK_ROOT=/home/niko/Anwendungen/Android/Sdk 			# Your full basic path to your Android SDK tools do not use ~/
export ANDROID_NDK_ROOT=$ANDROID_SDK_ROOT/ndk/21.1.6352462			# path to Google NDK
export ANDROID_NDK_PLATFORM=android-29                              # Which platform android-29 is max for 21.1.6352462


export JDK_PATH=/usr/lib/jvm/java-8-openjdk-amd64
export JAVA_HOME=$JDK_PATH 											# neccessary when there is more than one sdk
export PATH=$JAVA_HOME/bin:$PATH

QT_INSTALL_DIR=$QT_BASE_PATH/$QT_VERSION/android

# =====================================================


echo
echo ========== enter shadowbuild dir
echo
mkdir -p $SHADOW_BUILD_DIR
cd $SHADOW_BUILD_DIR


echo
echo "== Patch Qt Source =="
echo

# Build.VERSION_CODES.Q throw in some cases a compile errow even if it is build with SDK 29
# uncomment following if this happens
#EXTRACTSTYLE_PATH=$QT_SRC/qtbase/src/android/jar/src/org/qtproject/qt5/android
#if [ $(grep -c "Build.VERSION_CODES.Q" $EXTRACTSTYLE_PATH/ExtractStyle.java) -gt 0 ]; then
#	echo
#	echo  "== Patch ExtractStyle =="
#	echo
#	cp -af $EXTRACTSTYLE_PATH/ExtractStyle.java $EXTRACTSTYLE_PATH/ExtractStyle.java_bak
#	patch $EXTRACTSTYLE_PATH/ExtractStyle.java < $FULL_SCRIPTDIR/patches/extractstyle.patch
#fi

# A digital signage player requiere some webview features Qt set to false like autoplay video, correct scaling etc.
# This activates it
WEBVIEW_PATH=$QT_SRC/qtwebview/src/jar/src/org/qtproject/qt5/android/view/
if [[ $(grep -L "webSettings.setMediaPlaybackRequiresUserGesture(false)" $WEBVIEW_PATH/QtAndroidWebViewController.java) ]]; then
	echo
	echo  "== Patch WebView =="
	echo
	cp -af $WEBVIEW_PATH/QtAndroidWebViewController.java $WEBVIEW_PATH/QtAndroidWebViewController.java_bak
	patch $WEBVIEW_PATH/QtAndroidWebViewController.java < $FULL_SCRIPTDIR/patches/webview.patch
fi

echo
echo "== Configuring qtbase =="
echo

#export OPENSSL_LIBS="-lssl -lcrypto"

# uncomemnt this if you want to set your Linux system to maximum performance
#echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

$QT_SRC/configure -silent -c++std c++17 -opensource -confirm-license -nomake tests -nomake examples \
    -xplatform android-clang  \
    -ssl -openssl-runtime -I "/home/niko/Source/github/android_openssl/static/include" \
    -prefix $QT_INSTALL_DIR \
    -android-ndk-platform $ANDROID_NDK_ROOT \
    -android-sdk $ANDROID_SDK_ROOT \
    -android-ndk $ANDROID_NDK_ROOT \
    -skip qttranslations -skip qtserialport -skip qtwebengine -skip qtpurchasing -skip qtvirtualkeyboard \
    -no-warnings-are-errors -disable-rpath

confirm "Continue building?"

echo
echo "== Building qt =="
echo


# maybe make -j$DEV_JOBS works for you
# parallel jobs on two different Ryzen 5950X machines breaks randomly the build process
# So I use -j16

make -j16
if [ $? -eq 0 ] ; then
	echo
	echo "== Installing qt =="
	echo
	make install
else
	# do not install when process failed
	echo
	echo "== The make process failed =="
	echo
	exit 1
fi

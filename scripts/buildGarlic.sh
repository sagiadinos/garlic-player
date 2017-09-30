#!/bin/bash

#Set correct paths to your Qt runtime

export QT_VERSION=						# The Qt Version 5.7, 5.8, 5.9.2 etc
export API_VERSION=                     # The api version gcc_64 android_armv7 etc
export QT_BASE_PATH=                    # path to your qt base directory

export CONFIG_DEBUG_RELEASE=release     # set if debug or release

export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)
if [ -z "QT_BASE_PATH" ]; then
	echo Error: Set the correct paths in QT_PATH_RUNTIME 
	exit 1;
fi

echo 
echo ========== create player ========== 
echo 
mkdir -p build-Linux-$QT_VERSION-$API_VERSION
cd build-Linux-$QT_VERSION-$API_VERSION

echo build garlic player...
$QT_BASE_PATH/$QT_VERSION/$API_VERSION/bin/qmake ../src/complete.pro "CONFIG+=$CONFIG_DEBUG_RELEASE"
make -j $DEV_JOBS --silent

# check if binary was created 
if [ ! -f bin/garlic-player ]; then
	echo Binary was not created!
    exit 1;
fi


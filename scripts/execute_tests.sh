#!/bin/bash

# =============================================== 
# config your paths
# ===============================================

export QT_VERSION=								# The Qt Version 5.7, 5.8, 5.9.2 etc
export QT_API_VERSION=							# The api version gcc_64 android_armv7 etc
export QT_BASE_PATH=							# path to your qt base directory

QT_PATH=$QT_BASE_PATH/$QT_VERSION/$QT_API_VERSION
if [ -z "$QT_BASE_PATH" ]; then
	echo Error: Set the correct paths
	exit 1;
fi

# =============================================== 
# create garlic lib first
# ===============================================

mkdir -p execute_tests
cd execute_tests

$QT_PATH/bin/qmake ../src/complete.pro "CONFIG+=release"
make -j $DEV_JOBS --silent

# check if binary was created 
if [ ! -f bin/garlic-player ]; then
	echo Binary was not created!
    exit 1;
fi

# =============================================== 
# execute tests
# ===============================================

export LD_LIBRARY_PATH=./lib;

$QT_PATH/bin/qmake ../tests/testsuite.pro -r -spec linux-g++

make -j1 --silent check TESTARGS="-silent"

exit 0;

#!/bin/bash

# =============================================== 
# config your paths
# ===============================================

export QT_VERSION=5.9.6						     # The Qt Version 5.7, 5.8, 5.9.2 etc
export QT_API_VERSION=gcc_64                     # The api version gcc_64 android_armv7 etc
export QT_BASE_PATH=/home/niko/Anwendungen/Qt    # path to your qt base directory

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

export CCACHE_DISABLE=1
export CCACHE_RECACHE=1
#export GCOV_PREFIX=./garlic-lib/
#export GCOV_PREFIX_STRIP

$QT_PATH/bin/qmake ../src/libs_only.pro "CONFIG+=debug gcov"
make -j $DEV_JOBS --silent

# =============================================== 
# execute tests
# ===============================================


$QT_PATH/bin/qmake ../tests/testsuite.pro -r -spec linux-g++ "CONFIG+=debug gcov"

make -j1 --silent check TESTARGS="-silent"

exit 0;

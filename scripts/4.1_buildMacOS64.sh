#!/bin/bash

set -e
SCRIPTDIR=$(dirname "$0")
source $SCRIPTDIR/env.sh

export QT_PATH_RUNTIME=$QT_BASE_PATH/$QT_VERSION/clang_64
export SHADOW_BUILD_DIR="clang_64-$SHADOW_BUILD_DIR"
export TEST_DIR=tests

echo 
echo ========== enter shadowbuild dir 
echo 
mkdir -p $SHADOW_BUILD_DIR
cd $SHADOW_BUILD_DIR

echo 
echo ========== prepare build 
echo 
$QT_PATH_RUNTIME/bin/qmake $GARLIC_DIR/src/complete.pro "CONFIG+=$CONFIG_DEBUG_RELEASE"

echo 
echo ========== build with $DEV_JOBS dev jobs
echo 
make -j $DEV_JOBS --silent

# check if binary was created 
if [ ! -f bin/garlic-player.app/Contents/MacOS/garlic-player ]; then
	echo Binary was not created!
    exit 1;
fi

echo 
echo ========== copy QtAVWidgets.framework which is forgotten by macdeployqt
echo

mkdir -p bin/garlic-player.app/Contents/Frameworks
cp -R $QT_PATH_RUNTIME/lib/QtAVWidgets.framework bin/garlic-player.app/Contents/Frameworks

echo 
echo ========== deploy dmg
echo

$QT_PATH_RUNTIME/bin/macdeployqt bin/garlic-player.app -dmg
mv bin/garlic-player.dmg bin/garlic-player-$DEPLOY_SUFFIX.dmg

exit 0;

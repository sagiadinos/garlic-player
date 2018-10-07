#!/bin/bash

set -e
SCRIPTDIR=$(dirname "$0")

export QT_BASE_PATH=											# path to your qt base directory
export QT_VERSION=												# The Qt Version 5.7, 5.8, 5.9.2 etc
export CONFIG_DEBUG_RELEASE=release    							# set if debug or release
export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)			# determine how many cores can be used

if [ -z "QT_BASE_PATH" ]; then
	echo Error: Set the correct paths in QT_BASE_PATH 
	exit 1;
fi

# build directory  name
export SHADOW_BUILD_DIR=build-$QT_VERSION-$CONFIG_DEBUG_RELEASE
DATE=($(date +"%Y-%m-%d"))

# $BUILD_NUMBER indicates jenkins ci
if [ -z "$BUILD_NUMBER" ]; then
	export GARLIC_DIR=
	export DEPLOY_BASE_NAME=garlic-player-$DATE

else
	export GARLIC_DIR=..
	export DEPLOY_BASE_NAME=garlic-player-$DATE-build-$BUILD_NUMBER
	export SHADOW_BUILD_DIR=$SHADOW_BUILD_DIR-$BUILD_NUMBER
fi

# create diretory for deployed files
export PACKAGE_DIR=../packages
mkdir -p packages

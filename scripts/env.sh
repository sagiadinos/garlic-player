#!/bin/bash

set -e
SCRIPTDIR=$(dirname "$0")

export QT_BASE_PATH="NEED TO CUSTOMIZED"						# path to your qt base directory
export QT_VERSION=5.12.6										# The Qt Version 5.7, 5.8, 5.9.2 etc
export CONFIG_DEBUG_RELEASE=release    							# set if debug or release
export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)			# determine how many cores can be used

if [ -z "QT_BASE_PATH" ]; then
	echo Error: Set the correct paths in QT_BASE_PATH 
	exit 1;
fi
##################################################################

# create directory name
export SHADOW_BUILD_DIR=build-$QT_VERSION-$CONFIG_DEBUG_RELEASE

# check if called from jenkins use $BUILD number else the Date
if [ -z "$BUILD_NUMBER" ]; then
	export GARLIC_DIR="NEED TO CUSTOMIZED"
	export DEPLOY_SUFFIX=($(date +"%Y-%m-%d"))
else
	export GARLIC_DIR=$PWD
	export DEPLOY_SUFFIX=build-$BUILD_NUMBER
fi
# create diretory for deployed files
export PACKAGE_DIR=../packages
mkdir -p packages

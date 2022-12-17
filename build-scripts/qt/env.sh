#!/bin/bash

set -e
SCRIPTDIR=$(dirname "$0")


function confirm {
    read -p "$1 [y/n] " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]
    then
        exit 1
    fi
}


export QT_BASE_PATH=/home/niko/Anwendungen/Qt						# path to your qt base directory
export QT_VERSION=5.15.7										# The Qt Version 5.7, 5.8, 5.9.2 etc
export CONFIG_DEBUG_RELEASE=release    							# set if debug or release
export QT_SRC=$QT_BASE_PATH/$QT_VERSION/Src                     # path to the Qt source code

export DEV_JOBS=$(grep -c "^processor" /proc/cpuinfo)			# determine how many cores can be used

if [ -z "QT_BASE_PATH" ]; then
	echo Error: Set the correct paths in QT_BASE_PATH 
	exit 1;
fi
##################################################################

# create directory name
export SHADOW_BUILD_DIR=build-$QT_VERSION


#!/bin/bash

set -e

export FFMPEG_ROOT=/usr/local/Cellar/ffmpeg/4.3.1_7
export LIBRARY_PATH=$FFMPEG_ROOT/lib:$LIBRARY_PATH
export CPLUS_INCLUDE_PATH=$FFMPEG_ROOT/include:$CPLUS_INCLUDE_PATH

mkdir -p build-QtAVPlayer
cd build-QtAVPlayer

/Applications/Qt/5.12.12/clang_64/bin//qmake /Users/niko/Source/QtAVPlayer/QtAVPlayer.pro

make -j8

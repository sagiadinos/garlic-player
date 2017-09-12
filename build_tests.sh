#!/bin/bash

# need to compile parser, quazip and libzip first

QT_PATH=""
if [ -z "$QT_PATH" ]; then
	echo Error: Set the correct paths in QT_PATH 
	exit 1;
fi

mkdir -p buildTests
cd buildTests

export LD_LIBRARY_PATH=../../src/lib;

$QT_PATH/bin/qmake ../tests/testsuite.pro -r -spec linux-g++

make -j1 check TESTARGS="-silent"

exit 0;

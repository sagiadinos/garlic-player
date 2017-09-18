#!/bin/bash

QT_PATH=
if [ -z "$QT_PATH" ]; then
	echo Error: Set the correct paths in QT_PATH 
	exit 1;
fi

mkdir -p execute_tests
cd execute_tests

export LD_LIBRARY_PATH=../../src/lib;

$QT_PATH/bin/qmake ../tests/testsuite.pro -r -spec linux-g++

make -j1 --silent check TESTARGS="-silent"

exit 0;

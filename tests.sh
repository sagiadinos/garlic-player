#!/bin/bash
PATH_QMAKE="qmake"

cd tests/

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../lib ; export LD_LIBRARY_PATH
$PATH_QMAKE testsuite.pro -r -spec linux-g++

make -j1 check TESTARGS="-silent"

exit 0;
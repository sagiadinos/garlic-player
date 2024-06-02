#!/bin/bash

mkdir -p build-tests
rm -rf build-tests/*
cd build-tests

cmake ..
make

ctest -V

exit 0


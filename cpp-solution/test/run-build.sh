#!/bin/sh
rm -rf build
mkdir build
cd build
cmake ..
cd googletest-build
make
cd ..
make

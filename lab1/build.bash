#!/bin/bash

export CC=/usr/sbin/clang

mkdir -p build
cd build || exit

cmake ..
cmake --build .
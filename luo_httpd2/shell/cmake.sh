#!/usr/bin/env bash

BUILD_DIR=../build
INI_DIR=/usr/local/etc/luo_httpd
BIN_PATH=/usr/local/bin/luo_httpd

if [[ -d $BUILD_DIR ]]; then
  echo "cleaning..."
  rm -rf $BUILD_DIR
  rm -rf $INI_DIR
  rm $BIN_PATH
fi

echo "mkdir $BUILD_DIR"
mkdir $BUILD_DIR

echo "cd $BUILD_DIR"
cd $BUILD_DIR

echo "cmake .."
cmake ..

echo "make"
make

echo "make install"
make install

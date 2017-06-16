#!/usr/bin/env bash

BUILD_DIR=./build
INI_DIR=/usr/local/etc/luo_db_pool
BIN_FILE=/usr/local/bin/luo_db_pool

echo "cleanup..."

if [[ -d $BUILD_DIR ]]; then
    echo "rf -rf $BUILD_DIR"
    rm -rf $BUILD_DIR
fi

if [[ -d $INI_DIR ]]; then
    echo "rf -rf $INI_DIR"
    rm -rf $INI_DIR
fi

if [[ -f $BIN_FILE ]]; then
    echo "rm $BIN_FILE"
    rm $BIN_FILE
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
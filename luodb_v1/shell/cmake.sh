#!/usr/bin/env bash

project_name="luodb"

root_dir=$(cd "$(dirname "$1")"; pwd)
echo "root_dir:     $root_dir"

build_dir=$root_dir/build
echo "build_dir:    $build_dir"

bin_dir=$root_dir/bin
echo "bin_dir:      $bin_dir"

echo "cleanup..."

if [[ -d $build_dir ]]; then
    echo "rf -rf $build_dir"
    rm -rf $build_dir
fi

if [[ -d $bin_dir ]]; then
    echo "rf -rf $bin_dir"
    rm -rf $bin_dir
fi

echo "mkdir $build_dir"
mkdir $build_dir

echo "mkdir $bin_dir"
mkdir $bin_dir

echo "cd $build_dir"
cd $build_dir

echo "cmake .."
cmake ..

echo "make"
make

if [[ -f $bin_dir/$project_name ]]; then
    strip $bin_dir/$project_name
    chmod +x $bin_dir/$project_name
fi

echo "done."
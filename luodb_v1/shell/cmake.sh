#!/usr/bin/env bash

project_name="luodb"

root_dir=$(cd "$(dirname "$1")"; pwd)
echo "root_dir:     $root_dir"

build_dir=$root_dir/build
echo "build_dir:    $build_dir"

bin_dir=$root_dir/bin
echo "bin_dir:      $bin_dir"

echo "cleanup start..."

if [[ -d $bin_dir ]]; then
    find $bin_dir/* | xargs rm
else
    mkdir $bin_dir
fi

if [[ -d $build_dir ]]; then
    find $build_dir/* | xargs rm -rf
else
    mkdir $build_dir
fi

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
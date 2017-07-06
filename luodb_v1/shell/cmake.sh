#!/usr/bin/env bash

project_name="luodb"
server_name="$project_name"-server
client_name="$project_name"-client

root_dir=$(cd "$(dirname "$1")" || exit; pwd)
echo "root_dir:     $root_dir"

build_dir="$root_dir"/build
echo "build_dir:    $build_dir"

bin_dir="$root_dir"/bin
echo "bin_dir:      $bin_dir"

echo "cleanup start..."

rm -rf "$bin_dir"
rm -rf "$build_dir"

echo "mkdir..."

mkdir "$bin_dir"
mkdir "$build_dir"

echo "cd $build_dir"
cd "$build_dir" || exit

echo "cmake .."
cmake ..

echo "make"
make

if [[ -f "$bin_dir"/"$server_name" ]]; then
    strip "$bin_dir"/"$server_name"
    chmod +x "$bin_dir"/"$server_name"
fi

if [[ -f "$bin_dir"/"$client_name" ]]; then
    strip "$bin_dir"/"$client_name"
    chmod +x "$bin_dir"/"$client_name"
fi

echo "cmake done."
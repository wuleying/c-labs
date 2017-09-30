#!/usr/bin/env bash

ENV_ROOT_DIR=$(cd "$(dirname "$1")" || exit; pwd)
ENV_BIN_DIR="$ENV_ROOT_DIR"/bin

valgrind --tool=memcheck --leak-check=full --show-reachable=yes "$ENV_BIN_DIR"/luodb-server
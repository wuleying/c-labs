#!/usr/bin/env bash

ENV_ROOT_DIR=$(cd "$(dirname "$1")" || exit; pwd)
ENV_BIN_DIR="$ENV_ROOT_DIR"/bin
PID_FILE="$ENV_BIN_DIR"/luodb.pid

#杀死原进程
kill -9 `sed -n 1p "$PID_FILE"`

if [[ -f "$PID_FILE" ]]; then
    rm "$PID_FILE"
fi

echo "luodb-server stopped."
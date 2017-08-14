#!/usr/bin/env bash

pwd;
shell/luodb-server-stop.sh
shell/cmake.sh server
shell/luodb-server-start.sh
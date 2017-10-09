#!/usr/bin/env bash

pgrep -f "luodb-client" | xargs kill -9

echo "luodb-client stopped."
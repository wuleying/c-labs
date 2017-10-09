#!/usr/bin/env bash

ps -ef | grep bin/luodb-client | grep -v 'grep'| awk '{printf $2}' | xargs kill -9
#!/bin/bash
cd $(cd $(dirname $0); pwd)/..

git fetch -p
git reset --hard origin/main
git clean -fdx
git submodule update --init --recursive

./setup/restart-server.sh

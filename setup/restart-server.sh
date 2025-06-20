#!/bin/bash
cd $(cd $(dirname $0); pwd)/..

docker compose down -v --rmi all

make setup-intern

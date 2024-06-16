#!/usr/bin/env bash

docker build -t libtock-c-newlib-4.4.0.20231231 .
id=$(docker create libtock-c-newlib-4.4.0.20231231)
docker cp $id:/libtock-c/newlib/libtock-newlib-4.4.0.20231231.zip libtock-newlib-4.4.0.20231231.zip

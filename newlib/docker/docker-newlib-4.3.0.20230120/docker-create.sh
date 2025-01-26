#!/usr/bin/env bash

docker build -t libtock-c-newlib-4.3.0.20230120 .
id=$(docker create libtock-c-newlib-4.3.0.20230120)
docker cp $id:/libtock-c/newlib/libtock-newlib-4.3.0.20230120.zip libtock-newlib-4.3.0.20230120.zip

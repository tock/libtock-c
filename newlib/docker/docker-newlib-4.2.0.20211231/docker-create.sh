#!/usr/bin/env bash

docker build -t libtock-c-newlib-4.2.0.20211231 .
id=$(docker create libtock-c-newlib-4.2.0.20211231)
docker cp $id:/libtock-c/newlib/libtock-newlib-4.2.0.20211231.zip libtock-newlib-4.2.0.20211231.zip

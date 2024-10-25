#!/usr/bin/env bash

docker build -t libtock-c-picolib-1.8.5 .
id=$(docker create libtock-c-picolib-1.8.5)
docker cp $id:/libtock-c/newlib/libtock-picolib-1.8.5.zip libtock-picolib-1.8.5.zip

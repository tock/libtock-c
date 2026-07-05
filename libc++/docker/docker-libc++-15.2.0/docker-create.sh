#!/usr/bin/env bash

docker build -t libtock-c-libcpp-15.2.0 .
id=$(docker create libtock-c-libcpp-15.2.0)
docker cp $id:/libtock-c/libc++/libtock-libc++-15.2.0.zip libtock-libc++-15.2.0.zip

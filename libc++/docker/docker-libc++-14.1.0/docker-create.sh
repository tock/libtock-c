#!/usr/bin/env bash

docker build -t libtock-c-libcpp-14.1.0 .
id=$(docker create libtock-c-libcpp-14.1.0)
docker cp $id:/libtock-c/libc++/libtock-libc++-14.1.0.zip libtock-libc++-14.1.0.zip

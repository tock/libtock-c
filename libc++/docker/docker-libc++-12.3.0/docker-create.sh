#!/usr/bin/env bash

docker build -t libtock-c-libcpp-12.3.0 .
id=$(docker create libtock-c-libcpp-12.3.0)
docker cp $id:/libtock-c/libc++/libtock-libc++-12.3.0.zip libtock-libc++-12.3.0.zip

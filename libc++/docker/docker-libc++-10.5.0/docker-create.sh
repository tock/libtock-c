#!/usr/bin/env bash

docker build -t libtock-c-libcpp-10.5.0 .
id=$(docker create libtock-c-libcpp-10.5.0)
docker cp $id:/libtock-c/libc++/libtock-libc++-10.5.0.zip libtock-libc++-10.5.0.zip

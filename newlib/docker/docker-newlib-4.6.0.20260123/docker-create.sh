#!/usr/bin/env bash

docker build -t libtock-c-newlib-4.6.0.20260123 .
id=$(docker create libtock-c-newlib-4.6.0.20260123)
docker cp $id:/libtock-c/newlib/libtock-newlib-4.6.0.20260123.zip libtock-newlib-4.6.0.20260123.zip

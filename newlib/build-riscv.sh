#!/usr/bin/env bash

set -x

NEWLIB_SRC_DIR=$1
NEWLIB_INSTALL_DIR=$2
TARGET=$3

$NEWLIB_SRC_DIR/configure --target=$TARGET \
  --disable-newlib-supplied-syscalls \
  --disable-nls \
  --enable-newlib-reent-small \
  --disable-newlib-fvwrite-in-streamio \
  --disable-newlib-fseek-optimization \
  --disable-newlib-wide-orient \
  --enable-newlib-nano-malloc \
  --disable-newlib-unbuf-stream-opt \
  --enable-lite-exit \
  --enable-newlib-global-atexit \
  --enable-newlib-nano-formatted-io \
  --prefix=`realpath $NEWLIB_INSTALL_DIR`

make -j$(nproc) CFLAGS_FOR_TARGET='-g -Os -ffunction-sections -fdata-sections'
make install

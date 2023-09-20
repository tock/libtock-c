#!/usr/bin/env bash

NEWLIB_SRC_DIR=$1

# We want to end up with newlib compiled for the `v6-m/nofp`, `v7-m/nofp`, and
# `v7e-m/nofp` architectures. For that to happen the arm-none-eabi-gcc compiler
# must have been compiled with multilib support for those architectures. We
# verify that here before actually building anything.
if ! arm-none-eabi-gcc --print-multi-lib | grep -q 'v6-m/nofp'; then
  echo "ERROR: arm-none-eabi-gcc not configured with multilib support for v6-m/nofp"
  exit -1
fi
if ! arm-none-eabi-gcc --print-multi-lib | grep -q 'v7-m/nofp'; then
  echo "ERROR: arm-none-eabi-gcc not configured with multilib support for v7-m/nofp"
  exit -1
fi
if ! arm-none-eabi-gcc --print-multi-lib | grep -q 'v7e-m/nofp'; then
  echo "ERROR: arm-none-eabi-gcc not configured with multilib support for v7e-m/nofp"
  exit -1
fi

$NEWLIB_SRC_DIR/configure --target=arm-none-eabi \
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
  --enable-newlib-nano-formatted-io

make -j$(nproc) CFLAGS_FOR_TARGET='-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative'

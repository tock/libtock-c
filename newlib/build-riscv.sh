#!/usr/bin/env bash

NEWLIB_SRC_DIR=$1

# Choose the target based on what toolchain is installed.
if command -v riscv64-none-elf-gcc &> /dev/null
then
    TARGET=riscv64-none-elf
elif command -v riscv32-none-elf-gcc &> /dev/null
then
    TARGET=riscv32-none-elf
elif command -v riscv64-elf-gcc &> /dev/null
then
    TARGET=riscv64-elf
else
    TARGET=riscv64-unknown-elf
fi


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
  --enable-newlib-nano-formatted-io

make -j$(nproc) CFLAGS_FOR_TARGET='-g -Os -ffunction-sections -fdata-sections'

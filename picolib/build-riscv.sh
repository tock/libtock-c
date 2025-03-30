#!/usr/bin/env bash

set -x

PICOLIB_SRC_DIR=$1
PICOLIB_INSTALL_DIR=$2

$PICOLIB_SRC_DIR/scripts/do-configure riscv64-unknown-elf -Dprefix=`realpath $PICOLIB_INSTALL_DIR` \
  -Dpicolib=false \
  -Dpicocrt=false \
  -Dpicocrt-lib=false \
  -Dtinystdio=false \
  -Dnewlib-stdio64=false \
  -Dmultilib-list=rv32i/ilp32,rv32im/ilp32,rv32imac/ilp32 \
  -Dnewlib-reent-small=true \
  -Dnewlib-global-atexit=true

ninja
ninja install

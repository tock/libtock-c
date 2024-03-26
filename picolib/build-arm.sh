#!/usr/bin/env bash

set -x

PICOLIB_SRC_DIR=$1
PICOLIB_INSTALL_DIR=$2

# We want to end up with newlib compiled for at least the `v6-m/nofp`,
# `v7-m/nofp`, and `v7e-m/nofp` architectures. For that to happen the
# arm-none-eabi-gcc compiler must have been compiled with multilib support for
# those architectures. We verify that here before actually building anything.
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

# Picolibc Configure Flags
#
# picolib=false             # Do not use picolib sbrk. We need to use the memop syscalls.
# picocrt=false             # Do not build crt0, we have our own.
# picocrt-lib=false         # Do not package crt0.
# tinystdio=false           # Do not use AVR tinystdio, which uses a different underlying interface (aka stdout object).
# newlib-stdio64=false      # Do not include 64 bit APIs.
# newlib-reent-small=true   # ?
# newlib-global-atexit=true # ?
$PICOLIB_SRC_DIR/scripts/do-configure arm-none-eabi -Dprefix=`realpath $PICOLIB_INSTALL_DIR` \
  -Dpicolib=false \
  -Dpicocrt=false \
  -Dpicocrt-lib=false \
  -Dtinystdio=false \
  -Dnewlib-stdio64=false \
  -Dnewlib-reent-small=true \
  -Dnewlib-global-atexit=true \
  -Dmultilib-list=thumb/v6-m/nofp,thumb/v7-m/nofp,thumb/v7e-m/nofp \
  -Dc_args="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative"

ninja
ninja install

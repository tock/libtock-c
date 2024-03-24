#!/usr/bin/env bash

set -x

GCC_SRC_DIR=$1
GCC_INSTALL_DIR=$2
LIBC_INCLUDE_PATH=$3
TARGET=$4

if [[ ! -e $LIBC_INCLUDE_PATH ]]; then
  echo "ERROR: Missing LIBC_INCLUDE_PATH, expected"
  echo "       $LIBC_INCLUDE_PATH"
  echo ""
  echo "Ensure that appropriate newlib version has been built before building libc++"
  exit 1
fi

export CFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -isystem $LIBC_INCLUDE_PATH"
export CXXFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -isystem $LIBC_INCLUDE_PATH"

if gcc --version | grep -q clang; then
  echo "$(tput bold)System gcc is clang. Overriding with gcc-13"
  echo "$(tput sgr0)You may need to brew install gcc@13 if you haven't."
  echo ""
  sleep 2
  export CC=gcc-13
  export CXX=g++-13

  gmp=$($CC -v 2>&1 | tr " " "\n" | grep gmp)
  mpfr=$($CC -v 2>&1 | tr " " "\n" | grep mpfr)
  mpc=$($CC -v 2>&1 | tr " " "\n" | grep mpc)
  isl=$($CC -v 2>&1 | tr " " "\n" | grep isl)
  extra_with="$gmp $mpfr $mpc $isl"
else
  extra_with=""
fi

$GCC_SRC_DIR/configure \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu \
  --target=$TARGET \
  --with-newlib $extra_with \
  --with-headers=$LIBC_INCLUDE_PATH \
  --enable-languages="c c++" \
  --prefix=`realpath $GCC_INSTALL_DIR` \

make -j$(nproc)
make install

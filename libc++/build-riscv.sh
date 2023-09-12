#!/usr/bin/env bash

GCC_SRC_DIR=$1

NEWLIB_VERSION=4.3.0.20230120

SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
NEWLIB_INCLUDE_PATH=$SCRIPTPATH/../newlib/newlib-$NEWLIB_VERSION/newlib/libc/include

if [[ ! -e $NEWLIB_INCLUDE_PATH ]]; then
  echo "ERROR: Missing NEWLIB_INCLUDE_PATH, expected"
  echo "       $NEWLIB_INCLUDE_PATH"
  echo ""
  echo "Ensure that appropriate newlib version has been built before building libc++"
  exit 1
fi

export CFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -isystem $NEWLIB_INCLUDE_PATH"
export CXXFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -isystem $NEWLIB_INCLUDE_PATH"

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

$GCC_SRC_DIR/configure \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu \
  --target=$TARGET \
  --with-newlib $extra_with \
  --with-headers=$NEWLIB_INCLUDE_PATH \
  --enable-languages="c c++" \

make -j$(nproc)

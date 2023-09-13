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

export CFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative -mthumb -isystem $NEWLIB_INCLUDE_PATH"
export CXXFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative -mthumb -isystem $NEWLIB_INCLUDE_PATH"

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

# Create our own build file for multilib compilation. This is selected with the
# `--with-multilib-list=@t-tock` flag. Due to how the configure process works,
# the file has to be in the gcc/config/arm folder.
cat << EOF > $GCC_SRC_DIR/gcc/config/arm/t-tock
For Tock!!

MULTILIB_OPTIONS     =
MULTILIB_DIRNAMES    =
MULTILIB_EXCEPTIONS  =
MULTILIB_MATCHES     =
MULTILIB_REUSE       =
MULTILIB_REQUIRED    =

# Each entry in MULTILIB_OPTIONS serves as a level in the build hierarchy.
MULTILIB_OPTIONS  += mthumb
MULTILIB_DIRNAMES += thumb

MULTILIB_OPTIONS  += march=armv6s-m/march=armv7-m/march=armv7e-m
MULTILIB_DIRNAMES += v6-m v7-m v7e-m

MULTILIB_OPTIONS  += mfloat-abi=soft
MULTILIB_DIRNAMES += nofp

# The actual targets we want to build.
MULTILIB_REQUIRED += mthumb/march=armv6s-m/mfloat-abi=soft
MULTILIB_REQUIRED += mthumb/march=armv7-m/mfloat-abi=soft
MULTILIB_REQUIRED += mthumb/march=armv7e-m/mfloat-abi=soft
EOF


$GCC_SRC_DIR/configure \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu \
  --target=arm-none-eabi \
  --with-multilib-list=@t-tock \
  --with-newlib $extra_with \
  --with-headers=$NEWLIB_INCLUDE_PATH \
  --enable-languages="c c++" \

make -j$(nproc)

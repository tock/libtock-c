#! /bin/bash


normal=$(tput sgr0)
red=$(tput setaf 1)
green=$(tput setaf 2)

# This script expects to execut in the libtock-c/openthread-tock directory. Confirm this is true.
current_directory=$(basename "$(pwd)")
if [ "$current_directory" != "openthread-tock" ]; then
    echo "${red}Error: The create_openthread-tock script must be invoked\
	    in the 'openthread-tock' directory. Exiting.${normal}"
    exit
fi


# Confirm that the git submodule has been checked out.
git submodule status -- openthread >/dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "${red}Error: git submodule 'openthread' not found.${normal}" 
    echo "To checkout the submodule, use the command ${green}$ git submodule update --init -- openthread${normal}"
    exit 
fi

set -e 
# Create build directory and invoke CMake to create and execute build system. We specify that
# the platform used will be external (Tock specific). For now, we specify the multiple
# instance flag. This may change in future iterations of this script. The logs are currently disabled
# to decrease code size.
ARGS="-DOT_APP_CLI=OFF -DOT_APP_NCP=OFF -DOT_PLATFORM=external -DOT_FULL_LOGS=OFF -DOT_JOINER=OFF"

# specify mtd or ftd
THREAD_TYPE=mtd

# if THREAD_TYPE is mtd add -DOT_MTD=ON -DOT_FTD=OFF to ARGS
if [ "$THREAD_TYPE" = "mtd" ]; then
	ARGS+=" -DOT_MTD=ON -DOT_FTD=OFF"
fi

# if THREAD_TYPE is ftd add -DOT_MTD=OFF -DOT_FTD=ON to ARGS
if [ "$THREAD_TYPE" = "ftd" ]; then
	ARGS+=" -DOT_MTD=OFF -DOT_FTD=ON"
fi

mkdir -p build && cd build && cmake .. $ARGS -Wno-dev  && cmake --build . --target openthread-$THREAD_TYPE && cd .. 

# Specify the directory containing the .a files
library_directory="build/openthread"

# Specify where libraries will be placed to follow Tock's external library structure. This
# structure is specified in:
# [https://github.com/tock/libtock-c/blob/master/doc/compilation.md].
TEMPLATE_BUILD_DIR="build/cortex-m4"
DST_DIR="lib"

libs=("libopenthread" "libmbedx509" "libmbedcrypto" "libmbedtls" "libtcplp")
for lib in "${libs[@]}"; do
	mkdir -p "lib/$lib/$TEMPLATE_BUILD_DIR"
done

# Copy each static library from the openthread build to the Tock external library directory #

# libopenthread-ftd
cp -f $library_directory/src/core/libopenthread-$THREAD_TYPE.a $DST_DIR/libopenthread/build/cortex-m4/libopenthread.a

# libmbedx509
cp -f $library_directory/third_party/mbedtls/repo/library/libmbedx509.a $DST_DIR/libmbedx509/build/cortex-m4/libmbedx509.a

# libmbedcrypto
cp -f $library_directory/third_party/mbedtls/repo/library/libmbedcrypto.a $DST_DIR/libmbedcrypto/build/cortex-m4/libmbedcrypto.a

# libmedtls
cp -f $library_directory/third_party/mbedtls/repo/library/libmbedtls.a $DST_DIR/libmbedtls/build/cortex-m4/libmbedtls.a

# libtcplp
cp -f $library_directory/third_party/tcplp/libtcplp-$THREAD_TYPE.a $DST_DIR/libtcplp/build/cortex-m4/libtcplp.a



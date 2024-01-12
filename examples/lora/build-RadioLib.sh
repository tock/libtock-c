#!/bin/bash

pushd RadioLib/examples/NonArduino/Tock/

rm -rf build

# Change the source to point to the current libtock-c directory
# Note, x-platform `sed -i` has odd, but particular syntax
# https://stackoverflow.com/questions/5694228/sed-in-place-flag-that-works-both-on-mac-bsd-and-linux
sed -i._SED_HACK 's|${CMAKE_CURRENT_SOURCE_DIR}/libtock-c|../../../../../../../|g' CMakeLists.txt
sed -i._SED_HACK 's|target_include_directories(${PROJECT_NAME} PUBLIC|target_include_directories(\${PROJECT_NAME}\n    PUBLIC ../../../../../../|g' CMakeLists.txt

find . -name '*._SED_HACK' -delete

mkdir -p build
cd build

cmake -G "CodeBlocks - Unix Makefiles" ..

# Build the Tock example application
# This will fail to link, as it can't find the libtock-c libraries
# That's fine for us though, as we just need to build the RadioLib
# library, not the entire example application
make -j4 2> /dev/null

popd

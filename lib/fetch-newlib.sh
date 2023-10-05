#!/usr/bin/env bash

NEWLIB_VERSION=4.3.0.20230120
NEWLIB_SHA=afbc6f4c20fc47e56199f043f303c7fde8eb1213a2cb907c463be05ff9c258e9

LIBTOCKC_ROOT_DIR=$1

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-newlib-$NEWLIB_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock/"\
  "https://alpha.mirror.svc.schuermann.io/files/"\
)

# Try from each mirror until we successfully download a .zip file.
# pushd $LIBTOCKC_ROOT_DIR/lib
for MIRROR in ${MIRRORS[@]}; do
  URL=$MIRROR/$ZIP_FILE
  echo "Fetching newlib from ${MIRROR}..."
  echo "  Fetching ${URL}..."
  wget -q "$URL" && (echo "$NEWLIB_SHA $ZIP_FILE" | sha256sum -c)
  if [ $? -ne 0 ]; then
    echo "WARNING: Fetching newlib from mirror $MIRROR failed!" >&2
  else
    break
  fi
done
unzip $ZIP_FILE
# popd

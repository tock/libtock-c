#!/usr/bin/env bash

PICOLIB_VERSION=$1

if [ $PICOLIB_VERSION = "1.8.5" ]; then
  PICOLIB_SHA="5168a7ea6522717ec5867e0105a224fe7e3779371b095deedc2cd58da369d8cc"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-picolib-$PICOLIB_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files"\
)

let FOUND=0

# Try from each mirror until we successfully download a .zip file.
for MIRROR in ${MIRRORS[@]}; do
  URL=$MIRROR/$ZIP_FILE
  echo "Fetching picolib from ${MIRROR}..."
  echo "  Fetching ${URL}..."
  wget -q "$URL" && (echo "$PICOLIB_SHA $ZIP_FILE" | sha256sum -c)
  if [ $? -ne 0 ]; then
    echo "  WARNING: Fetching picolib from mirror $MIRROR failed!" >&2
  else
    let FOUND=1
    break
  fi
done

if [[ $FOUND -ne 0 ]]; then
  unzip $ZIP_FILE
else
  echo "ERROR: Unable to find tock-picolib"
  exit -1
fi

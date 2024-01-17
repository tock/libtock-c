#!/usr/bin/env bash

NEWLIB_VERSION=$1

if [ $NEWLIB_VERSION = "4.3.0.20230120" ]; then
  NEWLIB_SHA="2595f02f7cb2fd2e444f4ddc7955deca4c52deb3f91411c4d28326be8b0d9e0d"
elif [ $NEWLIB_VERSION = "4.2.0.20211231" ]; then
  NEWLIB_SHA="5916d76f1cc3c0f5487275823c85a9a9954edfa15f5706342ecb254d634ed559"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-newlib-$NEWLIB_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files/tock"\
)

let FOUND=0

# Try from each mirror until we successfully download a .zip file.
for MIRROR in ${MIRRORS[@]}; do
  URL=$MIRROR/$ZIP_FILE
  echo "Fetching newlib from ${MIRROR}..."
  echo "  Fetching ${URL}..."
  wget -q "$URL" && (echo "$NEWLIB_SHA $ZIP_FILE" | sha256sum -c)
  if [ $? -ne 0 ]; then
    echo "  WARNING: Fetching newlib from mirror $MIRROR failed!" >&2
  else
    let FOUND=1
    break
  fi
done

if [[ $FOUND -ne 0 ]]; then
  unzip $ZIP_FILE
else
  echo "ERROR: Unable to find tock-newlib"
  exit -1
fi

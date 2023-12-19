#!/usr/bin/env bash

NEWLIB_VERSION=$1

if [ $NEWLIB_VERSION = "4.3.0.20230120" ]; then
  NEWLIB_SHA="eab60504bd2ee5408242e9079f883ce82d00cec6fa439bf9b7869eae0b9dba05"
elif [ $NEWLIB_VERSION = "4.2.0.20211231" ]; then
  NEWLIB_SHA="33fbb7ab2b0be189110d39d615905bbf4d51cc8181c138fdf9219c96aea6f114"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-newlib-$NEWLIB_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files"\
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

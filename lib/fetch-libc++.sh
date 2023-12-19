#!/usr/bin/env bash

GCC_VERSION=$1

if [ $GCC_VERSION = "13.2.0" ]; then
  GCC_SHA="8f57e699f77fddc0c9e123b64793a7e159b14a61f7052713d06d34d41d084308"
elif [ $GCC_VERSION = "12.3.0" ]; then
  GCC_SHA="9b6f3a5cf0c2de887683e17b3945b056347f03bccb6129340cfb0d49d6cb3c95"
elif [ $GCC_VERSION = "10.5.0" ]; then
  GCC_SHA="705050dec9331b93ce5cc9cff63e564cdb541f09d84f299f3cde97ad5d2cb343"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-libc++-$GCC_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files"\
)

let FOUND=0

# Try from each mirror until we successfully download a .zip file.
for MIRROR in ${MIRRORS[@]}; do
  URL=$MIRROR/$ZIP_FILE
  echo "Fetching libc++ from ${MIRROR}..."
  echo "  Fetching ${URL}..."
  wget -q "$URL" && (echo "$GCC_SHA $ZIP_FILE" | sha256sum -c)
  if [ $? -ne 0 ]; then
    echo "  WARNING: Fetching libc++ from mirror $MIRROR failed!" >&2
  else
    let FOUND=1
    break
  fi
done

if [[ $FOUND -ne 0 ]]; then
  unzip $ZIP_FILE
else
  echo "ERROR: Unable to find tock-libc++"
  exit -1
fi

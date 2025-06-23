#!/usr/bin/env bash

GCC_VERSION=$1

if [ $GCC_VERSION = "14.1.0" ]; then
  GCC_SHA="46d50ea6a380f2c977e6aad187da216c35b793b31b3ea6de9646339c2a22f13c"
elif [ $GCC_VERSION = "13.2.0" ]; then
  GCC_SHA="9940242a1390f897f8c3fe6e7a8821e863580797f9a6e80a91f52e41dd8086a1"
elif [ $GCC_VERSION = "12.3.0" ]; then
  GCC_SHA="b0686eb1905594bde7b746fc58be97aceac8f802d8b5171adb6a4e84f3906d30"
elif [ $GCC_VERSION = "10.5.0" ]; then
  GCC_SHA="1cd4eef592bcc7b9ec77e2c21b50dabcff9b614b4cd1ec82a9dac238c8789c95"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-libc++-$GCC_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files/tock"\
)

if test -x /usr/bin/shasum; then
  CHECK_SHA_CMD="shasum -a 256 -c"
else
  CHECK_SHA_CMD="sha256sum -c"
fi

let FOUND=0

# Try from each mirror until we successfully download a .zip file.
for MIRROR in ${MIRRORS[@]}; do
  URL=$MIRROR/$ZIP_FILE
  echo "Fetching libc++ from ${MIRROR}..."
  echo "  Fetching ${URL}..."
  # Note: There must be two space characters for `shasum` (sha256sum doesn't care)
  wget -O $ZIP_FILE  "$URL" && (echo "$GCC_SHA  $ZIP_FILE" | $CHECK_SHA_CMD)
  if [ $? -ne 0 ]; then
    if test -f $ZIP_FILE; then
      file $ZIP_FILE
      ls -l $ZIP_FILE
      shasum -a 256 $ZIP_FILE
    fi
    echo "  WARNING: Fetching libc++ from mirror $MIRROR failed!" >&2
  else
    let FOUND=1
    break
  fi
done

if [[ $FOUND -ne 0 ]]; then
  echo "Unpacking $ZIP_FILE..."
  unzip -q $ZIP_FILE
  echo "Done upacking $ZIP_FILE..."
else
  echo "ERROR: Unable to find tock-libc++"
  exit -1
fi

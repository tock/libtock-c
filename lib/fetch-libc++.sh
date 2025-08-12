#!/usr/bin/env bash

# Exit on error:
set -Eeuo pipefail

GCC_VERSION=$1

if [[ $GCC_VERSION == "14.1.0" ]]; then
  GCC_SHA="46d50ea6a380f2c977e6aad187da216c35b793b31b3ea6de9646339c2a22f13c"
elif [[ $GCC_VERSION == "13.2.0" ]]; then
  GCC_SHA="9940242a1390f897f8c3fe6e7a8821e863580797f9a6e80a91f52e41dd8086a1"
elif [[ $GCC_VERSION == "12.3.0" ]]; then
  GCC_SHA="b0686eb1905594bde7b746fc58be97aceac8f802d8b5171adb6a4e84f3906d30"
elif [[ $GCC_VERSION == "10.5.0" ]]; then
  GCC_SHA="1cd4eef592bcc7b9ec77e2c21b50dabcff9b614b4cd1ec82a9dac238c8789c95"
fi

# Name of the pre-created compiled directories.
ZIP_FILE=libtock-libc++-$GCC_VERSION.zip

# List of mirrors we support.
MIRRORS=(\
  "https://www.cs.virginia.edu/~bjc8c/archive/tock"\
  "https://alpha.mirror.svc.schuermann.io/files/tock"\
)

function check_sha256() {
  if test -x /usr/bin/shasum; then
    echo "$1  $2" | shasum -a 256 -c
    return $?
  else
    sha256sum -c <(echo "$1  $2")
    return $?
  fi
}

FOUND=0

function fetch_and_unpack() {
  # Try from each mirror until we successfully download a .zip file.
  for MIRROR in ${MIRRORS[@]}; do
    URL=$MIRROR/$ZIP_FILE
    echo "Fetching libc++ from ${MIRROR}..."
    echo "  Fetching ${URL}..."
    # Note: There must be two space characters for `shasum` (sha256sum doesn't care)
    wget -O $ZIP_FILE "$URL" && check_sha256 "$GCC_SHA" "$ZIP_FILE"
    if [ $? -ne 0 ]; then
      if test -f $ZIP_FILE; then
        file $ZIP_FILE
        ls -l $ZIP_FILE
        shasum -a 256 $ZIP_FILE
      fi
      echo "  WARNING: Fetching libc++ from mirror $MIRROR failed!" >&2
    else
      FOUND=1
      break
    fi
  done

  if [[ $FOUND -ne 0 ]]; then
    echo "Unpacking $ZIP_FILE..."
    # -n: don't overwrite existing files, -q: quiet mode
    unzip -n -q $ZIP_FILE
    echo "Done upacking $ZIP_FILE..."
    exit 0
  else
    echo "ERROR: Unable to find tock-libc++"
    exit -1
  fi
}

# We must ensure that multiple parallel fetch and unzip operations
# don't trample over each other, which we do by obtaining a write-lock
# on the ZIP file that's being downloaded / extracted.
#
# This will also truncate any already downloaded ZIP file, which is
# fine because we'll overwrite it anyways.
#
# Under Linux platforms, we can expect `flock` to be installed, whereas for
# BSDs and macOS, we use `shlock` instead:
if which flock 2>&1 >/dev/null; then
  NONBLOCK_LOCK_ACQ_FAIL=0
  # We'd like for bash to choose the file descriptor, but that syntax
  # ({fd}<"${ZIP_FILE}") isn't supported on the ancient version of
  # Bash shipped on macOS. So we just pick one (200).
  while true; do
    : >> "${ZIP_FILE}"
    {
      if [ $NONBLOCK_LOCK_ACQ_FAIL -eq 0 ]; then
        flock -n 200 || NONBLOCK_LOCK_ACQ_FAIL=1
        if [ $NONBLOCK_LOCK_ACQ_FAIL -ne 0 ]; then
          # Try again, blocking this time:
          echo "Could not acquire non-blocking lock on ${ZIP_FILE}, waiting for lock to be released..." >&2
          continue
        fi
      else
        flock 200
      fi
      echo "Acquired lock on file ${ZIP_FILE}" >&2
      fetch_and_unpack "${ZIP_FILE}"
    } 200<"${ZIP_FILE}"
  done
elif which shlock 2>&1 >/dev/null; then
  while true; do
    trap "rm -f ${ZIP_FILE}.lock" EXIT
    if shlock -f "${ZIP_FILE}.lock" -p "$$"; then
      break
    else
      echo "Could not acquire lock on ${ZIP_FILE}.lock, retrying in 10..." >&2
      sleep 10
    fi
  done
  echo "Acquired lock on file ${ZIP_FILE}" >&2
  fetch_and_unpack "${ZIP_FILE}"
else
  echo "Neither \"flock\" nor \"shlock\" seem to be installed, cannot proceed!"
  exit 1
fi

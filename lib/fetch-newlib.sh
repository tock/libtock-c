#!/usr/bin/env bash

# Exit on error:
set -e

NEWLIB_VERSION=$1

if [ $NEWLIB_VERSION = "4.4.0.20231231" ]; then
  NEWLIB_SHA="686af44e1bba625eb24b3cfb1fd2d48a61848c1edebbd49b5dbec554ebf2ea94"
elif [ $NEWLIB_VERSION = "4.3.0.20230120" ]; then
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

if test -x /usr/bin/shasum; then
  CHECK_SHA_CMD="shasum -a 256 -c"
else
  CHECK_SHA_CMD="sha256sum -c"
fi

FOUND=0

# We must ensure that multiple parallel fetch and unzip operations
# don't trample over each other, which we do by obtaining a write-lock
# on the ZIP file that's being downloaded / extracted.
#
# This will also truncate any already downloaded ZIP file, which is
# fine because we'll overwrite it anyways.
NONBLOCK_LOCK_ACQ_FAIL=0
while true; do
    : >> ${ZIP_FILE}
    {
	if [ $NONBLOCK_LOCK_ACQ_FAIL -eq 0 ]; then
            flock -n $fd || NONBLOCK_LOCK_ACQ_FAIL=1
	    if [ $NONBLOCK_LOCK_ACQ_FAIL -ne 0 ]; then
		# Try again, blocking this time:
		echo "Could not acquire non-blocking lock on ${ZIP_FILE}, waiting for lock to be released..." >&2
		continue
	    fi
	else
	    flock $fd
	fi
        echo "Acquired lock on file ${ZIP_FILE}" >&2

        # Try from each mirror until we successfully download a .zip file.
        for MIRROR in ${MIRRORS[@]}; do
          URL=$MIRROR/$ZIP_FILE
          echo "Fetching newlib from ${MIRROR}..."
          echo "  Fetching ${URL}..."
          # Note: There must be two space characters for `shasum` (sha256sum doesn't care)
          wget -O $ZIP_FILE "$URL" && (echo "$NEWLIB_SHA  $ZIP_FILE" | $CHECK_SHA_CMD)
          if [ $? -ne 0 ]; then
            echo "  WARNING: Fetching newlib from mirror $MIRROR failed!" >&2
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
          echo "ERROR: Unable to find tock-newlib"
          exit -1
        fi
    } {fd}<${ZIP_FILE}
done

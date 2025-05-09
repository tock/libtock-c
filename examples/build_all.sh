#!/usr/bin/env bash

NUM_JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || 4)

# Sentinel flag to indicate to apps that a `build all` is running.
#
# Generally, apps should not be sensitive to this flag. Build all is
# usually only invoked by a user when they're looking for local, CI-like
# behavior. However, a local repo checkout is not quite the same as a
# pristine CI environment, and certain test apps may need slightly
# different behavior for a local "build all" versus CI.
#
# For example: Some apps, e.g. LoRa-based ones, require radio
# configuration that end users *must* set themselves [e.g. to select
# operating region]. For users of these apps, we *do not* want them to
# build successfully 'out of the box' without explicit user
# intervention, but we do still want them to be compile-tested in CI. An
# app may ship with a `radioConfig_example.h` but include (and
# `.gitignore`) `radioConfig.h`.  CI can unconditionally swap in the
# `_example` version, but a local checkout should use a real config file
# it is available. At the same time, apps which are not used in a local
# checkout should not throw compile errors for a local "build all", and
# should thus swap in the `_example` if a real config is not present. An
# app build which is sensitive to `TOCK_BUILDALL` can capture this
# subtlety.
export TOCK_BUILDALL=1

set -e
set -u
set -o pipefail

bold=$(tput bold)
normal=$(tput sgr0)
red=$(tput setaf 1)

# Try to build everything and collect failures at the end
declare -a failures

function opt_rebuild {
	if [ "${CI-}" == "true" ]; then
		echo "::group::Verbose Rebuild for $1"
		echo "${bold}Rebuilding Verbose: $1${normal}"
		make CFLAGS=-Werror V=1
		echo "::endgroup::"
	fi
}

for mkfile in `find . -maxdepth 6 -name Makefile`; do
	dir=`dirname $mkfile`
	if [ $dir == "." ]; then continue; fi
	# Skip directories with leading _'s, useful for leaving test apps around
	if [[ $(basename $dir) == _* ]]; then continue; fi

	# If running under CI, give a hint to the UX to show each app build independently
	if [ "${CI-}" == "true" ]; then
		echo "::group::Build for $dir"
	fi

	pushd $dir > /dev/null
	echo ""
	echo "Building $dir"
	make CFLAGS=-Werror -j $NUM_JOBS || { echo "${bold} ⤤ Failure building $dir${normal}" ; opt_rebuild $dir; failures+=("$dir"); }
	popd > /dev/null

	if [ "${CI-}" == "true" ]; then
		echo "::endgroup::"
	fi
done

# https://stackoverflow.com/questions/7577052/bash-empty-array-expansion-with-set-u
if [[ ${failures[@]+"${failures[@]}"} ]]; then
	echo ""
	echo "${bold}${red}Build Failures:${normal}"
	for fail in ${failures[@]}; do
		echo $fail
	done
	exit 1
fi

echo ""
echo "${bold}All Built.${normal}"

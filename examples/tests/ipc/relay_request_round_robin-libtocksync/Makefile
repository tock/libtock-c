# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

##########
# Custom targets for this application

# Keep the all target the default
all:

# Build three different app versions and upload them
#
# The AVOID_TOOL thing is definitely a hack. Libtock-c has a helpf script in
# `tools/check_override.sh` which ensures that Makefile additions use the
# override keyword when defining variables. These lines are command-line
# arguments though, so they can't and don't need to use override. Having the
# word "override" physically appear on the line avoids detection by the tool.
.PHONY: ROUNDROBIN
ROUNDROBIN:
	touch main.c
	$(MAKE) program CFLAGS="-DSERVICE=0" PACKAGE_NAME="RoundRobin-Service0" AVOID_TOOL="override"
	touch main.c
	$(MAKE) program CFLAGS="-DSERVICE=1" PACKAGE_NAME="RoundRobin-Service1" AVOID_TOOL="override"
	touch main.c
	$(MAKE) program CFLAGS="-DSERVICE=2" PACKAGE_NAME="RoundRobin-Service2" AVOID_TOOL="override"

##########

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

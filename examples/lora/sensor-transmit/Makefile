# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

# Use the libtock-c Make system
include $(TOCK_USERLAND_BASE_DIR)/libradio/Makefile

include $(TOCK_USERLAND_BASE_DIR)/libradio/Makefile.app

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

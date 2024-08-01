# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

override CPPFLAGS += -DRADIOLIB_CLOCK_DRIFT_MS=9

ifneq ($(CI),)
override CPPFLAGS += "-DRADIO_CONFIG_CI=radioConfig_example.h"
endif

ELF2TAB_ARGS += --write_id 17767 --read_ids 17767 --access_ids 17767

# Use the libtock-c Make system
EXTERN_LIBS := $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib
include $(TOCK_USERLAND_BASE_DIR)/libradio/Makefile

# `EXTERN_LIBS` points to `libradio/RadioLib` so we pull in `Makefile.app`
# manually.
include $(TOCK_USERLAND_BASE_DIR)/libradio/Makefile.app

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

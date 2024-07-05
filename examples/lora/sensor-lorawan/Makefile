# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

STACK_SIZE = 4096

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

override CPPFLAGS += -DRADIOLIB_CLOCK_DRIFT_MS=9

ifneq ($(CI),)
override CPPFLAGS += "-DRADIO_CONFIG_CI=radioConfig_example.h"
endif

# Use the libtock-c Make system
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/RadioLib

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

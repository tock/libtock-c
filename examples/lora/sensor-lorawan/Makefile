# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

STACK_SIZE = 4096

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

override CPPFLAGS += -DRADIOLIB_CLOCK_DRIFT_MS=9

# If we are building for a testing configuration (e.g. CI) then it's okay to
# use the example config header. However, if someone is doing local testing,
# especially of a different thing, we don't want to accidentally overwrite
# the build obeject here with the example config when a real one exists.
ifneq ($(TOCK_BUILDALL),)
ifeq ($(wildcard radioConfig.h),)
override CPPFLAGS += "-DRADIO_CONFIG_CI=radioConfig_example.h"
endif
endif

# Use the libtock-c Make system
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/RadioLib

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lua53
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/lua/*.c)

override CFLAGS += -DLUA_32BITS -D"luai_makeseed()"=0

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libtcplp
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libtcplp
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/third_party/tcplp

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/lib/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/bsdtcp/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/bsdtcp/cc/*.c)


# Need openthread headers
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/../../include

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

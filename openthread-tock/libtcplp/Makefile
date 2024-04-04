.PHONY: default
default: all ;

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libtcplp
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libtcplp
LIB_SRC_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/third_party/tcplp

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/lib/*.c)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/bsdtcp/*.c)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/bsdtcp/cc/*.c)



# Need openthread headers
override CPPFLAGS += -I$(LIB_SRC_DIR)/../../include

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk










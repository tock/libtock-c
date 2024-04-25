# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libtcplp
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libopenthread/libtcplp
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/third_party/tcplp

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/lib/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/bsdtcp/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/bsdtcp/cc/*.c)

# Need openthread headers
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/../../include

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# tcplp is generating the following errors, so we supress them here
override CPPFLAGS_$(LIBNAME) += -Wno-sign-compare
override CPPFLAGS_$(LIBNAME) += -Wno-redundant-decls
override CPPFLAGS_$(LIBNAME) += -Wno-missing-prototypes
override CPPFLAGS_$(LIBNAME) += -Wno-missing-declarations
override CPPFLAGS_$(LIBNAME) += -Wno-unused-parameter

# tcplp also generates a stack usage error, which there is not a way
# to silence. We apply a broadstroke disabling of all errors to 
# silence this error for this library.
override CPPFLAGS_$(LIBNAME) += -w

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

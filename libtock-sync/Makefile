# Libtock-sync makefile. Can build Libtock standalone. Also included by application
# makefiles to ensure their libtock dependency is built

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libtocksync
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libtock-sync
$(LIBNAME)_SRC_ROOT := $($(LIBNAME)_DIR)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_SRC_ROOT)/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/crypto/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/display/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/interface/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/net/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/peripherals/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/sensors/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/storage/*.c)

# Need libtock headers.
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

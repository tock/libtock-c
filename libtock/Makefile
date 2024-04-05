# Libtock makefile. Can build Libtock standalone. Also included by application
# makefiles to ensure their libtock dependency is built

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libtock
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)
$(LIBNAME)_SRC_ROOT := $($(LIBNAME)_DIR)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_SRC_ROOT)/internal/*.c) $(wildcard $($(LIBNAME)_SRC_ROOT)/*.c) $(wildcard $($(LIBNAME)_SRC_ROOT)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

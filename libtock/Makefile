# Libtock makefile. Can build Libtock standalone. Also included by application
# makefiles to ensure their libtock dependency is built

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libtock
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/internal/*.c) $(wildcard $($(LIBNAME)_DIR)/*.c) $(wildcard $($(LIBNAME)_DIR)/*.s)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/chips/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/crypto/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/crypto/syscalls/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/display/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/interface/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/interface/syscalls/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/kernel/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/net/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/net/syscalls/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/peripherals/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/peripherals/syscalls/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/sensors/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/sensors/syscalls/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/storage/*.c)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

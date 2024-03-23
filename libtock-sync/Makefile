# Libtock-sync makefile. Can build Libtock standalone. Also included by application
# makefiles to ensure their libtock dependency is built

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libtocks
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libtock-sync

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/crypto/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/display/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/interface/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/net/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/peripherals/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/sensors/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/storage/*.c)

# Need libtock headers.
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := picolib-tock
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_DIR)/*.c)

override CPPFLAGS_$(LIBNAME) += -I$(TOCK_USERLAND_BASE_DIR)/libtock

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

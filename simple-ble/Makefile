# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := simple-ble
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)
$(LIBNAME)_SRC_ROOT := $($(LIBNAME)_DIR)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_SRC_ROOT)/*.c)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

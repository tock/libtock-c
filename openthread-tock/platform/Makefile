# Makefile for openthread-tock platform library
TOCK_USERLAND_BASE_DIR ?= ../../..

LIBNAME := platform-tock

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/include
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/src/core
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock
$(LIBNAME)_SRC_ROOT := $($(LIBNAME)_DIR)
$(LIBNAME)_SRCS  := $(wildcard $($(LIBNAME)_SRC_ROOT)/*.c)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

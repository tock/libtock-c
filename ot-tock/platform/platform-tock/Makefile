# Makefile for ot-tock platform library
TOCK_USERLAND_BASE_DIR ?= ../../..
LIBNAME := platform-tock

CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/include
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/src/core
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock
$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/*.c)                   \
    $(wildcard $($(LIBNAME)_DIR)/*.cxx)                 \ 
    $(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

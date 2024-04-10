# Makefile for libopenthread platform library
TOCK_USERLAND_BASE_DIR ?= ../..

LIBNAME := platform

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/include
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread/src/core
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libopenthread/platform
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/libopenthread/platform
$(LIBNAME)_SRCS :=				\
	$(wildcard $($(LIBNAME)_DIR)/*.c)	\
	$(wildcard $($(LIBNAME)_DIR)/*.cxx)	\
	$(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

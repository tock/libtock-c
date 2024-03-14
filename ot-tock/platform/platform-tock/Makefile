.PHONY: default
default: all ;

$(info Makefile)
# Makefile for ot-tock platform library
TOCK_USERLAND_BASE_DIR ?= ../../..

# Calling this here feels a bit hacky, but I am struggling 
# to find a different approach to call the openthread build 
# script. Ideas to improve this are welcome.
#build_script:
#	cd $(TOCK_USERLAND_BASE_DIR)/ot-tock && ./create_ot-tock

LIBNAME := platform-tock

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/include
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/src/core
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock
$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/*.c)                   \
    $(wildcard $($(LIBNAME)_DIR)/*.cxx)                 \ 
    $(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

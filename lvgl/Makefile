TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lvgl
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

$(LIBNAME)_SRCS += $(shell find $($(LIBNAME)_DIR)/lvgl/src -type f -name '*.S')
$(LIBNAME)_SRCS += $(shell find $($(LIBNAME)_DIR)/lvgl/src -type f -name '*.c')
$(LIBNAME)_SRCS += $(shell find $($(LIBNAME)_DIR)/lvgl/src -type f -name '*.cpp')

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error -I$(LIBNAME)_DIR/lvgl

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk


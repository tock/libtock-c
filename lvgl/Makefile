TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lvgl
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)


DEPPATH += --dep-path $(LVGL_DIR)
VPATH += :$(LVGL_DIR)

LVGL_DIR = $(TOCK_USERLAND_BASE_DIR)/lvgl

include $(LVGL_DIR)/lvgl/lvgl.mk

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(CSRCS)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk


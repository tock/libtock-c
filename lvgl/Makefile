TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lvgl
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)


DEPPATH += --dep-path $(LVGL_DIR)
VPATH += :$(LVGL_DIR)
CSRCS += 

override CFLAGS += -Wno-error=unused-macros -Wno-error=sign-compare -Wno-error=float-equal\
  -Wno-error=unused-parameter -Wno-error=unused-variable\
  -Wno-error=shadow -Wno-error=type-limits -Wno-error=overflow

LVGL_DIR = $(TOCK_USERLAND_BASE_DIR)/lvgl

include $(LVGL_DIR)/lvgl/lvgl.mk

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(CSRCS)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk


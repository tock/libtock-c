TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lvgl
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/core/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/draw/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/font/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/gpu/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/hal/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/misc/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/widgets/*.c)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

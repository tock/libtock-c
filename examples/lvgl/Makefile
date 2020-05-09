# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

LVGL_DIR = $(TOCK_USERLAND_BASE_DIR)/lvgl

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/lvgl -DLVGL -DLV_CONF_INCLUDE_SIMPLE -I.
include $(TOCK_USERLAND_BASE_DIR)/lvgl/lvgl.mk

# Which files to compile.
C_SRCS := $(wildcard *.c)
C_SRCS+=$(CSRCS)

APP_HEAP_SIZE := 20000

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

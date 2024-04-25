# Include this Makefile in the app's makefile to use the OpenThread Minimal
# Thread Device (MTD) library.

TOCK_USERLAND_BASE_DIR ?= ..
LIBOPENTHREAD_DIR := $(TOCK_USERLAND_BASE_DIR)/libopenthread

EXTERN_LIBS += $(LIBOPENTHREAD_DIR)/libopenthread-mtd
EXTERN_LIBS += $(LIBOPENTHREAD_DIR)/libtcplp
EXTERN_LIBS += $(LIBOPENTHREAD_DIR)/libmbedtls
EXTERN_LIBS += $(LIBOPENTHREAD_DIR)/platform

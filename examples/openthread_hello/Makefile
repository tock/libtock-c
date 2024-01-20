# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/include
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/src/core
# CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/examples/platforms

EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/ot-tock
APP_HEAP_SIZE:=40000
# Which files to compile.


C_SRCS := dummy_platform.cpp 
C_SRCS += $(wildcard *.c)

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

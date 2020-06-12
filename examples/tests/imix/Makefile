# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# imix is only cortex-m4
TOCK_TARGETS := cortex-m4

APP_HEAP_SIZE := 2048

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/libnrfserialization

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

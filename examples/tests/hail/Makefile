# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

APP_HEAP_SIZE := 2048

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/libnrfserialization

# Comma separated list of boards this app is intended for. Optional.
SUPPORTED_BOARDS = hail

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

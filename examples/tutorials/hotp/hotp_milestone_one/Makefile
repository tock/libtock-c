# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../../

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include U8G2 graphics library.
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/u8g2

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

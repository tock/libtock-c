# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

PACKAGE_NAME = org.tockos.examples.rot13

# For IPC, we only support 32-bit platforms for now.
TOCK_TARGETS_32BIT_ONLY := yes

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

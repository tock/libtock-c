# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

ELF2TAB_ARGS += --read_ids 9090 --access_ids 9090

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

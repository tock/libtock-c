# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Make sure we have storage permissions.
ELF2TAB_ARGS += --write_id 0x4016 --read_ids 0x4016 --access_ids 0x4016

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

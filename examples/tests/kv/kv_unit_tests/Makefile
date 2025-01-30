# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Magic numbers that have to allign with what the kernel gives this
# application as a short ID.
ELF2TAB_ARGS += --write_id 1153320202 --read_ids 1153320202 --access_ids 1153320202

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

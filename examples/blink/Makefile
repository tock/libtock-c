# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

ELF2TAB_ARGS=--app-version 7 --sha256 --sha512 --permissions 128,0 128,1 128,2 128,3 6001,5 0,9 0,1004 --write_id 48877 --read_ids 4 5 9 88 --access_ids 111

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

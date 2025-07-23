# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Set the SHA256 hash for the credential checker on the `nrf52840dk-test-invs`
# board. Also set the write_id to 0 meaning no storage permissions.
ELF2TAB_ARGS += --sha256 --write_id 0

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

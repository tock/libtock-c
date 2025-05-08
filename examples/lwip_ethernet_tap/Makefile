# Makefile for LwIP TAP network example application

# The default application stack and heap size allocations are not
# nearly sufficient for LwIP. While smaller limits may well work,
# these have been tested to work *shrug*.
STACK_SIZE=32768
APP_HEAP_SIZE=32768

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lwip

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

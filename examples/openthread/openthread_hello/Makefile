# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Specify this app depends on the MTD OpenThread library.
include $(TOCK_USERLAND_BASE_DIR)/libopenthread/libopenthread-mtd.mk

# set stack size to 5000 to support openthread app
STACK_SIZE:=5000

C_SRCS := main.c

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Specify this app depends on the MTD OpenThread library.
include $(TOCK_USERLAND_BASE_DIR)/libopenthread/libopenthread-mtd.mk

# set stack size to 8000 to support openthread app
STACK_SIZE:=8000
APP_HEAP_SIZE:=5000

PACKAGE_NAME = org.tockos.thread-tutorial.openthread

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

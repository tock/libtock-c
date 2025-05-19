# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

PACKAGE_NAME = app_loader# org.tockos.processes-tutorial.app_loader

# Which files to compile.
C_SRCS := $(wildcard *.c)

APP_HEAP_SIZE := 2000

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Change name of generated app
PACKAGE_NAME = udp-button-press 

APP_HEAP_SIZE := 2048

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

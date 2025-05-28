# Makefile for user application

# Limit things to cortex-m4 while we figure things out
TOCK_TARGETS := cortex-m4

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

override CFLAGS += -I../app_binaries/cortex-m4

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Include app loading support rules. Rules to generate app binary images.
include ../support/AppLoaderSupport.mk

build/cortex-m4/main.o: build/cortex-m4/adc.embed

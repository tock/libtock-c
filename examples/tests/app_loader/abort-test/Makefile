# Makefile for user application

# Restrict to ARM targets for the moment, as we don't have the infrastructure in
# place to enforce the placement rules for RISC-V binaries with dynamic loading
TOCK_TARGETS ?= cortex-m0 cortex-m3 cortex-m4 cortex-m7

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# List of apps to generate embed rules for
APPS_TO_EMBED := \
  $(TOCK_USERLAND_BASE_DIR)/examples/tests/adc/adc

# Include app loading support rules. Rules to generate app binary images.
include ../support/AppLoaderSupport.mk

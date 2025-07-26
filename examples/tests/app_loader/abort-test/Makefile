# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# List of apps to generate embed rules for
APPS_TO_EMBED := \
  $(TOCK_USERLAND_BASE_DIR)/examples/tests/adc/adc

# Include app loading support rules. Rules to generate app binary images.
include ../support/CommonAppMake.mk

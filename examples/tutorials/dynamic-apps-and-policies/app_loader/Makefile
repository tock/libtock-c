# Makefile for user application

# Restrict to Cortex M4 for the moment. Add new chip families as needed.
TOCK_TARGETS ?= cortex-m4

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

PACKAGE_NAME = app_loader# org.tockos.processes-tutorial.app_loader

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include an ECDSA signature in the compiled TBF.
ELF2TAB_ARGS += --ecdsa-nist-p256-private ../keys/ec-secp256r1-tutorial-key.private.p8

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# List of apps to generate embed rules for
APPS_TO_EMBED := \
  ../temperature \
  ../counter

# Include app loading support rules. Rules to generate app binary images.
include ../support/AppLoaderSupport.mk

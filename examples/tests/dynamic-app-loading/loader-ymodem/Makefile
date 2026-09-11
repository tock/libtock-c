# Makefile for user application

## Restrict to Cortex M4 for the moment. Add new chip families as needed.
#TOCK_TARGETS ?= cortex-m4

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include an ECDSA signature in the compiled TBF.
ELF2TAB_ARGS += --ecdsa-nist-p256-private ../../../tutorials/dynamic-apps-and-policies/keys/ec-secp256r1-tutorial-key.private.p8
TBF_NO_FOOTER = yes
ELF2TAB_ARGS += --minimum-footer-size 115652

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk


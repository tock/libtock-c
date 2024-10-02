# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

# Use the libtock-c Make system
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/RadioLib

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

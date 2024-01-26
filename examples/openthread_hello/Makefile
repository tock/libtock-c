# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Include needed header files for OpenThread library
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/include
CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/ot-tock/openthread/src/core

# Include external ot-tock static library
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/ot-tock
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock

# Arbitrarily set this for now. We will need to revisit this value as the default
# heap size will likely be too small for OpenThread.
APP_HEAP_SIZE:= 40000
STACK_SIZE:=40000

# The ot-tock static library is currently only compiled for cortex-m4. Likewise,
# we currently only build OpenThread applications for the cortex-m4 platform. 
TOCK_TARGETS:=cortex-m4

C_SRCS := main.c


# NOTE: Some documentation seemed to say that the AppMakefile.mk include 
# needed to be the last line this makefile. However, the scope has issues
# with building the platform-tock library if the order is flipped

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Include makefile for platform-tock library
include $(TOCK_USERLAND_BASE_DIR)/ot-tock/platform/platform-tock/Makefile

# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

#build_openthread:
#	cd $(TOCK_USERLAND_BASE_DIR)/openthread-tock && ./create_openthread-tock.sh

# Include needed header files for OpenThread library
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/include
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/src/core
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

# The openthread-tock static library is currently only compiled for cortex-m4. Likewise,
# we currently only build OpenThread applications for the cortex-m4 platform.    # This structure is somewhat cumbersome, but is necessary to follow libtock-c's library import system.
TOCK_TARGETS:=cortex-m4     

OT_TOCK_LIB_DIR += $(TOCK_USERLAND_BASE_DIR)/openthread-tock/lib

EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libopenthread
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libmbedcrypto
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libmbedx509
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libtcplp
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libmbedtls
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

# Arbitrarily set this for now. We will need to revisit these values.
#APP_HEAP_SIZE:=5000
STACK_SIZE:=5000

C_SRCS := main.c

#include $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock/Makefile

# NOTE: Some documentation seemed to say that the AppMakefile.mk include 
# needed to be the last line this makefile. However, the scope has issues
# with building the platform-tock library if the order is flipped

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

#include $(TOCK_USERLAND_BASE_DIR)/openthread-tock/Makefile

# Include makefile for platform-tock library
#include $(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock/Makefile


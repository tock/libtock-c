# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

OT_TOCK_LIB_DIR += $(TOCK_USERLAND_BASE_DIR)/libopenthread

EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libopenthread-mtd
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libtcplp
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/libmbedtls
EXTERN_LIBS += $(OT_TOCK_LIB_DIR)/platform

# Arbitrarily set this for now. We will need to revisit these values.
#APP_HEAP_SIZE:=5000
STACK_SIZE:=5000

C_SRCS := main.c

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

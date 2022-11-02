# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
C_SRCS := $(wildcard *.c) \
          $(wildcard ./nimble/host/pts/src/*.c) \
          $(wildcard ./nimble/host/services/src/*.c) \
          $(wildcard ./nimble/host/services/gap/src/*.c) \
          $(wildcard ./nimble/host/src/*.c) \
          $(wildcard ./nimble/host/store/src/*.c) \
          $(wildcard ./nimble/host/util/src/*.c) \
          $(wildcard ./nimble/transport/tock/src/*.c) \
          $(wildcard ./nimble/transport/src/transport.c) \
          $(wildcard ./porting/nimble/src/*.c) \
          $(wildcard ./porting/npl/tock/src/*.c) \


# And some NOT to compile.
C_SRCS := $(filter-out ./nimble/host/src/ble_gatts_lcl.c,$(C_SRCS))
C_SRCS := $(filter-out ./porting/nimble/src/hal_timer.c,$(C_SRCS))

$(info $(C_SRCS))

CPPFLAGS += -I./ \
            -I./nimble/include \
            -I./nimble/host/include \
            -I./nimble/host/util/include \
            -I./nimble/host/services/gap/include \
            -I./nimble/transport/include \
            -I./porting/npl/tock/include \
            -I./porting/nimble/include \



# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

TOCK_TARGETS := cortex-m0\
                cortex-m3\
                cortex-m4\
                cortex-m7\
                rv32imac|rv32imac.0x20040060.0x80002800|0x20040060|0x80002800\
                rv32imac|rv32imac.0x40430060.0x80004000|0x40430060|0x80004000\
                rv32imac|rv32imac.0x40440060.0x80007000|0x40440060|0x80007000\
                rv32imac|rv32imac.0x403B0060.0x3FCC0000|0x403B0060|0x3FCC0000\
                rv32imc|rv32imc.0x20030080.0x10005000|0x20030080|0x10005000\
                rv32imc|rv32imc.0x41000060.0x42008000|0x41000060|0x42008000

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/lvgl

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/lvgl

# Which files to compile.
C_SRCS += $(wildcard *.c)

# include $(TOCK_USERLAND_BASE_DIR)/lvgl/lvgl.mk

APP_HEAP_SIZE := 40000
STACK_SIZE := 4096

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Ensure the required submodule is present
lvgl_driver.c:	| ../../lvgl/lvgl/lvgl.h

../../lvgl/lvgl/lvgl.h:
	git submodule init ../../lvgl/lvgl
	git submodule update ../../lvgl/lvgl

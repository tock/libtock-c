# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

# Include the core RadioLib headers
override CPPFLAGS += -isystem $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -isystem $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock

# Include the base of libtock-c to fix the libtock/ includes from RadioLib
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/

ifeq ($(CMAKE),1)
# Use the RadioLib cmake build infrastructure
LIBS_cortex-m += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m0 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m3 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m4 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m7 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a

LIBS_rv32i += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a
LIBS_rv32imc += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a
LIBS_rv32imac += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

$(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a:
	cd $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/ && \
		rm -rf build-arm && \
		mkdir -p build-arm && cd build-arm && \
		LIBTOCK_C_DIRECTORY="$(TOCK_USERLAND_BASE_DIR)" cmake -G "CodeBlocks - Unix Makefiles" .. && \
		$(MAKE) -j4 2> /dev/null || true

$(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a:
	cd $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/examples/NonArduino/Tock/ && \
		rm -rf build-riscv && \
		mkdir -p build-riscv && cd build-riscv && \
		LIBTOCK_C_DIRECTORY="$(TOCK_USERLAND_BASE_DIR)" cmake -G "CodeBlocks - Unix Makefiles" -DRISCV_BUILD=1 .. && \
		$(MAKE) -j4 2> /dev/null || true
else
# Use the libtock-c Make system
LIBS_cortex-m0 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/cortex-m0/RadioLib.a
LIBS_cortex-m3 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/cortex-m3/RadioLib.a
LIBS_cortex-m4 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/cortex-m4/RadioLib.a
LIBS_cortex-m7 += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/cortex-m7/RadioLib.a

LIBS_rv32imc += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/rv32imc/RadioLib.a
LIBS_rv32imac += $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib/build/rv32imac/RadioLib.a

EXTERN_LIBS := $(TOCK_USERLAND_BASE_DIR)/libradio/RadioLib
include $(TOCK_USERLAND_BASE_DIR)/libradio/Makefile

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
endif

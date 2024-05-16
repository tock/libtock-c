# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

# Include the core RadioLib headers
override CPPFLAGS += -isystem ../RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -isystem ../RadioLib/examples/NonArduino/Tock

# Include the base of libtock-c to fix the libtock/ includes from RadioLib
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/

LIBS_cortex-m += ../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m0 += ../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m3 += ../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m4 += ../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a
LIBS_cortex-m7 += ../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a

LIBS_rv32i += ../RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a
LIBS_rv32imc += ../RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a
LIBS_rv32imac += ../RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

../RadioLib/examples/NonArduino/Tock/build-arm/RadioLib/libRadioLib.a:
	cd ../RadioLib/examples/NonArduino/Tock/ && \
		rm -rf build-arm && \
		mkdir -p build-arm && cd build-arm && \
		LIBTOCK_C_DIRECTORY="$(TOCK_USERLAND_BASE_DIR)/../../../" cmake -G "CodeBlocks - Unix Makefiles" .. && \
		$(MAKE) -j4 2> /dev/null || true

../RadioLib/examples/NonArduino/Tock/build-riscv/RadioLib/libRadioLib.a:
	cd ../RadioLib/examples/NonArduino/Tock/ && \
		rm -rf build-riscv && \
		mkdir -p build-riscv && cd build-riscv && \
		LIBTOCK_C_DIRECTORY="$(TOCK_USERLAND_BASE_DIR)/../../../" cmake -G "CodeBlocks - Unix Makefiles" -DRISCV_BUILD=1 .. && \
		$(MAKE) -j4 2> /dev/null || true

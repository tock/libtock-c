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

../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a:
	cd ../ && ./build-RadioLib.sh

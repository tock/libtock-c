# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../

# We only compile this for Cortex-M platforms because we only have
# libraries for Cortex-M.
TOCK_TARGETS := cortex-m0 cortex-m3 cortex-m4 cortex-m7

# Which files to compile.
CXX_SRCS := $(wildcard *.cc)

# Include the core RadioLib headers
override CPPFLAGS += -I../RadioLib/src

# Include the Tock specific headers
override CPPFLAGS += -I../RadioLib/examples/NonArduino/Tock

# Include the base of libtock-c to fix the libtock/ includes from RadioLib
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/

override LEGACY_LIBS_cortex-m += ../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a
override LEGACY_LIBS_cortex-m0 += ../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a
override LEGACY_LIBS_cortex-m3 += ../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a
override LEGACY_LIBS_cortex-m4 += ../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a
override LEGACY_LIBS_cortex-m7 += ../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

../RadioLib/examples/NonArduino/Tock/build/RadioLib/libRadioLib.a:
	cd ../ && ./build-RadioLib.sh

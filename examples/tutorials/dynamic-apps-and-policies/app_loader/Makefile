# Makefile for user application

# Restrict to Cortex M4 for the moment. Add new chip families as needed.
TOCK_TARGETS ?= cortex-m4

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

PACKAGE_NAME = app_loader# org.tockos.processes-tutorial.app_loader

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include an ECDSA signature in the compiled TBF.
ELF2TAB_ARGS += --ecdsa-nist-p256-private ../keys/ec-secp256r1-tutorial-key.private.p8

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# List of apps to generate embed rules for
APPS_TO_EMBED := \
  ../temperature \
  ../counter \
  ../../../../examples/blink \

AUTOGEN_HEADER = autogen_binaries.h
main.o: autogen_binaries.h

$(AUTOGEN_HEADER): $(APPS_TO_EMBED)
	@echo "// `app_loader`'s makefile autogenerates this file. Do not edit manually." > $(AUTOGEN_HEADER)
	@echo "#pragma once" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		echo "#include \"$$app.xxd\"" >> $(AUTOGEN_HEADER); \
		upper=$$(echo $$app | tr a-z A-Z); \
		echo "#define APP_$$upper $${app}_embed" >> $(AUTOGEN_HEADER); \
	done
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "const uint8_t* binaries[] = {" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		upper=$$(echo $$app | tr a-z A-Z); \
		echo "  APP_$$upper," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "size_t binary_sizes[] = {" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		upper=$$(echo $$app | tr a-z A-Z); \
		echo "  sizeof(APP_$$upper)," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "const char* binary_names[] = {" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		echo "  \"$$app\"," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)

# Include app loading support rules. Rules to generate app binary images.
include ../support/AppLoaderSupport.mk

# Makefile for user application

.DEFAULT_GOAL = all

# Restrict to Cortex M4 for the moment. Add new chip families as needed.
TOCK_TARGETS ?= cortex-m4

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../../..

PACKAGE_NAME = app_loader

# Which files to compile.
C_SRCS := $(wildcard *.c)

# Include an ECDSA signature in the compiled TBF.
ELF2TAB_ARGS += --ecdsa-nist-p256-private ../keys/ec-secp256r1-tutorial-key.private.p8

# List of apps to generate embed rules for:
APPS_TO_EMBED := \
	# $(TOCK_USERLAND_BASE_DIR)/examples/blink \

BINARY_COUNT = $(words $(APPS_TO_EMBED))
AUTOGEN_HEADER = loadable_binaries.h
main.c: $(AUTOGEN_HEADER)


build:
	$(TRACE_DIR)
	@mkdir -p $@

# Ensure we rebuild if `APPS_TO_EMBED` changes.
.PHONY: FORCE
define DEPENDABLE_VAR
build/$(1): build
	@printf '%s' "$($(1))" > build/$(1)
ifneq ("$(shell cat build/$(1) 2>/dev/null)","$($(1))")
build/$(1): FORCE
endif
endef
$(eval $(call DEPENDABLE_VAR,APPS_TO_EMBED))

$(AUTOGEN_HEADER): build/APPS_TO_EMBED
	@echo "// \`app_loader\`'s makefile autogenerates this file. Do not edit manually." > $(AUTOGEN_HEADER)
	@echo "#pragma once" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "#define BINARY_COUNT ${BINARY_COUNT}" >> $(AUTOGEN_HEADER)
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
		echo "	$${app}_embed_size," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "const char* binary_names[] = {" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		echo "  \"$$app\"," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)
	@echo "" >> $(AUTOGEN_HEADER)
	@echo "size_t actual_sizes[] = {" >> $(AUTOGEN_HEADER)
	@for app in $(notdir $(APPS_TO_EMBED)); do \
		echo "	$${app}_embed_actual_size," >> $(AUTOGEN_HEADER); \
	done
	@echo "};" >> $(AUTOGEN_HEADER)

# Include userland master makefile. Contains rules and flags for actually
# building the application.
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk

# Include app loading support rules. Rules to generate app binary images.
include ../support/AppLoaderSupport.mk

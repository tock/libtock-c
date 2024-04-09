# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libmbedtls
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libmbedtls
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/third_party/mbedtls

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/repo/library/*.c)

override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/../../src/core
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/../../include
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/repo/include
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)
override CPPFLAGS += -I$($(LIBNAME)_DIR)

# Include our standard OpenThread configuration to ensure all flags are set
# consistently.
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock
override CPPFLAGS += -DOPENTHREAD_PLATFORM_CORE_CONFIG_FILE=\"openthread-core-tock-config.h\"

# Include our config file for mbedtls which makes one minor modification.
override CPPFLAGS += -DMBEDTLS_CONFIG_FILE=\"libtock-mbedtls-config.h\"

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error
override CPPFLAGS_$(LIBNAME) += -Wno-unused-macros -Wno-redundant-decls

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

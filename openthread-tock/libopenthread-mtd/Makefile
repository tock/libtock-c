# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libopenthread-mtd
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libopenthread-mtd
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/api/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/backbone_router/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/border_router/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/coap/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/common/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/crypto/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/diags/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/instance/instance.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/mac/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/meshcop/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/net/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/radio/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/thread/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_SRC_ROOT)/src/core/utils/*.cpp)

override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/src/core
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/include
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/third_party/mbedtls/repo/include
override CPPFLAGS += -I$($(LIBNAME)_SRC_ROOT)/third_party/mbedtls
# Need to include the folder where `openthread-core-tock-config.h` is located.
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/openthread-tock/platform/platform-tock

# Package settings for the Tock version of OpenThread.
override CPPFLAGS += -DPACKAGE_NAME=\"tock-openthread-mtd\"
override CPPFLAGS += -DPACKAGE_VERSION=\"0.1\"
# Specify we want a minimal thread device build of the library.
override CPPFLAGS += -DOPENTHREAD_MTD=1
# Configuration header files we need to specify.
override CPPFLAGS += -DOPENTHREAD_PLATFORM_CORE_CONFIG_FILE=\"openthread-core-tock-config.h\"
override CPPFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error
# Ignore warnings that openthread seems to not care about.
override CPPFLAGS_$(LIBNAME) += -Wno-useless-cast -Wno-old-style-cast
override CPPFLAGS_$(LIBNAME) += -Wno-missing-declarations -Wno-date-time
override CPPFLAGS_$(LIBNAME) += -Wno-zero-as-null-pointer-constant
override CPPFLAGS_$(LIBNAME) += -Wno-suggest-attribute=format -Wno-redundant-decls

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libopenthread-ftd
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/libopenthread/libopenthread-ftd
$(LIBNAME)_SRC_ROOT := $(TOCK_USERLAND_BASE_DIR)/libopenthread/openthread

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

override CPPFLAGS_$(LIBNAME) += -I$($(LIBNAME)_SRC_ROOT)/src/core
override CPPFLAGS_$(LIBNAME) += -I$($(LIBNAME)_SRC_ROOT)/include
override CPPFLAGS_$(LIBNAME) += -I$($(LIBNAME)_SRC_ROOT)/third_party/mbedtls/repo/include
override CPPFLAGS_$(LIBNAME) += -I$($(LIBNAME)_SRC_ROOT)/third_party/mbedtls
# Need to include the folder where `openthread-core-tock-config.h` is located.
override CPPFLAGS_$(LIBNAME) += -I$(TOCK_USERLAND_BASE_DIR)/libopenthread/platform

# Package settings for the Tock version of OpenThread.
override CPPFLAGS_$(LIBNAME) += -DPACKAGE_NAME=\"tock-openthread-ftd\"
override CPPFLAGS_$(LIBNAME) += -DPACKAGE_VERSION=\"0.1\"
# Specify we want a minimal thread device build of the library.
override CPPFLAGS_$(LIBNAME) += -DOPENTHREAD_FTD=1
# Configuration header files we need to specify.
override CPPFLAGS_$(LIBNAME) += -DOPENTHREAD_PLATFORM_CORE_CONFIG_FILE=\"openthread-core-tock-config.h\"
override CPPFLAGS_$(LIBNAME) += -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error
# Ignore warnings that openthread seems to not care about.
override CPPFLAGS_$(LIBNAME) += -Wno-useless-cast -Wno-old-style-cast
override CPPFLAGS_$(LIBNAME) += -Wno-missing-declarations -Wno-date-time
override CPPFLAGS_$(LIBNAME) += -Wno-zero-as-null-pointer-constant
override CPPFLAGS_$(LIBNAME) += -Wno-suggest-attribute=format -Wno-redundant-decls
override CPPFLAGS_$(LIBNAME) += -Wno-suggest-attribute=noreturn

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk


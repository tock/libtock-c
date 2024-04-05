# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libopenthread-ftd
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libopenthread-ftd
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

override CPPFLAGS += -DPACKAGE_NAME=\"tock-openthread-ftd\"
override CPPFLAGS += -DPACKAGE_VERSION=\"0.1\"
override CPPFLAGS += -DOPENTHREAD_FTD=1
override CPPFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

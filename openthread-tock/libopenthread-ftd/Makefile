# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libopenthread-ftd
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libopenthread-ftd
LIB_SRC_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/api/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/backbone_router/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/border_router/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/coap/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/common/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/crypto/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/diags/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/instance/instance.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/mac/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/meshcop/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/net/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/radio/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/thread/*.cpp)
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/utils/*.cpp)

override CPPFLAGS += -I$(LIB_SRC_DIR)/src/core
override CPPFLAGS += -I$(LIB_SRC_DIR)/include
override CPPFLAGS += -I$(LIB_SRC_DIR)/third_party/mbedtls/repo/include
override CPPFLAGS += -I$(LIB_SRC_DIR)/third_party/mbedtls

override CPPFLAGS += -DPACKAGE_NAME=\"tock-openthread-ftd\"
override CPPFLAGS += -DPACKAGE_VERSION=\"0.1\"
override CPPFLAGS += -DOPENTHREAD_FTD=1
override CPPFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\"

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

.PHONY: default
default: all ;

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ../..
LIBNAME := libmbedtls
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/libmbedtls
LIB_SRC_DIR := $(TOCK_USERLAND_BASE_DIR)/openthread-tock/openthread/third_party/mbedtls

# List all C and Assembly files
$(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/repo/library/*.c)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/api/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/backbone_router/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/border_router/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/coap/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/common/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/crypto/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/diags/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/instance/instance.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/mac/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/meshcop/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/net/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/radio/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/thread/*.cpp)
# $(LIBNAME)_SRCS  += $(wildcard $(LIB_SRC_DIR)/src/core/utils/*.cpp)


# # Need libtock headers
# override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock
override CPPFLAGS += -I$(LIB_SRC_DIR)/../../src/core
override CPPFLAGS += -I$(LIB_SRC_DIR)/../../include
override CPPFLAGS += -I$(LIB_SRC_DIR)/repo/include
override CPPFLAGS += -I$($(LIBNAME)_DIR)

# override CPPFLAGS += -DPACKAGE_NAME=\"tockot\"
# override CPPFLAGS += -DPACKAGE_VERSION=\"0.1\"
# override CPPFLAGS += -DOT_MTD=OFF -DOT_FTD=ON
# override CPPFLAGS += -DOPENTHREAD_MTD=1
override CPPFLAGS += -DMBEDTLS_CONFIG_FILE=\"libtock-mbedtls-config.h\"

override CPPFLAGS += -DOPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS=1
override CPPFLAGS += -DOPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS_MANAGEMENT=1
override CPPFLAGS += -DOPENTHREAD_CONFIG_JOINER_ENABLE=0
override CPPFLAGS += -DOPENTHREAD_CONFIG_NCP_HDLC_ENABLE=1
override CPPFLAGS += -DOPENTHREAD_CONFIG_PING_SENDER_ENABLE=0
override CPPFLAGS += -DOPENTHREAD_CONFIG_THREAD_VERSION=OT_THREAD_VERSION_1_3

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk










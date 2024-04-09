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
override CPPFLAGS_$(LIBNAME) += -Wno-unused-macros -Wno-redundant-decls

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

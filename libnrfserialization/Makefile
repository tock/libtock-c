# Git hash of the library to use
VERSION_HASH := f813b5dbfef3b7a71044bca72e2ab8b3c4d9e593

# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libnrfserialization
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)
$(LIBNAME)_SRC_ROOT := $($(LIBNAME)_DIR)
LIB_SRC_DIR := $($(LIBNAME)_SRC_ROOT)/nrf5x-base-$(VERSION_HASH)

# Settings and config for the softdevice/sdk.
SDK_DIR_NAME := nrf51_sdk_11.0.0
SOFTDEVICE_MODEL := s130
DEVICE := SAM4L
SOFTDEVICE_MODEL := s130
SDK_VERSION = 11
BOARD ?= BOARD_CUSTOM

# Location for BLE Address if stored in Flash
BLEADDR_FLASH_LOCATION ?= 0x0007FFF8

# How many central/peripherals are defined changes how much memory the
# softdevice requires. Change the amount of memory allotted in a custom ld
# file if your configuration is different than default.

ifeq ($(RAM_KB), 16)
  # limit 16 kB RAM nRFs to only act as peripherals. Doing otherwise
  # requires careful balancing of memory requirements and should be done
  # manually, not automatically
  CENTRAL_LINK_COUNT ?= 0
  PERIPHERAL_LINK_COUNT ?= 1
else
  CENTRAL_LINK_COUNT ?= 1
  PERIPHERAL_LINK_COUNT ?= 1
endif

# List all C files
$(LIBNAME)_SRCS += $(wildcard $($(LIBNAME)_DIR)/*.c)

$(LIBNAME)_SRCS += $(LIB_SRC_DIR)/lib/simple_ble.c
$(LIBNAME)_SRCS += $(LIB_SRC_DIR)/advertisement/simple_adv.c
$(LIBNAME)_SRCS += $(LIB_SRC_DIR)/advertisement/eddystone.c
$(LIBNAME)_SRCS += $(LIB_SRC_DIR)/services/device_info_service.c

$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/codecs/$(SOFTDEVICE_MODEL)/serializers/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/codecs/$(SOFTDEVICE_MODEL)/middleware/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/transport/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/struct_ser/$(SOFTDEVICE_MODEL)/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/common/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_db_discovery/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_hrs_c/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_bas_c/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_dis/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/mailbox/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/util/app_error.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/scheduler/*.c)
$(LIBNAME)_SRCS += $(wildcard $(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/trace/*.c)

# Include directories
override CPPFLAGS += -I$($(LIBNAME)_DIR)
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/common/softdevice_handler/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/$(SOFTDEVICE_MODEL)/headers/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/ser_phy
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/ser_phy/config
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/codecs/common/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/hal/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/transport/

CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/device/

CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/timer/
CPPFLAGS_$(LIBNAME) += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/util/

# Defines for the nrf sdk
CPPFLAGS_$(LIBNAME) += -g -D$(DEVICE) -D$(BOARD) -std=c11 -Os -DSDK_VERSION_$(SDK_VERSION) -DSOFTDEVICE_$(SOFTDEVICE_MODEL)
CPPFLAGS_$(LIBNAME) += -D__TOCK__
CPPFLAGS_$(LIBNAME) += -DSVCALL_AS_NORMAL_FUNCTION
CPPFLAGS_$(LIBNAME) += -DBLE_STACK_SUPPORT_REQD
CPPFLAGS_$(LIBNAME) += -DSOFTDEVICE_PRESENT -DCENTRAL_LINK_COUNT=$(CENTRAL_LINK_COUNT) -DPERIPHERAL_LINK_COUNT=$(PERIPHERAL_LINK_COUNT)
CPPFLAGS_$(LIBNAME) += -DBLEADDR_FLASH_LOCATION=$(BLEADDR_FLASH_LOCATION)

# Need libtock headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

# Avoid failing in CI due to warnings in the library.
CPPFLAGS_$(LIBNAME) += -Wno-error
CPPFLAGS_$(LIBNAME) += -Wno-implicit-function-declaration

# Include the rules to build the library.
include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

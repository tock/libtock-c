LIBNAME := libnrfserialization
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

VERSION_HASH := f813b5dbfef3b7a71044bca72e2ab8b3c4d9e593
LIB_SRC_DIR := $($(LIBNAME)_DIR)/nrf5x-base-$(VERSION_HASH)

# So it doesn't think it's on the nRF and try to include nRF code.
override CFLAGS += -D__TOCK__
override CFLAGS += -DSVCALL_AS_NORMAL_FUNCTION
override CFLAGS += -DSOFTDEVICE_s130


SDK_DIR_NAME := nrf51_sdk_11.0.0
SOFTDEVICE_MODEL := s130
SDK_VERSION = 11


# INCLUDE DIRECTORIES
override CFLAGS += -I$($(LIBNAME)_DIR)

override CFLAGS += -I$(LIB_SRC_DIR)/advertisement/
override CFLAGS += -I$(LIB_SRC_DIR)/lib/

override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/common/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_db_discovery/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_hrs_c/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_bas_c/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/ble/ble_services/ble_dis/

override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/common/softdevice_handler/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/$(SOFTDEVICE_MODEL)/headers/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/softdevice/$(SOFTDEVICE_MODEL)/headers/nrf51

override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/ser_phy
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/common/transport/ser_phy/config
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/codecs/common/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/hal/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/serialization/application/transport/

override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/device/

override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/mailbox/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/timer/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/util/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/scheduler/
override CFLAGS += -I$(LIB_SRC_DIR)/sdk/$(SDK_DIR_NAME)/components/libraries/trace/

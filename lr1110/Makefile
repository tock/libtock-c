TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lr1110
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

LR1110_DIR = $(TOCK_USERLAND_BASE_DIR)/lr1110/lr1110
SEEED_DIR = $(LR1110_DIR)/seeed

ifeq ($(wildcard $(SEEED_DIR)/.*),)
    $(error Error: $(SEEED_DIR) directory does not exist. Please run `make -f Makefile.setup` first)
endif

# include changed source files
CHANGED_CSRCS := $(wildcard $(LR1110_DIR)/src_changed/*.c)

# include changed headers
override CFLAGS += -I$(LR1110_DIR)/inc_changed

# -DREGION_US_915: Defines the LoRaWAN region to US 915 MHz band
# -DRP2_103: Specifies the use of Regional Parameters version RP2-1.0.3
# -DTASK_EXTENDED_2: The LoRa Basics Modem extended uplink ID to be used for Wi-Fi uplinks
# -DLR11XX, -DLR11XX_TRANSCEIVER: Let the code select methods for LR11XX radio board 
override CFLAGS += -DREGION_US_915 -DRP2_103 -DTASK_EXTENDED_2 -DLR11XX -DLR11XX_TRANSCEIVER

# include unchanged source files from seeed's repo
UNCHANGED_CSRCS := \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/soft_secure_element/aes.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/alc_sync/alc_sync.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/almanac_update/almanac_update.c \
$(SEEED_DIR)/apps/common/apps_utilities.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/soft_secure_element/cmac.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/device_management/dm_downlink.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/fifo_ctrl.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/file_upload/file_upload.c \
$(SEEED_DIR)/geolocation_middleware/gnss/src/gnss_helpers.c \
$(SEEED_DIR)/geolocation_middleware/gnss/src/gnss_middleware.c \
$(SEEED_DIR)/geolocation_middleware/gnss/src/gnss_queue.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lorawan_api/lorawan_api.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/lorawan_certification.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_bootloader.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_crypto_engine.c \
$(SEEED_DIR)/geolocation_middleware/gnss/src/lr11xx_driver_extension.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_driver_version.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_gnss.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_lr_fhss.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_radio.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_radio_timings.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_regmem.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_wifi.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1_stack_mac_layer.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_class_c/lr1mac_class_c.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_core.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_utilities.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/device_management/modem_context.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_supervisor/modem_supervisor.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/modem_utilities.c \
$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/common/src/mw_bsp.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_planner/src/radio_planner_hal.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_ral/src/ral_lr11xx.c \
$(SEEED_DIR)/wm1110/LR11XX/common/src/ral_lr11xx_bsp.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_ralf/src/ralf_lr11xx.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/stream/rose.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_class_b/smtc_beacon_sniff.c \
$(SEEED_DIR)/wm1110/LR11XX/common/src/smtc_board_lr11xx.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/smtc_clock_sync.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/services/smtc_duty_cycle.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/services/smtc_lbt.c \
$(SEEED_DIR)/apps/common/smtc_modem_api_str.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/smtc_modem_crypto.c \
$(SEEED_DIR)/smtc_hal/smtc_modem_hal.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/smtc_modem_services_hal.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/services/smtc_multicast.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_class_b/smtc_ping_slot.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/smtc_real/src/smtc_real.c \
$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/LR1110/LR1110MB1DxS/smtc_shield_lr1110mb1dxs.c \
$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/LR1110/LR1110MB1GxS/smtc_shield_lr1110mb1gxs.c \
$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/common/src/smtc_shield_lr11x0_common.c \
$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/common/src/smtc_shield_lr11xx_common.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/soft_secure_element/soft_se.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/stream/stream.c \
$(SEEED_DIR)/geolocation_middleware/common/mw_common.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/lr11xx_system.c \
$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_core/smtc_modem_test.c \

# include unchanged headers from seeed's repo
override CFLAGS += \
-I$(SEEED_DIR)/apps/common/ \
-I$(SEEED_DIR)/geolocation_middleware/gnss/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/device_management/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lorawan_api/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_class_b/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/lr1mac_class_c/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/services/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac/src/smtc_real/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_services/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_supervisor/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_drivers/lr11xx_driver/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/radio_planner/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/soft_secure_element/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/alc_sync/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/almanac_update/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/file_upload/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src/stream/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_ral/src/ \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_ralf/src/ \
-I$(SEEED_DIR)/wm1110/LR11XX/common/src/ \
-I$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/LR1110/LR1110MB1DxS/ \
-I$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/LR1110/LR1110MB1GxS/ \
-I$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/common/src/ \
-I$(SEEED_DIR)/lora_basics_modem \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/headers \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_services/src \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/modem_config \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/smtc_modem_crypto/smtc_secure_element \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_core/lr1mac \
-I$(SEEED_DIR)/geolocation_middleware/common \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_api \
-I$(SEEED_DIR)/lora_basics_modem/smtc_modem_hal \
-I$(SEEED_DIR)/geolocation_middleware/bsp \
-I$(SEEED_DIR)/geolocation_middleware/wifi/src \
-I$(SEEED_DIR)/wm1110/LR11XX/smtc_lr11xx_board \
-I$(SEEED_DIR)/wm1110/LR11XX/smtc_shield_lr11xx/common/inc/ \
-I$(SEEED_DIR)/wm1110/LR11XX/radio_drivers_hal \
-I$(SEEED_DIR)/wm1110/LR11XX/common/inc \
-I$(SEEED_DIR)/wm1110/interface \
-I$(SEEED_DIR)/smtc_hal/inc

$(LIBNAME)_SRCS := $(UNCHANGED_CSRCS) $(CHANGED_CSRCS)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
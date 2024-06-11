LR1110_DIR = $(TOCK_USERLAND_BASE_DIR)/lr1110/lr1110
SEEED_DIR = $(LR1110_DIR)/seeed

# include changed headers and parameters
override CFLAGS += -I$(LR1110_DIR)/inc_changed -DREGION_US_915 -DRP2_103 -DTASK_EXTENDED_2

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
-I$(SEEED_DIR)/smtc_hal/ \
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
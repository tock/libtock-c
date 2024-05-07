TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := wm1110
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

WM1110_DIR = $(TOCK_USERLAND_BASE_DIR)/wm1110/wm1110
SEEED_DIR = $(WM1110_DIR)/seeed

ifeq ($(wildcard $(SEEED_DIR)/.*),)
    $(error Error: $(SEEED_DIR) directory does not exist. Please run `make -f Makefile.setup` first)
endif


# list of names of unchanged files from Seeed Studio 
C_UNCHANGED_FILES := aes.c alc_sync.c almanac_update.c apps_utilities.c cmac.c \
dm_downlink.c fifo_ctrl.c file_upload.c gnss_helpers.c gnss_middleware.c gnss_queue.c \
lorawan_api.c lorawan_certification.c lr11xx_bootloader.c lr11xx_ce.c \
lr11xx_crypto_engine.c lr11xx_driver_extension.c lr11xx_driver_version.c \
lr11xx_gnss.c lr11xx_lr_fhss.c lr11xx_radio.c lr11xx_radio_timings.c lr11xx_regmem.c \
lr11xx_wifi.c lr1_stack_mac_layer.c lr1mac_class_c.c lr1mac_core.c lr1mac_utilities.c \
modem_context.c modem_supervisor.c modem_utilities.c mw_bsp.c radio_planner_hal.c \
ral_lr11xx.c ral_lr11xx_bsp.c ralf_lr11xx.c rose.c smtc_beacon_sniff.c smtc_board_lr11xx.c \
smtc_clock_sync.c smtc_duty_cycle.c smtc_lbt.c smtc_modem_api_str.c smtc_modem_crypto.c \
smtc_modem_hal.c smtc_modem_services_hal.c smtc_multicast.c smtc_ping_slot.c smtc_real.c \
smtc_shield_lr1110mb1dxs.c smtc_shield_lr1110mb1gxs.c smtc_shield_lr11x0_common.c \
smtc_shield_lr11xx_common.c soft_se.c stream.c

# generate paths after searching with filenames, add all paths to CSRCS
UNCHANGED_CSRCS := $(foreach file,$(C_UNCHANGED_FILES),$(shell find $(SEEED_DIR) -type f -name $(file)))

# include changed headers and parameters
override CFLAGS += -I$(WM1110_DIR)/inc_changed -DREGION_US_915 -DRP2_103 -DTASK_EXTENDED_2

# modify functions from TockLibrary.mk to include headers into C flags, not CPP flags
UNCHANGED_C_DIRS := $(sort $(dir $(UNCHANGED_CSRCS)))

define HEADER_C_INCLUDES
ifneq ($$(wildcard $(1)/*.h),"")
  override CFLAGS += -I$(1)
endif
ifneq ($$(wildcard $(1)/include/*.h),"")
  override CFLAGS += -I$(1)include
endif
ifneq ($$(wildcard $(1)/../include/*.h),"")
  override CFLAGS += -I$(1)../include
endif
endef

$(foreach hdrdir,$(UNCHANGED_C_DIRS),$(eval $(call HEADER_C_INCLUDES,$(hdrdir))))

# manually add headers that the function did not cover
override CFLAGS += \
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


override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock
override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

# add changed source files
CHANGED_CSRCS := $(wildcard $(WM1110_DIR)/src_changed/*.c)
CSRCS := $(UNCHANGED_CSRCS) $(CHANGED_CSRCS)

# List all C and Assembly files
$(LIBNAME)_SRCS  := $(CSRCS)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
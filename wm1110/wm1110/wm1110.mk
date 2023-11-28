CSRCS += smtc_hal_gpio.c
CSRCS += smtc_hal_spi.c
CSRCS += lr11xx_hal.c
CSRCS += lr11xx_system.c
CSRCS += alarm_timer.c
CSRCS += alarm_internal.c
CSRCS += tock.c
CSRCS += lora_phy.c
CSRCS += tock_gpio.c

DEPPATH += --dep-path $(WM1110_DIR)/$(WM1110_DIR_NAME)/src
VPATH += :$(WM1110_DIR)/$(WM1110_DIR_NAME)/src

CFLAGS += "-I$(WM1110_DIR)/$(WM1110_DIR_NAME)/src"

# run this file to make library, and then write example apps under example folder, include this library, to make app?
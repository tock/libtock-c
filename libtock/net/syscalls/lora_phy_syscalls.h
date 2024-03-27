#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_LORA_PHY_SPI  0x30003
#define DRIVER_NUM_LORA_PHY_GPIO 0x30004

bool libtock_lora_phy_exists(void);

returncode_t libtock_lora_phy_set_upcall_spi(subscribe_upcall callback, void* opaque);

returncode_t libtock_lora_phy_set_readonly_allow_master_write_buffer(const uint8_t* buffer, uint32_t len);

returncode_t libtock_lora_phy_set_readwrite_allow_master_read_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_lora_phy_command_read_write(uint32_t length);

returncode_t libtock_lora_phy_gpio_command_enable_output(uint32_t pin);

returncode_t libtock_lora_phy_gpio_command_set(uint32_t pin);

returncode_t libtock_lora_phy_gpio_command_clear(uint32_t pin);

returncode_t libtock_lora_phy_gpio_command_toggle(uint32_t pin);

returncode_t libtock_lora_phy_gpio_command_enable_input(uint32_t pin, uint32_t pin_config);

returncode_t libtock_lora_phy_gpio_command_read(uint32_t pin, uint32_t* pin_value);

returncode_t libtock_lora_phy_gpio_command_enable_interrupt(uint32_t pin, uint32_t irq_config);

returncode_t libtock_lora_phy_gpio_command_disable_interrupt(uint32_t pin);

returncode_t libtock_lora_phy_gpio_command_interrupt_callback(subscribe_upcall callback, void* opaque);

#ifdef __cplusplus
}
#endif

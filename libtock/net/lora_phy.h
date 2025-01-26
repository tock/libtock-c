#pragma once

#include "../peripherals/gpio.h"
#include "../tock.h"
#include "syscalls/lora_phy_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for a LoRa PHY SPI callback.
//
// - `arg1` (`returncode_t`): Status from doing the SPI transaction.
typedef void (*libtock_lora_phy_callback_spi)(returncode_t);

returncode_t libtock_lora_phy_write(const uint8_t*                buf,
                                    uint32_t                      len,
                                    libtock_lora_phy_callback_spi cb);

returncode_t libtock_lora_phy_read_write(const uint8_t*                write,
                                         uint8_t*                      read,
                                         uint32_t                      len,
                                         libtock_lora_phy_callback_spi cb);

returncode_t libtock_lora_phy_gpio_enable_output(uint32_t pin);
returncode_t libtock_lora_phy_gpio_set(uint32_t pin);
returncode_t libtock_lora_phy_gpio_clear(uint32_t pin);
returncode_t libtock_lora_phy_gpio_toggle(uint32_t pin);
returncode_t libtock_lora_phy_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config);
returncode_t libtock_lora_phy_gpio_read(uint32_t pin, int* pin_value);
returncode_t libtock_lora_phy_gpio_enable_interrupt(uint32_t pin, libtock_gpio_interrupt_mode_t irq_config);
returncode_t libtock_lora_phy_gpio_disable_interrupt(uint32_t pin);
returncode_t libtock_lora_phy_gpio_set_callback(libtock_gpio_callback_interrupt cb);

#ifdef __cplusplus
}
#endif

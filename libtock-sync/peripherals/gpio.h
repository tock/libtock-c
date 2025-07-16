#pragma once

#include <libtock/peripherals/gpio.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_gpio_exists(void);

// Configure a GPIO pin as an input and then wait until a rising interrupt
// occurs.
returncode_t libtocksync_gpio_wait_until_high(uint32_t pin, libtock_gpio_input_mode_t pin_config);

// Configure a GPIO pin as an input and then wait until a falling interrupt
// occurs.
returncode_t libtocksync_gpio_wait_until_low(uint32_t pin, libtock_gpio_input_mode_t pin_config);

// Configure a GPIO pin as an input and then wait until the pin level changes.
returncode_t libtocksync_gpio_wait_until_changed(uint32_t pin, libtock_gpio_input_mode_t pin_config);

#ifdef __cplusplus
}
#endif

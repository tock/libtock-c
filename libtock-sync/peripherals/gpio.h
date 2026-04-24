#pragma once

#include <libtock/peripherals/gpio_types.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_gpio_exists(void);

// Returns the number of GPIO pins configured on the board.
returncode_t libtocksync_gpio_count(int* count);

// Configure a GPIO pin as an output.
returncode_t libtocksync_gpio_enable_output(uint32_t pin);

// Configure a GPIO pin as an input.
returncode_t libtocksync_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config);

// Set an output pin high.
returncode_t libtocksync_gpio_set(uint32_t pin);

// Set an output pin low.
returncode_t libtocksync_gpio_clear(uint32_t pin);

// Toggle an output pin.
returncode_t libtocksync_gpio_toggle(uint32_t pin);

// Read the current state of a GPIO input pin.
returncode_t libtocksync_gpio_read(uint32_t pin, int* pin_value);

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

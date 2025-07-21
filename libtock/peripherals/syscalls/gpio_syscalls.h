#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_DRIVER_NUM 0x4

// Check if the GPIO driver is available in the kernel.
bool libtock_gpio_driver_exists(void);

// Set the upcall for getting GPIO pin interrupts.
returncode_t libtock_gpio_set_upcall(subscribe_upcall callback, void* opaque);

// Get the number of GPIO pins available on the board.
returncode_t libtock_gpio_command_count(uint32_t* count);

// Set a pin as an output.
returncode_t libtock_gpio_command_enable_output(uint32_t pin);

// Set a pin as an input with an optional pull-up/down.
returncode_t libtock_gpio_command_enable_input(uint32_t pin, uint32_t pin_config);

// Set an output pin high.
returncode_t libtock_gpio_command_set(uint32_t pin);

// Set an output pin low.
returncode_t libtock_gpio_command_clear(uint32_t pin);

// Toggle an output pin.
returncode_t libtock_gpio_command_toggle(uint32_t pin);

// Read the level of an input pin.
returncode_t libtock_gpio_command_read(uint32_t pin, uint32_t* pin_value);

// Enable interrupts on an input pin.
returncode_t libtock_gpio_command_enable_interrupt(uint32_t pin, uint32_t irq_config);

// Disable interrupts on an input pin.
returncode_t libtock_gpio_command_disable_interrupt(uint32_t pin);

// Disable a pin and minimize the power draw from the pin.
returncode_t libtock_gpio_command_disable(uint32_t pin);

#ifdef __cplusplus
}
#endif

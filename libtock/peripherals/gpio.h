#pragma once

#include "../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for gpio interrupts
//
// - `arg1` (`uint32_t`): Pin number of the pin that interrupted.
// - `arg2` (`bool`): If the value is high (true) or low (false).
typedef void (*libtock_gpio_callback_interrupt)(uint32_t, bool);

typedef enum {
  libtock_pull_none=0,
  libtock_pull_up,
  libtock_pull_down,
} libtock_gpio_input_mode_t;

typedef enum {
  libtock_change=0,
  libtock_rising_edge,
  libtock_falling_edge,
} libtock_gpio_interrupt_mode_t;

// Check if the driver exists.
bool libtock_gpio_exists(void);

// Returns the number of GPIO pins configured on the board.
returncode_t libtock_gpio_count(int* count);

// Set the callback function that is called when a GPIO interrupt fires.
returncode_t libtock_gpio_set_interrupt_callback(libtock_gpio_callback_interrupt cb);

// Set the pin as an input.
//
// `pin_config` configures the pin with a pull up, pull down, or neither.
returncode_t libtock_gpio_enable_input(uint32_t pin, libtock_gpio_input_mode_t pin_config);

// Set the pin as an output.
returncode_t libtock_gpio_enable_output(uint32_t pin);

// Set the pin high.
returncode_t libtock_gpio_set(uint32_t pin);

// Set the pin low.
returncode_t libtock_gpio_clear(uint32_t pin);

// Toggle the pin.
returncode_t libtock_gpio_toggle(uint32_t pin);

// Read the state of an input pin.
returncode_t libtock_gpio_read(uint32_t pin, int* pin_value);

// Enable interrupts for the pin.
//
// The callback function needs to be set via
// `libtock_gpio_set_interrupt_callback()`.
returncode_t libtock_gpio_enable_interrupt(uint32_t pin, libtock_gpio_interrupt_mode_t irq_config);

// Disable interrupts for the pin.
returncode_t libtock_gpio_disable_interrupt(uint32_t pin);

// Completely disable the pin allowing it to go into its lowest power mode.
returncode_t libtock_gpio_disable(uint32_t pin);

#ifdef __cplusplus
}
#endif

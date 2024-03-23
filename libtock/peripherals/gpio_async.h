#pragma once

#include "gpio.h"
#include "../tock.h"
#include "syscalls/gpio_async_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signature for async gpio interrupts
//
// - `arg1` (`uint32_t`): Port number of the pin that interrupted.
// - `arg2` (`uint32_t`): Pin number of the pin that interrupted.
// - `arg3` (`bool`): If the value is high (true) or low (false).
typedef void (*libtock_gpio_async_callback_interrupt)(uint32_t, uint32_t, bool);

// Function signature for GPIO async callback.
//
// - `arg1` (`returncode_t`): Status of the command operation.
// - `arg2` (`bool`): On a read if the value is high (true) or low (false).
typedef void (*libtock_gpio_async_callback_command)(returncode_t, bool);

returncode_t libtock_gpio_async_set_interrupt_callback(libtock_gpio_async_callback_interrupt cb);

returncode_t libtock_gpio_async_make_output(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_clear(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_set(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_toggle(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_read(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_enable_interrupt(uint32_t port, uint8_t pin, libtock_gpio_interrupt_mode_t irq_config, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_disable_interrupt(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

returncode_t libtock_gpio_async_disable(uint32_t port, uint8_t pin, libtock_gpio_async_callback_command cb);

#ifdef __cplusplus
}
#endif

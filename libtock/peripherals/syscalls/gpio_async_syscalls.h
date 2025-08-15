#pragma once

#include "../../tock.h"
#include "../gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_GPIO_ASYNC 0x80003

#define CONCAT_PORT_DATA(port, data) (((data & 0xFFFF) << 16) | (port & 0xFFFF))

bool libtock_gpio_async_driver_exists(void);
returncode_t libtock_gpio_async_set_upcall_command(subscribe_upcall callback, void* opaque);
returncode_t libtock_gpio_async_set_upcall_interrupt(subscribe_upcall callback, void* opaque);
returncode_t libtock_gpio_async_command_make_output(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_set(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_clear(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_toggle(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config);
returncode_t libtock_gpio_async_command_read(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_enable_interrupt(uint32_t port, uint8_t pin,
                                                         libtock_gpio_interrupt_mode_t irq_config);
returncode_t libtock_gpio_async_command_disable_interrupt(uint32_t port, uint8_t pin);
returncode_t libtock_gpio_async_command_disable(uint32_t port, uint8_t pin);

#ifdef __cplusplus
}
#endif

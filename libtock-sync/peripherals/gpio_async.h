#pragma once

#include <libtock/peripherals/gpio.h>
#include <libtock/peripherals/gpio_async.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_gpio_async_exists(void);

returncode_t libtocksync_gpio_async_make_output(uint32_t port, uint8_t pin);

returncode_t libtocksync_gpio_async_set(uint32_t port, uint8_t pin);

returncode_t libtocksync_gpio_async_clear(uint32_t port, uint8_t pin);

returncode_t libtocksync_gpio_async_toggle(uint32_t port, uint8_t pin);

returncode_t libtocksync_gpio_async_make_input(uint32_t port, uint8_t pin, libtock_gpio_input_mode_t pin_config);

returncode_t libtocksync_gpio_async_read(uint32_t port, uint8_t pin, bool* value);

returncode_t libtocksync_gpio_async_enable_interrupt(uint32_t port, uint8_t pin,
                                                     libtock_gpio_interrupt_mode_t irq_config);

returncode_t libtocksync_gpio_async_disable_interrupt(uint32_t port, uint8_t pin);

returncode_t libtocksync_gpio_async_disable_sync(uint32_t port, uint8_t pin);

#ifdef __cplusplus
}
#endif

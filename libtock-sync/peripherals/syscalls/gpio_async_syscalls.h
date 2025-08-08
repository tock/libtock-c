#pragma once

#include <libtock/peripherals/syscalls/gpio_async_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a GPIO-Async operation with no values to finish.
//
// In lieu of redundant `wait_for_[set|clear|...]`, generic_command can be used
// for any operation that only reports success or failure, expecting no values.
returncode_t libtocksync_gpio_async_yield_wait_for_generic_command(void);

returncode_t libtocksync_gpio_async_yield_wait_for_read(bool* value);

#ifdef __cplusplus
}
#endif

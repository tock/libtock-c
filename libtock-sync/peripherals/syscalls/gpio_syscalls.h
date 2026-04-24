#pragma once

#include <libtock/peripherals/syscalls/gpio_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for a GPIO interrupt and return the pin and its level.
returncode_t libtocksync_gpio_yield_wait_for(uint32_t* pin, bool* value);

#ifdef __cplusplus
}
#endif

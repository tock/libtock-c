#pragma once

#include <libtock/peripherals/syscalls/spi_peripheral_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an SPI peripheral operation to complete.
returncode_t libtocksync_spi_peripheral_yield_wait_for(void);

#ifdef __cplusplus
}
#endif

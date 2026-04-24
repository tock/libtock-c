#pragma once

#include <libtock/peripherals/syscalls/spi_controller_syscalls.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Wait for an SPI controller operation to complete.
returncode_t libtocksync_spi_controller_yield_wait_for(void);

#ifdef __cplusplus
}
#endif

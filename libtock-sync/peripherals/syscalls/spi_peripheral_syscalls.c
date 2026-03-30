#include "spi_peripheral_syscalls.h"

returncode_t libtocksync_spi_peripheral_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_SPI_PERIPHERAL, 1);
  return RETURNCODE_SUCCESS;
}

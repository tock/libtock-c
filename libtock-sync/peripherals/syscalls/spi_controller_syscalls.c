#include "spi_controller_syscalls.h"

returncode_t libtocksync_spi_controller_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_SPI_CONTROLLER, 0);
  return RETURNCODE_SUCCESS;
}

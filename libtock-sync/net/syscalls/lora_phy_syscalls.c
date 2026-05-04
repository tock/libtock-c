#include "lora_phy_syscalls.h"

returncode_t libtocksync_lora_phy_yield_wait_for(void) {
  yield_wait_for(DRIVER_NUM_LORA_PHY_SPI, 0);
  return RETURNCODE_SUCCESS;
}

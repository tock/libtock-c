#include "lora_phy.h"



struct lora_phy_spi_data {
  bool fired;
  returncode_t ret;
};

static struct lora_phy_spi_data result = {.fired = false};

static void lora_phy_spi_cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_lora_phy_write(const uint8_t* write,
                                        uint32_t       len) {
  result.fired = false;
  returncode_t ret;

  ret = libtock_lora_phy_write(write, len, lora_phy_spi_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  return result.ret;
}

returncode_t libtocksync_lora_phy_read_write(const uint8_t* write,
                                             uint8_t*       read,
                                             uint32_t       len) {

  result.fired = false;
  returncode_t ret;

  ret = libtock_lora_phy_read_write(write, read, len, lora_phy_spi_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&result.fired);
  return result.ret;

}

#include "lora_phy.h"

returncode_t libtocksync_lora_phy_write(const uint8_t* write,
                                        uint32_t       len) {
  returncode_t ret;

  ret = libtock_lora_phy_set_readonly_allow_master_write_buffer(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_lora_phy_set_readonly_allow_master_write_buffer(NULL, 0);
  };

  ret = libtock_lora_phy_command_read_write(len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_lora_phy_yield_wait_for();
  return ret;
}

returncode_t libtocksync_lora_phy_read_write(const uint8_t* write,
                                             uint8_t*       read,
                                             uint32_t       len) {
  returncode_t ret;

  ret = libtock_lora_phy_set_readwrite_allow_master_read_buffer(read, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_lora_phy_set_readwrite_allow_master_read_buffer(NULL, 0);
  };

  ret = libtock_lora_phy_set_readonly_allow_master_write_buffer(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_lora_phy_set_readonly_allow_master_write_buffer(NULL, 0);
  };

  ret = libtock_lora_phy_command_read_write(len);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_lora_phy_yield_wait_for();
  return ret;
}

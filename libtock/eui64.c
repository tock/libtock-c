#include "eui64.h"

int eui64_exists(void) {
  return driver_exists(DRIVER_NUM_EUI64);
}

int libtock_eui64_set_readwrite_allow(uint8_t *eui64_buf) {
  // Provide the buffer to the kernel
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_EUI64, EUI64_ALLOW_RW, (void *) eui64_buf, EUI64_BUF_SIZE);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_eui64_command_getter(void) {
  // Issue the command to the kernel
  syscall_return_t cval = command(DRIVER_NUM_EUI64, EUI64_CMD_GETTER, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t get_eui64(uint8_t *eui64_buf) {
  returncode_t ret = libtock_eui64_set_readwrite_allow(eui64_buf);
  if (ret != RETURNCODE_SUCCESS) {
    printf("failed to allow buffer\n");
    return ret;
  }

  return libtock_eui64_command_getter();
}
#include <stdio.h>

#include <timer.h>
#include <libtock-sync/peripherals/usb.h>

int main(void) {
  returncode_t ret;
  printf("[TEST] UDP\n");

  if (!libtock_usb_exists()) {
    printf("USB test: driver is not present\n");
    return -1;
  }

  ret = libtocksync_usb_enable_and_attach();
  if (ret == RETURNCODE_SUCCESS) {
    printf("USB test: Enabled and attached\n");
  } else {
    printf("USB test: Attach failed with status %d\n", ret);
  }

  return 0;
}

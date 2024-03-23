#include <stdio.h>

#include <console.h>
#include <libtock-sync/crypto/hmac.h>

#define KEY_LEN  32
#define DATA_LEN 256
#define DEST_LEN 32

uint8_t key_buf[KEY_LEN]   = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xA1};
uint8_t data_buf[DATA_LEN] = "A language empowering everyone to build reliable and efficient software.";
uint8_t dest_buf[DEST_LEN];



int main(void) {
  returncode_t ret;
  printf("[TEST] HMAC\r\n");

  if (!libtock_hmac_exists()) {
    printf("No HMAC driver.\n");
    return -2;
  }

  ret = libtocksync_hmac_simple(LIBTOCK_HMAC_SHA256,
    key_buf, 11,
    data_buf, strlen(data_buf),
    dest_buf, DEST_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to compute HMAC.\n");
    return -1;
  }

  return 0;
}

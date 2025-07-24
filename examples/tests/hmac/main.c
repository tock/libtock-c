#include <stdio.h>
#include <string.h>

#include <libtock-sync/crypto/hmac.h>
#include <libtock/interface/console.h>

#define KEY_LEN  32
#define DATA_LEN 256
#define DEST_LEN 32

uint8_t key_buf[KEY_LEN]   = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xA0, 0xA1};
uint8_t data_buf[DATA_LEN] = "A language empowering everyone to build reliable and efficient software.";
uint8_t dest_buf[DEST_LEN];
uint8_t expected_output[DEST_LEN] = {0xb5, 0x05, 0x55, 0x53, 0xbd, 0x5e, 0x75, 0xd4, 0xfc, 0x96, 0xbf, 0x25, 0x59, 0xd6,
  0x55, 0xb7, 0x7a, 0x58, 0xae, 0xa8, 0xb4, 0xc4, 0x07, 0x6c, 0x52, 0xf7, 0x74, 0x85, 0x1b, 0xba, 0x06, 0xd8};



int main(void) {
  returncode_t ret;
  printf("[TEST] HMAC\r\n");

  if (!libtock_hmac_exists()) {
    printf("No HMAC driver.\n");
    return -2;
  }

  ret = libtocksync_hmac_simple(LIBTOCK_HMAC_SHA256,
                                key_buf, 11,
                                data_buf, strlen((const char*) data_buf),
                                dest_buf, DEST_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to compute HMAC.\n");
    return -1;
  }

  bool match = true;

  printf("[HMAC] Computed Output: ");
  for (int i = 0; i < DEST_LEN; i++) {
    printf("%02x", dest_buf[i]);

    if (dest_buf[i] != expected_output[i]) {
      match = false;
    }
  }
  printf("\n");

  printf("[HMAC] Expected Output: ");
  for (int i = 0; i < DEST_LEN; i++) {
    printf("%02x", expected_output[i]);
  }
  printf("\n");

  printf("[HMAC] ");
  if (match) {
    printf("Matched!\n");
  } else {
    printf("Error! Computation was incorrect!\n");
  }

  return 0;
}

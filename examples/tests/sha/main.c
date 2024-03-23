#include <stdio.h>
#include <string.h>

#include <libtock-sync/crypto/sha.h>

#define DATA_LEN 256
#define DEST_LEN 32

uint8_t data_buf[DATA_LEN] = "A language empowering everyone to build reliable and efficient software.";
uint8_t dest_buf[DEST_LEN];

int main(void) {
  returncode_t ret;
  printf("[TEST] SHA\r\n");

  if (!libtock_sha_exists()) {
    printf("No SHA driver.\n");
    return -2;
  }

  ret = libtocksync_sha_simple_hash(LIBTOCK_SHA256,
    data_buf, strlen((char*) data_buf),
    dest_buf, DEST_LEN);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to compute SHA.\n");
    return -1;
  }

  return 0;
}

#include <stdio.h>
#include <string.h>

#include <libtock-sync/crypto/sha.h>

#define DATA_LEN 256
#define DEST_LEN 32

uint8_t data_buf[DATA_LEN] = "A language empowering everyone to build reliable and efficient software.";
uint8_t dest_buf[DEST_LEN];
uint8_t correct_buf[DEST_LEN] = {0xd9, 0x13, 0x44, 0xac, 0xb5, 0x29, 0xe0, 0x8a, 0xc1, 0x7b, 0xf4, 0x20, 0x9e, 0x66,
  0x42, 0x48, 0x36, 0x6c, 0x5d, 0x95, 0xec, 0xfe, 0x95, 0xd1, 0xe5, 0x07, 0x77, 0x80, 0x65, 0x9a, 0x91, 0x5b};

int main(void) {
  returncode_t ret;
  printf("[TEST] SHA\r\n");

  if (!libtocksync_sha_exists()) {
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
  printf("SHA computation finished.\n");

  printf("Correct: ");
  for (int i = 0; i < 32; i++) {
    printf("%02x", correct_buf[i]);
  }
  printf("\n");

  bool match = true;
  printf("Got    : ");
  for (int i = 0; i < 32; i++) {
    if (correct_buf[i] != dest_buf[i]) {
      match = false;
    }

    printf("%02x", dest_buf[i]);
  }
  printf("\n");

  if (match) {
    printf("SHA computation correct.\n");
  } else {
    printf("ERROR! SHA computation incorrect.\n");
  }

  bool correct;
  ret = libtocksync_sha_hash_verification(LIBTOCK_SHA256,
                                          data_buf, strlen((char*) data_buf),
                                          correct_buf, DEST_LEN,
                                          &correct);
  if (ret != RETURNCODE_SUCCESS) {
    printf("Unable to verify SHA.\n");
    return -1;
  }
  printf("SHA verification finished.\n");
  if (correct) {
    printf("SHA matched!\n");
  } else {
    printf("ERROR! SHA did NOT match.\n");
  }

  return 0;
}

#include <stdio.h>

#include <tock.h>

int main(void) {
  printf("[TEST] MPU Grant Start\n");

  uint8_t* grant_start = tock_app_grant_begins_at();

  printf("Reading from grant region\n");
  uint8_t value = *grant_start;
  printf("Address: %p, value: %02x\n", grant_start, value);
  printf("FAIL! Should not be able to read grant region!!\n");
}

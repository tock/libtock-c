#include <stdio.h>

#include <tock.h>

int main(void) {
  printf("[TEST] MPU Flash Start Write\n");

  uint8_t* flash_start = tock_app_flash_begins_at();

  printf("Writing first byte in flash...\n");
  *flash_start = 0;
  printf("FAIL! Should not be able to write start of flash!!\n");
}

#include <stdio.h>
#include <stdlib.h>

#include <libtock/storage/kv.h>

int main(void) {
  printf("[TEST] Check for Key-Value Support\n");

  if (libtock_kv_exists()) {
    printf("Key-Value support is enabled.\n");
    tock_exit(0);
  } else {
    printf("[ERROR] Key-Value support is NOT enabled.\n");
    tock_exit(-1);
  }

  return 0;
}

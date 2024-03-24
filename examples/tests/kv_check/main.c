#include <stdio.h>
#include <stdlib.h>

#include <console.h>
#include <kv.h>

int main(void) {
  printf("[KV] Check for Key-Value Support\n");

  if (kv_check_status() == RETURNCODE_SUCCESS) {
    printf("Key-Value support is enabled.\n");
    tock_exit(0);
  } else {
    printf("[ERROR] Key-Value support is NOT enabled.\n");
    tock_exit(-1);
  }

  return 0;
}

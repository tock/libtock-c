#include <stdio.h>

#include <eui64.h>

uint8_t eui64_buf[EUI64_BUF_SIZE];

int main(void) {
  int ret;

  printf("[EUI64] EUI-64 test app\n");

  ret = get_eui64(eui64_buf);

  if (ret != RETURNCODE_SUCCESS) {
    printf("ERROR getting EUI-64\n");
    printf("Error code: %d\n", ret);
    return 1;
  }

  printf("EUI-64: ");
  for (int i = 0; i < EUI64_BUF_SIZE; i++) {
    printf("%02x", eui64_buf[i]);
  }  

  printf("\n");

  return 0;
}

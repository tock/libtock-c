#include <eui64.h>
#include <stdio.h>

int main(void) {
  int ret;

  printf("[EUI64] EUI-64 test app\n");
  uint64_t eui64;
  ret = get_eui64(&eui64);

  if (ret != RETURNCODE_SUCCESS) {
    printf("ERROR getting EUI-64\n");
    printf("Error code: %d\n", ret);
    return 1;
  }

  printf("EUI-64: ");
  for (int i = 0; i < 8; i++) {
    printf("%02x", ((uint8_t*)&eui64)[i]);
  }

  printf("\n");

  return 0;
}

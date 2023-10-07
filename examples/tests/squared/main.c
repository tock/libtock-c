#include "squared.h"
#include "timer.h"

int main(void) {
  printf("Squared Test App\n");

  uint32_t number = 0;

  while (true) {
    uint64_t squared_userspace;
    uint64_t squared_kernel;

    squared_userspace = (uint64_t) number * (uint64_t) number;
    int ret = squared_square(number, &squared_kernel);
    if (ret != RETURNCODE_SUCCESS) {
      printf("ERROR getting square from kernel\n");
      printf("Got error: %s\n", tock_strrcode(ret));
      return -2;
    }

    printf("Squared %li\n", number);
    printf("  Userspace: %#08lx%08lx\n", (uint32_t)(squared_userspace >> 32),(uint32_t)squared_userspace);
    printf("  Kernel:    %#08lx%08lx\n", (uint32_t)(squared_kernel >> 32),(uint32_t)squared_kernel );

    if (squared_userspace == squared_kernel) {
      printf("  Match Success!\n\n");
    } else {
      printf("  ERROR!!\n");
      return -1;
    }

    number += 100500;

    delay_ms(500);
  }

  return 0;
}

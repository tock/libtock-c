#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <tock.h>

int main(void) {
  printf("[TEST] App Settings\n");

  printf("  mem_start:                         %p\n", tock_app_memory_begins_at());
  printf("  mem_end:                           %p\n", tock_app_memory_ends_at());
  printf("  flash_start:                       %p\n", tock_app_flash_begins_at());
  printf("  flash_end:                         %p\n", tock_app_flash_ends_at());
  printf("  kernel_memory_break:               %p\n", tock_app_grant_begins_at());
  printf("  number of writeable flash regions: %i\n", tock_app_number_writeable_flash_regions());
}

#include <console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DRIVER_NUM_TEST_GRANT_DOUBLE_ENTRY 0xf001

void run_test(int);

void run_test(int index) {
  printf("[TEST] Double Grant Entry %i\n", index);

  printf("This test uses a capsule to verify a grant cannot be accessed\n");
  printf("twice simultaneously. When this runs you will likely see a panic\n");
  printf("which means the double grant enter was prevented and this test\n");
  printf("was successful. If you see an \"ERROR\" message then this failed.\n\n");

  // All we have to do is call any command on the test capsule.
  syscall_return_t ret;
  ret = command(DRIVER_NUM_TEST_GRANT_DOUBLE_ENTRY, index, 0, 0);
  if (ret.type == TOCK_SYSCALL_SUCCESS) {
    printf("Syscall returned. Verify no error message was printed.\n");
    printf("Ensure that `debug!()` in the kernel works.\n");
  } else {
    printf("Test was unable to complete.\n");
    printf("Ensure double entry test capsule is exposed to userspace\n");
    printf("with driver number %#x.\n", DRIVER_NUM_TEST_GRANT_DOUBLE_ENTRY);
  }
}

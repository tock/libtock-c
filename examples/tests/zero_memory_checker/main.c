#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int main(void) {
  printf("[ZMC] Look if memory is zeroed.\n");

  // Try to allocate a lot of memory.
  int trying = 128 * 1024;
  void* ptr  = NULL;
  while (true) {
    ptr = sbrk(trying);
    if (ptr == (void*) -1) {
      trying /= 2;
    } else {
      printf("[ZMC] Able to allocate %d bytes\n", trying);
      break;
    }
  }

  uint8_t* modify = (uint8_t*) ptr;
  int A = 0;
  int B = trying / 2;
  int C = trying - 1;

  uint8_t Aval = modify[A];
  uint8_t Bval = modify[B];
  uint8_t Cval = modify[C];

  printf("[ZMC] Checking index %d (%p): %#02x\n", A, modify + A, Aval);
  printf("[ZMC] Checking index %d (%p): %#02x\n", B, modify + B, Bval);
  printf("[ZMC] Checking index %d (%p): %#02x\n", C, modify + C, Cval);

  if (Aval != 0) printf("[ZMC] ERROR! index %d (%p) not zero!!\n", A, modify + A);
  if (Bval != 0) printf("[ZMC] ERROR! index %d (%p) not zero!!\n", B, modify + B);
  if (Cval != 0) printf("[ZMC] ERROR! index %d (%p) not zero!!\n", C, modify + C);

  if (Aval != 0 || Bval != 0 || Cval != 0) {
    exit(-1);
  }

  printf("[ZMC] Memory was zero! This is good. Try to write then reboot and check again.\n");
  printf("[ZMC] If this loops continuously then things are working.\n");

  printf("[ZMC] Writing to memory with arbitrary values\n");

  modify[0] = 0x44;
  modify[trying / 2] = 0xF9;
  modify[trying - 1] = 0x38;

  libtocksync_alarm_delay_ms(1000);

  printf("[ZMC] Rebooting\n");
  tock_restart(0);
}

#include <stdio.h>

#include <libtock/timer.h>
#include <libtock/tock.h>

int main(void) {
  printf("Testing fault behavior by faulting.\n");
  delay_ms(1000);
  int* x = (int*)(0xffffff00);
  *x = 1;
}

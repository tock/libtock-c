#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int main(void) {
  printf("Testing fault behavior by faulting.\n");
  libtocksync_alarm_delay_ms(1000);
  int* x = (int*)(0xffffff00);
  *x = 1;
}

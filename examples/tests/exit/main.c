#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int main(void) {
  printf("Testing exit.\n");
  libtocksync_alarm_delay_ms(1000);
  printf("Exiting.\n");
  tock_exit(0);
  printf("SHOULD NOT BE PRINTED.\n");
  return 0;
}

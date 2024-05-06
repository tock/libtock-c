#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/tock.h>

int x = 1;
int z = 0;
int main(void) {
  printf("Testing restart. x=%d (should be 1), z=%d (should be 0)\n", x, z);
  x++;
  z++;
  libtocksync_alarm_delay_ms(1000);
  printf("Restarting.\n");
  tock_restart(0);
  return 0;
}

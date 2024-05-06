/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>

#include <libtock/interface/console.h>
#include <libtock-sync/services/alarm.h>

int main(void) {
  while (1) {
    printf("Hello\n");
    libtocksync_alarm_delay_ms(1000);
  }
}

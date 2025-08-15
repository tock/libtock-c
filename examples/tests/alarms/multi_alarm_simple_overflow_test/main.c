#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/peripherals/syscalls/alarm_syscalls.h>

static void event_cb(uint32_t now, uint32_t expiration, void* ud) {
  int i = (int)ud;
  printf("%d %lu %lu\n", i, now, expiration);
}

int main(void) {
  libtock_alarm_t t1;
  libtock_alarm_t t2;

  uint32_t now;
  libtock_alarm_command_read(&now);

  uint32_t overflow_ms = libtock_alarm_ticks_to_ms(UINT32_MAX);

  libtock_alarm_in_ms(overflow_ms, event_cb, (void*)1, &t1);
  libtock_alarm_in_ms(1000, event_cb, (void*)2, &t2);

  while (1) {
    yield();
  }
}

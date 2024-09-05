#include <stdio.h>
#include <stdlib.h>

#include <libtock-sync/services/alarm.h>

static void event_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t expiration,
                     void*                             ud) {
  int i = (int)ud;
  printf("%d %ld %ld\n", i, now, expiration);
}

int main(void) {
  libtock_alarm_t t1;
  libtock_alarm_t t2;

  libtock_alarm_repeating_every_ms(500, event_cb, (void*)1, &t1);
  libtock_alarm_repeating_every_ms(1000, event_cb, (void*)2, &t2);

  while (1) {
    yield();
  }
}

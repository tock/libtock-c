#include <stdio.h>

#include <libtock/interface/led.h>
#include <libtock-sync/services/alarm.h>

struct alarm_cb_data {
  bool fired;
};

static struct alarm_cb_data data = { .fired = false };

static void alarm_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled) {
  data.fired = true;
}

int main(void) {

  alarm_t alarm;
  while (1) {
    data.fired = false;
    libtock_alarm_in_ms(1500, alarm_cb, &alarm);
    printf("spinning\n");
    while (yield_no_wait() == 0) {}
    printf("waiting\n");
    data.fired = false;
    libtock_alarm_in_ms(1500, alarm_cb, &alarm);
    yield();
  }
}

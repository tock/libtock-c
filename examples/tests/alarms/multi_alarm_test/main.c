#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>

static int interval;

typedef struct {
  int led;
  libtock_alarm_t timer;
  libtock_alarm_t repeating;
} timer_data;

static void toggle(int led_num) {
  libtock_led_on(led_num);
  libtocksync_alarm_delay_ms(300);
  libtock_led_off(led_num);
}

static void event_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t expiration,
                     void*                             ud) {
  timer_data* td = (timer_data*)ud;
  toggle(td->led);
}

static void start_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t expiration,
                     void*                             ud) {
  timer_data* td = (timer_data*)ud;
  libtock_alarm_repeating_every_ms(interval, event_cb, ud, &td->repeating);

  if (td->led == 1) {
    timer_data* td_new = (timer_data*)malloc(sizeof(timer_data));
    td_new->led = 2;
    libtock_alarm_in_ms(1000 * (2 + 1), start_cb, td_new, &td_new->timer);
  }
  toggle(td->led);
}

int main(void) {
  int spacing = 1000;  // 1 second between each led
  int num_leds;
  libtock_led_count(&num_leds);
  interval = spacing * num_leds;

  for (int i = 0; i < num_leds-1; i++) {
    timer_data* td = (timer_data*)malloc(sizeof(timer_data));
    td->led = i;
    libtock_alarm_in_ms(spacing * (i + 1), start_cb, td, &td->timer);
  }

  while (1) {
    yield();
  }
}

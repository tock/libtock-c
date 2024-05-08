#include <stdlib.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>

static int interval;

typedef struct {
  int led;
  libtock_alarm_t timer;
  libtock_alarm_repeating_t repeating;
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
  libtock_alarm_repeating_every(interval, event_cb, ud, &td->repeating);
  toggle(td->led);
}

int main(void) {
  int spacing = 1000;  // 1 second between each led
  int num_leds;
  libtock_led_count(&num_leds);
  interval = spacing * num_leds;

  for (int i = 0; i < num_leds; i++) {
    timer_data* td = (timer_data*)malloc(sizeof(timer_data));
    td->led = i;
    libtock_alarm_in_ms(spacing * (i + 1), start_cb, td, &td->timer);
  }

  while (1) {
    yield();
  }
}

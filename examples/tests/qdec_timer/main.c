#include <led.h>
#include <qdec.h>
#include <tock.h>
#include <timer.h>

static void timer_fired ( __attribute__ ((unused)) int arg0,
                          __attribute__ ((unused)) int arg1,
                          __attribute__ ((unused)) int arg2,
                          __attribute__ ((unused)) void *ud) {
  int qdec_num = qdec_displacement();
  printf("Displacement val: %d\n", qdec_num);

  // Set LEDs based on acc value
  // Note: LED rules arbitrarily chosen by developer
  if (qdec_num%2 == 0 && qdec_num > 0) {
    led_on(0);
    led_off(1);
    led_off(2);
  }  else if (qdec_num%2 != 0) {
    led_on(1);
    led_off(0);
    led_off(2);
  } else if (qdec_num%2 == 0 && qdec_num < 0) {
    led_on(2);
    led_off(0);
    led_off(1);
  } 
}

int main(void) {
  printf("Starting The [QDEC] Test App for Timers\n");
  if (!qdec_exists()) {
    printf("No QDEC driver\n");
    return -1;
  }
  qdec_enable();
  if (!qdec_interrupt_enable()) {
    printf("Can't enable interrupts!\n");
    return -1;
  }
  printf("QDEC has been fully initiated!\n");
  static tock_timer_t timer;
  timer_every(500, timer_fired, NULL, &timer);
  printf("Timer set!\n");
  return 0;
}

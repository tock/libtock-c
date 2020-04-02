#include <led.h>
#include <timer.h>
#include <qdec.h>
#include <tock.h>

/*static void timer_fired(__attribute__ ((unused)) int arg0,
		        __attribute__ ((unused)) int arg1,
			__attribute__ ((unused)) int arg2,
			__attribute__ ((unused)) void* ud) {
  int qdec_measure = 0;

  int fin = qdec_read();

  printf("QDEC measure: %d\n", fin);
  int num_leds = led_count();
  if (qdec_measure> 0 && qdec_measure< 25 && num_leds > 0) {
      led_on(0);
      led_off(1);
      led_off(2);
      led_off(3);
  } else if (qdec_measure> 25 && qdec_measure< 50 && num_leds > 1) {
      led_on(1);
      led_off(0);
      led_off(2);
      led_off(3);
  } else if (qdec_measure> 50 && qdec_measure< 75 && num_leds > 2) {
      led_on(2);
      led_off(1);
      led_off(0);
      led_off(3);
  } else if (qdec_measure> 75 && qdec_measure< 100 && num_leds > 3) {
      led_on(3);
      led_off(0);
      led_off(1);
      led_off(2);
  }   
}*/

int main(void) {
  printf("[QDEC] Test App\n");
  
  if (!qdec_exists()) {
    printf("No QDEC driver\n");
    return -1;
  }

  qdec_enable();
  printf("Setting timer\n");
 
  // Set mode to whichever example you want
  //static tock_timer_t timer;
  //timer_every(5000, timer_fired, NULL, &timer);
  printf("timer set\n");
}
 

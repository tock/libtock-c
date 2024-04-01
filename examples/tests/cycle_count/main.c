#include <stdio.h>

#include <cycle_counter.h>
#include <led.h>
#include <tock.h>

int main(void) {
  uint64_t cycles = 0;

  int rc = cycle_counter_reset();
  if (rc == RETURNCODE_ENOSUPPORT) {
    printf("Cycle counter not available\n");
  } else if (rc == RETURNCODE_ERESERVE) {
    printf("Cycle counter claimed by another app\n");
  } else if (rc != 0) {
    printf("Cycle counter: other error: %d\n", rc);
  }

  if (rc != 0) {
    return 0;
  }

  cycle_counter_start();
  led_toggle(0);
  cycle_counter_stop();
  cycle_counter_read(&cycles);
  printf("cycles to toggle led first time: %lld\n", cycles);

  cycle_counter_reset();
  cycle_counter_start();
  led_toggle(0);
  cycle_counter_stop();
  cycle_counter_read(&cycles);
  printf("cycles to toggle led second time: %lld\n", cycles);
  return 0;
}

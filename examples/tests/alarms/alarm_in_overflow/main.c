#include <libtock/services/alarm.h>
#include <libtock/tock.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * This test registers a "long running" alarm and verifies that the
 * alarm fires at about the time when it's expected to.
 *
 * NOTE: Modify the "MAX_TICKS" macro in "libtock/alarm.c" to
 * test the implementation of libtock_alarm_in_ms without waiting for
 * the alarm to overflow after 2^32 ticks (which is about 36 hours
 * with a 32768Hz oscillator). Example: if you want to simulate the
 * alarm overflowing after a second on a 32768Hz oscillator you can set
 * "MAX_TICKS" to 32768. You can get the frequence of the alarm by calling
 * alarm_internal_frequency.
 */

#define ALARM_LEN_MS 2000

static bool fired = false;

// helper function to get current time in milliseconds
// from gettimeasticks
static uint32_t get_time_ms(void) {
  struct timeval tv;
  libtock_alarm_gettimeasticks(&tv);
  return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// callback of type subscribe_upcall which will get called when alarm fires
static void alarm_fire_callback(__attribute__ ((unused)) uint32_t unused0,
                                __attribute__ ((unused)) uint32_t unused1,
                                void*                             user_data) {
  fired = true;

  uint32_t expected_alarm_end_ms = *((uint32_t*)user_data);
  uint32_t actual_fire_time_ms   = get_time_ms();
  uint32_t difference_ms         = actual_fire_time_ms - expected_alarm_end_ms;

  printf("Alarm Fired!\n");
  printf("\tExpected alarm to fire at %ld ms\n", expected_alarm_end_ms);
  printf("\tAlarm actually fired at %ld ms\n", actual_fire_time_ms);

  // check for millisecond level precision
  if (difference_ms <= 1) {
    printf("\tSuccess! Difference of %ld ms\n", difference_ms);
  } else {
    printf("\tFailed! Difference of %ld ms (> 1ms)\n", difference_ms);
  }

}

int main(void) {
  // need to allocate memory for a alarm before giving a pointer to `libtock_alarm_in_ms`
  libtock_alarm_t* alarm = malloc(sizeof(libtock_alarm_t));

  uint32_t alarm_start_ms = get_time_ms();

  // NOTE: this "expected_alarm_end_ms" variabe does NOT need to be passed into libtock_alarm_in_ms
  // as user data. It is only used for this testing program to print out the difference
  // between when the alarm was expected to fire vs. when it actually fired.
  // If you don't need to pass user data to your callback then you can pass in NULL
  // for user data.
  int expected_alarm_end_ms = alarm_start_ms + ALARM_LEN_MS;

  // actually schedule alarm to call callback once enough time passes
  libtock_alarm_in_ms(ALARM_LEN_MS, alarm_fire_callback, (void*)(&expected_alarm_end_ms), alarm);

  // yield back to the kernel because otherwise alarm callback won't be scheduled
  yield_for(&fired);

  free(alarm);
}

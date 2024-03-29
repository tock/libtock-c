// TODO ADD COPYRIGHT ETC

#include <alarm.h>
#include <assert.h>
#include <internal/alarm.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <timer.h>

static alarm_t alarm;

static void callback(int __attribute__((unused)) now, int __attribute__((unused)) interval, int __attribute__(
                       (unused)) arg2, void *aInstance) {
  otPlatAlarmMilliFired(aInstance);
}

// convert ms to physical clock ticks (this should be done using the timer_in method,
// but this works in the meantime)
static uint32_t milliToTicks(uint32_t milli) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);
  return milli * (frequency / 1000);
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
  // printf("Starting alarm at %d, with interval %d\n", aT0, aDt);
  OT_UNUSED_VARIABLE(aInstance);
  // TODO: this implementation for the alarm is temporary. Future implementation
  // should use the `timer_in` functionality to set the alarm.

  // check that aT0 and aDt are both less than max
  // uint32_t. this is a temporary fix until we implement
  // the wrap around functionality
  if ((aT0 > UINT32_MAX) || (aDt > UINT32_MAX)) {
    return;
  }

  // inputs aT0, aDt are in ms, but libtock call must be in clock ticks
  uint32_t ref = milliToTicks(aT0);
  uint32_t dt  = milliToTicks(aDt);

  // NOTE: This is a problem that will have to be fixed. Thread wants to
  // be able to set a 2^32 millisecond timer. However, when converting
  // milliseconds into clock ticks, the multiplication will overflow.
  //
  // OpenThread expects to be able to use the full 32 bit range in milliseconds.
  alarm_at(ref, dt, callback, (void *)aInstance, &alarm);
}

void otPlatAlarmMilliStop(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);
  alarm_cancel(&alarm);
}

uint32_t otPlatAlarmMilliGetNow(void) {
  // TODO: this implementation wraps around at 511 seconds on 
  // the nrf52840 as the counter register is 24 bits wide.
  // This will need to be fixed. 
  struct timeval tv;
  gettimeasticks(&tv, NULL);    // second arg is unused

  uint32_t nowSeconds    = tv.tv_sec;
  uint32_t nowMicro      = tv.tv_usec;
  uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

  return nowMilli32bit;
}
#include <alarm.h>
#include <assert.h>
#include <internal/alarm.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <plat.h>
#include <timer.h>

static alarm_t alarm;
static tock_timer_t timer_wrap;

// maintain global variables to be used for timer wrapping logic
uint32_t wrap_point = 0;
uint32_t wrap_count;
uint32_t prev_time_value;

static void callback(int __attribute__((unused)) now, int __attribute__((unused)) interval, int __attribute__(
			(unused)) arg2, void *aInstance) {
	otPlatAlarmMilliFired(aInstance);
}

// convert ms to physical clock ticks (this should be done using the timer_in method,
// but this works in the meantime)
static uint32_t milliToTicks(uint32_t milli) {
	uint32_t frequency;
	alarm_internal_frequency(&frequency);
	return (milli / 1000) * frequency + (milli % 1000) * (frequency / 1000);;
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
		assert(false);
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



static void wrap_time_upcall(int __attribute__((unused)) now, int __attribute__((unused)) interval, int __attribute__(
			(unused)) arg2, void __attribute__((unused)) *_ud) {
	// Timer for detecting wrapping event has fired. Call otPlatAlarmMilliGetNow()
	// to check check if a wrap has occured. 
	otPlatAlarmMilliGetNow();
}

// OpenThread timing initializer to obtain the wrap point 
void init_otPlatAlarm(void) {
	// libtock alarms overflow at 2^{32} ticks. We 
	// desire to find the time at which this overflow
	// will occur to set an alarm. The reason for this
	// alarm is elaborated upon in `otPlatAlarmMilliGetNow`.

	uint32_t frequency;
	alarm_internal_frequency(&frequency);
	wrap_point = (UINT32_MAX) / frequency;
	prev_time_value = otPlatAlarmMilliGetNow();
}

uint32_t otPlatAlarmMilliGetNow(void) {
	// This method faces the unique challenge of handling
	// libtock timers wrapping at 2^{32} bits. This creates
	// challenges at the wrap point as `gettimeasticks` will
	// wrap to be zero. We subsequently need `otPlatAlarmMilliGetNow`
	// to account for this so that we can provide time values 
	// past 2^{32} ticks.
	//
	// We implement this logic by obtaining the timepoint at which 
	// timer wraps occur and counting the number of wrapping events
	// that have occured. Together, these two values allow for the 
	// "getNow" time to be calculated. For this to work, we must 
	// ensure that we detect every wrapping event. We set a timer 
	// for this purpose to occur when the ticks value is halfway 
	// to wrapping. This is done to guard against the corner case
	// of other latencies resulting in missed overflow events.
	//
	// This approach has the obvious drawback of "waking up"
	// more often than otherwise necessary. This concern would 
	// be particularly pronounced for low power thread devices. 
	// However, the current Tock OpenThread port is not optimized 
	// for allowing sleep and this is subsequently not a major 
	// concern at this juncture.

	// We will check for overflows here and subsequently cancel and
	// reset the alarm.
	timer_cancel(&timer_wrap);

	struct timeval tv;
	gettimeasticks(&tv, NULL);    


	uint32_t nowSeconds    = tv.tv_sec;
	uint32_t nowMicro      = tv.tv_usec;
	uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

	// detect wrapping event
	if (nowMilli32bit < prev_time_value) {
		wrap_count++;
	}

	prev_time_value = nowMilli32bit;
	nowMilli32bit += wrap_count * wrap_point;

	// Set timer to ensure we do not miss a wrapping event. 
	// (TODO: add more detailed comment and confirm this will guarad
	// against missed wrapping event)
	timer_in(wrap_point >> 1, wrap_time_upcall, NULL, &timer_wrap); 

	return nowMilli32bit;
}

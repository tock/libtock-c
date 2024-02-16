// TODO ADD COPYRIGHT ETC

#include <alarm.h>
#include <internal/alarm.h>
#include <openthread/platform/alarm-micro.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <assert.h>
#include <timer.h>

// Based on reference implementations (https://github.com/openthread/ot-nxp/blob/main/src/common/alarm_freertos.c and https://github.com/openthread/ot-nrf528xx/blob/main/src/src/alarm.c), it seems there should only be one timer.
static alarm_t alarm;
static alarm_t alarm_micro;


static void callback(int now, int interval, int arg2, void *aInstance) {
    // printf("callback called\n");
    // printf("an alarm is firing! NOW: %lu -- INTERVAL %lu\n", now, interval);
    otPlatAlarmMilliFired(aInstance);
    // alarm_cancel(&alarm);
}

// convert ms to physical clock ticks
static uint32_t milliToTicks(uint32_t milli) {
    uint32_t frequency;
    alarm_internal_frequency(&frequency);
    
    return (milli / 1000) * frequency;
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    OT_UNUSED_VARIABLE(aInstance);
    // printf("alarm milli start\n");
    // DEBUGGING:
    // printf("alarm: setting alarm %lums after %lums. It's currently %lu\n", aDt, aT0,
    //        otPlatAlarmMilliGetNow());

    // check that aT0 and aDt are both less than max uint32_t
    if ((aT0 > UINT32_MAX) || (aDt > UINT32_MAX)) {
        printf("voided over limit\n");
        assert(false);
        return;
    }
    
    // inputs aT0, aDt are in ms, but libtock call must be in clock ticks
    uint32_t ref = milliToTicks(aT0);
    uint32_t dt = milliToTicks(aDt);
    // printf("***************************\n");    
    // printf("CURRENT TIME: %lu\n", otPlatAlarmMilliGetNow());
    // printf("*****************************alarm: setting alarm %lums after %lums.\n",  aDt, aT0);
    int return_code = alarm_at(ref, dt, callback, (void *)aInstance, &alarm);
    // NOTE: This is a big problem that will have to be fixed. Thread wants to
    // be able to set a 2^30 millisecond timer. However, when converting
    // milliseconds into clock ticks, the multiplication will overflow.
    // 
    // OpenThread expects to be able to use the full 32 bit range in milliseconds.

    // printf("alarm returncode %d\n", return_code);
}

void otPlatAlarmMilliStop(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    // printf("alarm milli stop\n");
    alarm_cancel(&alarm);
}

uint32_t otPlatAlarmMilliGetNow(void) {
    // printf("alarm milli get now: ");
    struct timeval tv;
    gettimeasticks(&tv, NULL);  // second arg is unused
    
    uint32_t nowSeconds = tv.tv_sec;
    uint32_t nowMicro = tv.tv_usec;
    uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

    // printf("%lu\n", nowMilli32bit);
    return nowMilli32bit;
}

// Symmetric implementation for microsecond alarm
// WARNING: no prints when this code is included?
/*
void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm micro start\n");
    printf("alarm_micro: setting alarm %lums after %lums. It's currently %lu\n", aDt, aT0,
           otPlatAlarmMilliGetNow());
    int return_code = alarm_at(aT0, aDt, callback, (void *)aInstance, &alarm_micro);
    printf("alarm_micro returncode %d\n", return_code);
}

void otPlatAlarmMicroStop(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm_micro stop\n");
    alarm_cancel(&alarm_micro);
}

uint32_t otPlatAlarmMicroGetNow(void) {
    printf("alarm_micro get now: ");
    struct timeval tv;
    gettimeasticks(&tv, NULL);  // second arg is unused
    
    uint32_t nowSeconds = tv.tv_sec;
    uint32_t nowMicro = tv.tv_usec;
    uint32_t nowMicro32bit = (nowSeconds * 1000 * 1000) + (nowMicro);

    printf("%lu\n", nowMicro32bit);
    return nowMicro32bit;
}
*/

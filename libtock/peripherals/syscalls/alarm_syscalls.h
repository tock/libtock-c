#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ALARM 0x0

// Check if the alarm driver is available on this board.
bool libtock_alarm_exists(void);

/*
 * Sets the callback for timers
 *
 * When invoked, the callback's first argument will be the timer value at which
 * the timer was fired.
 */
int libtock_alarm_set_upcall(subscribe_upcall callback, void *opaque);

/*
 * Get the the timer frequency in Hz.
 */
int libtock_alarm_command_get_frequency(uint32_t* frequency);

/*
 * Get the current alarm counter.
 */
int libtock_alarm_command_read(uint32_t* time);

/*
 * Stops any outstanding hardware alarm.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int libtock_alarm_command_stop(void);

/*
 * Starts a oneshot alarm
 *
 * expiration - relative expiration value from when kernel handles syscall.
 * Sets *actual to the time the actual alarm was set for by the kernel.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int libtock_alarm_command_set_relative(uint32_t dt, uint32_t* actual);

/*
 * Starts a oneshot alarm
 *
 * expiration - absolute expiration value = reference + dt.
 * Using reference + dt allows library to distinguish expired timers from
 * timers in the far future.
 *
 * Side-effects: cancels any existing/outstanding timers
 */
int libtock_alarm_command_set_absolute(uint32_t reference, uint32_t dt);

#ifdef __cplusplus
}
#endif

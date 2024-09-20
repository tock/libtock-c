/*
 * This module allows the client to initiate alarms and receive
 * callbacks when those alarms have expired. Clients can set one-shot alarms to
 * fire at particular clock values (`libtock_alarm_at`) or periodic alarms
 * (`libtock_alarm_repeating_every_ms`)
 *
 * The client should not assume anything about the underlying clock used by an
 * implementation other than that it is running at sufficient frequency to
 * deliver at least millisecond granularity and that it is a 32-bit clock (i.e.
 * it will wrap at 2^32 clock ticks).
 */

#pragma once

#include "../peripherals/syscalls/alarm_syscalls.h"
#include "../tock.h"

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function signature for alarm callback.
//
// - `arg1` (`now`): The current time when this callback was enqueued by the
//   kernel.
// - `arg2` (`scheduled`): The time (reference + dt) that this alarm was
//   originally scheduled to fire at.
// - `arg3` (`opaque`): An arbitrary user pointer passed back to the callback.
typedef void (*libtock_alarm_callback)(uint32_t, uint32_t, void*);

/** \brief Utility function for converting hardware-specific ticks to milliseconds
 */
uint32_t libtock_alarm_ticks_to_ms(uint32_t);

/** \brief Opaque handle to a single-shot alarm.
 *
 * An opaque handle to an alarm created by `alarm_at` or `alarm_in`. Memory
 * management is handled by the underlying implementation.
 */
typedef struct alarm {
  uint32_t reference;
  uint32_t dt;
  libtock_alarm_callback callback;
  void* ud;
  struct alarm* next;
  struct alarm* prev;
} libtock_alarm_ticks_t;

/** \brief Opaque handle to a repeating alarm.
 *
 * An opaque handle to an alarm created by `libtock_alarm_repeating_every_ms`
 * and `libtock_alarm_in_ms`.
 */
typedef struct alarm_data {
  // Length of timer in milliseconds.
  uint32_t interval_ms;
  // Number of times the underlying counter will overflow
  // before hitting the target interval_ms.
  uint32_t overflows_left;
  // Number of ticks remaining after the last time the
  // counter overflows.
  uint32_t remaining_ticks;
  libtock_alarm_callback callback;
  void* user_data;
  libtock_alarm_ticks_t alarm;
} libtock_alarm_t;


/** \brief Create a new alarm to fire at a particular clock value.
 *
 * The `alarm` parameter is allocated by the caller and must live as long as
 * the alarm is outstanding. `reference` and `dt` are in terms of the tick
 * time.
 *
 * Alarms longer than 2^32 ticks should use `libtock_alarm_in_ms`.
 *
 * \param reference the reference time from which the alarm is being set in ticks.
 * \param dt the time after reference that the alarm should fire in ticks.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param alarm pointer to a new alarm_t to be used by the implementation to keep
 *        track of the alarm.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtock_alarm_at(uint32_t reference, uint32_t dt, libtock_alarm_callback callback, void* opaque,
                     libtock_alarm_ticks_t* alarm);

/** \brief Cancels an existing alarm.
 *
 * The caller is responsible for freeing the `alarm_t`.
 *
 * \param alarm
 */
void libtock_alarm_cancel(libtock_alarm_ticks_t* alarm);

// Use this to implement _gettimeofday yourself as libtock-c doesn't provide
// an implementation.
//
// See https://github.com/tock/libtock-c/pull/355#issuecomment-1841351091 for
// more details
//
// ```c
// int _gettimeofday(struct timeval *tv, void *tzvp) {
//   return libtock_alarm_gettimeasticks(tv, tzvp);
// }
// ```
int libtock_alarm_gettimeasticks(struct timeval* tv, void* tzvp);

/** \brief Create a new alarm to fire in `ms` milliseconds.
 *
 * The `timer` parameter is allocated by the caller and must live as long as
 * the alarm is outstanding.
 *
 * This should be used for long periods. For more precision, but less time
 * range, use `alarm_at`.
 *
 * \param ms the number of milliseconds to fire the alarm after.
 * \param cb a callback to be invoked when the alarm expires.
 * \param opaque pointer passed to the callback.
 * \param alarm handle to the alarm that was created.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm);

/** \brief Create a new repeating alarm to fire every `ms` milliseconds.
 *
 * The `alarm` parameter is allocated by the caller and must live as long as
 * the repeating alarm is outstanding.
 *
 * \param ms the interval to fire the alarm at in milliseconds.
 * \param cb a callback to be invoked when the alarm expires.
 * \param opaque pointer passed to the callback.
 * \param alarm pointer to a new libtock_alarm_t to be used by the implementation to
 *        keep track of the alarm.
 */
void libtock_alarm_repeating_every_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque,
                                      libtock_alarm_t* alarm);

/** \brief Cancels an existing alarm set in milliseconds.
 *
 * \param alarm to cancel.
 */
void libtock_alarm_ms_cancel(libtock_alarm_t* alarm);

#ifdef __cplusplus
}
#endif

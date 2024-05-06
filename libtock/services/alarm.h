/*
 * This module allows the client to initiate alarms and receive
 * callbacks when those alarms have expired. Clients can set one-shot alarms to
 * fire at particular clock values (`libtock_alarm_at`) or periodic alarms
 * (`libtock_alarm_repeating_every`)
 *
 * The client should not assume anything about the underlying clock used by an
 * implementation other than that it is running at sufficient frequency to
 * deliver at least millisecond granularity and that it is a 32-bit clock (i.e.
 * it will wrap at 2^32 clock ticks).
 */

#pragma once

#include "../tock.h"
#include "../peripherals/syscalls/alarm_syscalls.h"

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif


// Function signature for alarm callback.
//
// - `arg1` (`now`): The current time when this callback was enqueued by the kernel.
// - `arg2` (`scheduled`): The time (reference + dt) that this alarm was originally scheduled
//    to fire at
typedef void (*libtock_alarm_callback)(uint32_t, uint32_t);

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
} alarm_t;

/** \brief Opaque handle to a repeating alarm.
 *
 * An opaque handle to a repeating alarm created by `libtock_alarm_repeating_every`.
 */
typedef struct alarm_repeating {
  uint32_t interval;
  libtock_alarm_callback cb;
  alarm_t alarm;
} alarm_repeating_t;


/** \brief Create a new alarm to fire at a particular clock value.
 *
 * The `alarm` parameter is allocated by the caller and must live as long as
 * the alarm is outstanding.
 *
 * \param reference: the reference time from which the alarm is being set
 * \param dt: the time after reference that the alarm should fire
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param a pointer to a new alarm_t to be used by the implementation to keep
 *        track of the alarm.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtock_alarm_at(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, alarm_t *alarm);

/** \brief Cancels an existing alarm.
 *
 * The caller is responsible for freeing the `alarm_t`.
 *
 * \param alarm
 */
void libtock_alarm_cancel(alarm_t*);

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
int libtock_alarm_gettimeasticks(struct timeval *tv, void *tzvp);

/** \brief Create a new alarm to fire in `ms` milliseconds.
 *
 * \param ms the number of milliseconds to fire the alarm after.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param A handle to the alarm that was created.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, alarm_t* alarm);

/** \brief Create a new repeating alarm to fire every `ms` milliseconds.
 *
 * The `alarm_repeating` parameter is allocated by the caller and must live as long as
 * the repeating alarm is outstanding.
 *
 * \param ms the interval to fire the alarm at in milliseconds.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param a pointer to a new alarm_repeating_t to be used by the implementation to
 *        keep track of the alarm.
 */
void libtock_alarm_repeating_every(uint32_t ms, libtock_alarm_callback cb, alarm_repeating_t* alarm_repeating);

/** \brief Cancels an existing repeating alarm.
 *
 * \param alarm_repeating
 */
void libtock_alarm_repeating_cancel(alarm_repeating_t* alarm_repeating);

#ifdef __cplusplus
}
#endif

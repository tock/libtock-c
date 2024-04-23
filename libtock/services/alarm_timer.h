/*
 * This module allows the client to initiate alarms and receive
 * callbacks when those alarms have expired. Clients can set one-shot alarms to
 * fire at particular clock values (`alarm_at`) or periodic alarms (`timer_every`)
 *
 * The client should not assume anything about the underlying clock used by an
 * implementation other than that it is running at sufficient frequency to
 * deliver at least millisecond granularity and that it is a 32-bit clock (i.e.
 * it will wrap at 2^32 clock ticks).
 */

#pragma once

#include "tock.h"
#include "../peripherals/syscalls/alarm_syscalls.h"

#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

// NOTE: alarm_timer uses a different callback approach than most other
// libtock drivers, by not defining a `libtock_alarm_callback` or similar,
// and instead using the "old" approach of passing around bare `subscribe_upcall`
// and `ud` pointers. This is necessary because the userspace virtualisation provided
// by this driver requires using `tock_enqueue`, a generic API
// that needs to be able to accept arbitrary callback types.

/** \brief Opaque handle to a single-shot alarm.
 *
 * An opaque handle to an alarm created by `alarm_at` or `alarm_in`. Memory
 * management is handled by the underlying implementation.
 */
typedef struct alarm {
  uint32_t reference;
  uint32_t dt;
  subscribe_upcall *callback;
  void* ud;
  struct alarm* next;
  struct alarm* prev;
} alarm_t;

/** \brief Opaque handle to a repeating alarm.
 *
 * An opaque handle to a repeating alarm created by `timer_every`.
 */
typedef struct tock_timer {
  uint32_t interval;
  subscribe_upcall* cb;
  void* ud;
  alarm_t alarm;
} tock_timer_t;


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
int libtock_alarm_at(uint32_t reference, uint32_t dt, subscribe_upcall cb, void* ud, alarm_t *alarm);

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
//   return libtock_gettimeasticks(tv, tzvp);
// }
// ```
int libtock_gettimeasticks(struct timeval *tv, void *tzvp);

/** \brief Create a new alarm to fire in `ms` milliseconds.
 *
 * \param ms the number of milliseconds to fire the alarm after.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param A handle to the alarm that was created.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtock_timer_in(uint32_t ms, subscribe_upcall cb, void* ud, tock_timer_t* timer);

/** \brief Create a new repeating alarm to fire every `ms` milliseconds.
 *
 * The `timer` parameter is allocated by the caller and must live as long as
 * the timer is outstanding.
 *
 * \param ms the interval to fire the alarm at in milliseconds.
 * \param callback a callback to be invoked when the alarm expires.
 * \param userdata passed to the callback.
 * \param a pointer to a new tock_timer_t to be used by the implementation to
 *        keep track of the alarm.
 */
void libtock_timer_every(uint32_t ms, subscribe_upcall cb, void* ud, tock_timer_t* timer);

/** \brief Cancels an existing timer.
 *
 * \param timer
 */
void libtock_timer_cancel(tock_timer_t*);

#ifdef __cplusplus
}
#endif

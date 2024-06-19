#pragma once

#include <libtock/peripherals/syscalls/alarm_syscalls.h>
#include <libtock/services/alarm.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Blocks for the given amount of time in milliseconds.
 *
 * This is a blocking version of `libtock_alarm_in_ms`. Instead of calling a user
 * specified callback, it blocks the current call-stack.
 *
 * \param ms the number of milliseconds to delay for.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL.
 */
int libtocksync_alarm_delay_ms(uint32_t ms);

/** \brief Functions as yield_for with a timeout in milliseconds.
 *
 * This yields on a condition variable, but will return early
 * if that condition is not met before the timeout in milliseconds.
 *
 * \param cond the condition to yield_for.
 * \param ms the amount of time before returning without the condition.
 * \return An error code. Either RETURNCODE_SUCCESS or RETURNCODE_FAIL for timeout.
 */
int libtocksync_alarm_yield_for_with_timeout(bool* cond, uint32_t ms);

#ifdef __cplusplus
}
#endif

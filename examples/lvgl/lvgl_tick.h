#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Milliseconds elapsed since the first call, wrapping at 2^32 ms, for use as
// LVGL's tick source (`lv_tick_set_cb`).
//
// LVGL computes elapsed time as `now - last` on `uint32_t` milliseconds, so
// its tick must wrap at 2^32 ms. The alarm driver's tick counter, however,
// wraps at 2^32 *ticks*, i.e. every 512 seconds on nRF52 (24-bit RTC at
// 32768Hz exposed as 32-bit ticks at 8.4MHz). Converting that counter to
// milliseconds directly yields a millisecond counter that jumps back to 0
// every 512 seconds, which LVGL interprets as ~49 days having elapsed,
// firing all timers early.
//
// This must be called at least once every 2^32 ticks (512 seconds on
// nRF52), which `lv_timer_handler` does as long as some timer is active.
uint32_t lvgl_tick_ms(void);

#ifdef __cplusplus
}
#endif

#pragma once

#include "tock.h"
#include "syscalls/analog_comparator_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Request the number of available ACs.
returncode_t libtock_analog_comparator_count(int* count);

// Compare the voltages of two pins (if one is higher than the other) on the
// corresponding AC by doing a single comparison.
//
// - channel - index of the analog comparator channel, starting at 0.
// - comparison - set to true if is higher
returncode_t libtock_analog_comparator_comparison(uint8_t channel, bool* comparison);

// Enable interrupt-based comparisons. This will make the AC listen and send an
// interrupt as soon as Vp > Vn.
//
// - channel - index of the analog comparator channel, starting at 0.
returncode_t libtock_analog_comparator_start_comparing(uint8_t channel);

// Disable interrupt-based comparisons. This will make the AC stop listening,
// and thereby stop sending interrupts.
//
// - channel - index of the analog comparator channel, starting at 0.
returncode_t libtock_analog_comparator_stop_comparing(uint8_t channel);

#ifdef __cplusplus
}
#endif

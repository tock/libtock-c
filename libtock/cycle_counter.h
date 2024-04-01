#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_CYCLE_COUNTER 0x00090008

bool cycle_counter_exists(void);
int cycle_counter_start(void);
int cycle_counter_stop(void);
int cycle_counter_reset(void);
int cycle_counter_read(uint64_t *count);

#ifdef __cplusplus
}
#endif

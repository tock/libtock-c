#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SQUARED 0x90005


// Return `number` squared in the `squared` value. Returns a return code value
// to indicate success or failure.
int squared_square(uint32_t number, uint64_t* squared);


#ifdef __cplusplus
}
#endif

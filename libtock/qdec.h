#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_QDEC 0x90003

bool qdec_exists(void);
bool qdec_enable(void);
bool qdec_interrupt_enable(void);
int qdec_subscribe(subscribe_cb callback, void* callback_args);
int qdec_displacement(void);

#ifdef __cplusplus
}
#endif

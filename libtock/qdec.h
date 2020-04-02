#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_QDEC 0x90001

bool qdec_exists(void);
int qdec_enable(void);
int qdec_interrupt_enable(void);
int qdec_set_callback(subscribe_cb callback, void* callback_args);
int qdec_displacement(void);

#ifdef __cplusplus
}
#endif

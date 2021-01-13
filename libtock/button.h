#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_BUTTON 0x3

subscribe_return_t button_subscribe(subscribe_cb callback, void *ud);
syscall_return_t button_enable_interrupt(int pin_num);
syscall_return_t button_disable_interrupt(int pin_num);
int button_read(int pin_num);
int button_count(void);


#ifdef __cplusplus
}
#endif


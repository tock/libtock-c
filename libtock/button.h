#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_BUTTON 0x3
#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_DEPRESSED 0
#define BUTTON_PRESSED   1 

int button_subscribe(subscribe_upcall callback, void *ud);
int button_enable_interrupt(int button_num);
int button_disable_interrupt(int button_num);
int button_read(int button_num, int* button_value);
int button_count(int* count);


#ifdef __cplusplus
}
#endif


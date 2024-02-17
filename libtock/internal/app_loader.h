#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "tock.h"

// Driver number for system call
#define DRIVER_NUM_APP_LOADER   0x10001
#define DRIVER_NUM_BUTTON       0x3

#define BUTTON_1 0
#define BUTTON_2 1
#define BUTTON_3 2
#define BUTTON_4 3

#define BUTTON_PRESSED 1

int app_loader_exists(void);
int app_loader_subscribe(subscribe_upcall cb, void *userdata);
int app_loader_write_buffer(uint8_t *buffer, uint32_t len);
int app_loader_command_setup(uint32_t app_length);
int app_loader_command_write(uint32_t flash_offset, uint32_t write_length);
int app_loader_command_load(void);
int app_loader_callback_subscribe_init(void);
int write_app(void);

int button_init(void);
int button_subscribe(subscribe_upcall callback, void *ud);
int button_enable_interrupt(int button_num);
int button_disable_interrupt(int button_num);
int button_read(int button_num, int* button_value);
int button_count(int* count);

#ifdef __cplusplus
}
#endif

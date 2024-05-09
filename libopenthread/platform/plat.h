#pragma once
#include<stdbool.h>

// Userspace ring buffer for handling async received upcalls 
// from the kernel while performing potentially sync (yielding)
// operations. The need for this is explained in more detail in
// the comment at the top of `system.c`. The new field is used
// to prevent `otSysProcessDrivers` from performing unnecessary
// work if new data has not arrived. 
typedef struct {
    char *buffer;
    int write_index;
    int read_index;
    bool new;
} ring_buffer;

// Initializer needed for alarm PAL methods.
void init_otPlatAlarm(void);

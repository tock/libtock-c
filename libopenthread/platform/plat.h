#pragma once
#include <libtock/tock.h>
#include <openthread/platform/radio.h>


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

bool pending_alarm_done_callback_status(void);

void reset_pending_alarm_done_callback(void);

bool pending_tx_done_callback_status(otRadioFrame *ackFrame, returncode_t *status, otRadioFrame* txFrame);

void reset_pending_tx_done_callback(void);

bool pending_rx_done_callback_status(void);

void reset_pending_rx_done_callback(void);

// Check if there are pending events that need to be handled before
// calling yield(). These events can be generated if the application happens
// to call yield() somewhere besides the main OpenThread loop.
bool openthread_platform_pending_work(void);

// Initializer needed for alarm PAL methods.
void init_otPlatAlarm(void);

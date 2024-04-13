#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NMEA 0x60009

// check if nmea sensor exists
bool nmea_exists(void);

int nmea_set_read_buffer(uint8_t* buffer, uint32_t len);

// function to be called when the nmea measurement is finished
//
// callback       - pointer to function to be called
// callback_args  - pointer to data provided to the callback
int nmea_set_callback (subscribe_upcall callback, void* callback_args);

int nmea_read_sentance(void);

// initiate a synchronous nmea seantance read
int nmea_read_sentance_sync(uint8_t* buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

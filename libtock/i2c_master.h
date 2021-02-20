#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

int i2c_master_set_callback (subscribe_upcall callback, void* callback_args);
int i2c_master_set_buffer(uint8_t* buffer, uint32_t len);
int i2c_master_write(uint8_t address, uint8_t length);
int i2c_master_read(uint8_t address, uint8_t length);
int i2c_master_write_read(uint8_t address, uint8_t length);

int i2c_master_write_sync(uint16_t address, uint8_t* buffer, uint16_t len);
int i2c_master_read_sync(uint16_t address, uint8_t* buffer, uint16_t len);
int i2c_master_write_read_sync(uint16_t address, uint8_t* buffer, uint16_t len);
  
#ifdef __cplusplus
}
#endif

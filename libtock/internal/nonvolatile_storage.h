#pragma once

#include "tock.h"

#define DRIVER_NUM_NONVOLATILE_STORAGE 0x50001

#ifdef __cplusplus
extern "C" {
#endif

subscribe_return_t nonvolatile_storage_internal_read_done_subscribe(subscribe_cb cb, void *userdata);
subscribe_return_t nonvolatile_storage_internal_write_done_subscribe(subscribe_cb cb, void *userdata);

allow_rw_return_t nonvolatile_storage_internal_read_buffer(uint8_t* buffer, uint32_t len);
allow_ro_return_t nonvolatile_storage_internal_write_buffer(uint8_t* buffer, uint32_t len);

int nonvolatile_storage_internal_get_number_bytes(void);
int nonvolatile_storage_internal_read(uint32_t offset, uint32_t length);
int nonvolatile_storage_internal_write(uint32_t offset, uint32_t length);

#ifdef __cplusplus
}
#endif

#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_NONVOLATILE_STORAGE 0x50001

bool libtock_nonvolatile_storage_driver_exists(void);

returncode_t libtock_nonvolatile_storage_set_upcall_read_done(subscribe_upcall cb, void* opaque);

returncode_t libtock_nonvolatile_storage_set_upcall_write_done(subscribe_upcall cb, void* opaque);

returncode_t libtock_nonvolatile_storage_set_allow_readwrite_read_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_nonvolatile_storage_set_allow_readonly_write_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_nonvolatile_storage_command_get_number_bytes(uint32_t* number_bytes);

returncode_t libtock_nonvolatile_storage_command_read(uint32_t offset, uint32_t length);

returncode_t libtock_nonvolatile_storage_command_write(uint32_t offset, uint32_t length);

#ifdef __cplusplus
}
#endif

#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_ISOLATED_NONVOLATILE_STORAGE 0x50004

bool libtock_isolated_nonvolatile_storage_exists(void);

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_get_number_bytes_done(subscribe_upcall cb, void* opaque);

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_read_done(subscribe_upcall cb, void* opaque);

returncode_t libtock_isolated_nonvolatile_storage_set_upcall_write_done(subscribe_upcall cb, void* opaque);

returncode_t libtock_isolated_nonvolatile_storage_set_allow_readwrite_read_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_isolated_nonvolatile_storage_set_allow_readonly_write_buffer(uint8_t* buffer, uint32_t len);

returncode_t libtock_isolated_nonvolatile_storage_command_get_number_bytes(void);

returncode_t libtock_isolated_nonvolatile_storage_command_read(uint32_t offset, uint32_t length);

returncode_t libtock_isolated_nonvolatile_storage_command_write(uint32_t offset, uint32_t length);

#ifdef __cplusplus
}
#endif

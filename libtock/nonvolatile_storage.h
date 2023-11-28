#pragma once

#include "tock.h"



#ifdef __cplusplus
extern "C" {
#endif


// Function signature for button press callbacks.
//
// - `arg1` (`returncode_t`): Returncode indicating status of the operation.
// - `arg2` (`int`): Length.
typedef void (*nonvolatile_storage_callback)(returncode_t, int);

returncode_t nonvolatile_storage_read(uint8_t* buffer, uint32_t length, uint32_t address, nonvolatile_storage_callback cb);
returncode_t nonvolatile_storage_write(uint8_t* buffer, uint32_t length, uint32_t address, nonvolatile_storage_callback cb);
returncode_t nonvolatile_storage_read_sync(uint8_t* buffer, uint32_t length, uint32_t address);
returncode_t nonvolatile_storage_write_sync(uint8_t* buffer, uint32_t length, uint32_t address);


#ifdef __cplusplus
}
#endif

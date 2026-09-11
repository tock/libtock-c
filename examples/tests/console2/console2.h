#pragma once

// Implement writing data to the console driver on the second console driver num.

#include <libtock/tock.h>
#include <libtock/interface/syscalls/console_syscalls.h>

bool console2_driver_exists(void);
returncode_t console2_sync_write(const uint8_t* buffer, uint32_t length, uint32_t* written);

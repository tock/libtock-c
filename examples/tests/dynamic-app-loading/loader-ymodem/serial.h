#pragma once

#include <libtock/tock.h>

bool libtock_ymodem_driver_exists(void);
returncode_t libtocksync_ymodem_write(const uint8_t* buffer, uint32_t length, uint32_t* written);
returncode_t libtocksync_ymodem_read(uint8_t* buffer, uint32_t length, uint32_t* read);

// Like `libtocksync_ymodem_read`, but gives up and returns
// `RETURNCODE_ECANCEL` if the read hasn't completed within `timeout_ms`
// milliseconds, instead of blocking forever. `*read` is only written on
// success.
returncode_t libtocksync_ymodem_read_with_timeout(uint8_t* buffer, uint32_t length, uint32_t* read,
                                                   uint32_t timeout_ms);
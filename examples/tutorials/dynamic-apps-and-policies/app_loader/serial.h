#pragma once

#include <libtock/tock.h>

bool libtock_ymodem_driver_exists(void);
returncode_t libtocksync_ymodem_write(const uint8_t* buffer, uint32_t length, uint32_t* written);
returncode_t libtocksync_ymodem_read(uint8_t* buffer, uint32_t length, uint32_t* read);

// Like `libtocksync_ymodem_read`, but gives up and returns
// `RETURNCODE_ECANCEL` if the read hasn't completed within `timeout_ms`
// milliseconds, instead of blocking forever. `*read` is only written on
// success.
//
// This driver doesn't implement `receive_abort()`, so a timed-out read
// can't actually be cancelled at the kernel: the receive stays allowed and
// outstanding, and will eventually complete on its own whenever enough
// bytes arrive. If this function returns `RETURNCODE_ECANCEL`, the caller
// MUST call it again with the *same* `buffer` and `length` (the receive
// that's still running will fill that buffer, not a new one) to pick up
// the result -- this can be repeated across as many timeouts as needed.
// The buffer is only (re-)allowed to the kernel on the first call of such
// a sequence.
returncode_t libtocksync_ymodem_read_with_timeout(uint8_t* buffer, uint32_t length, uint32_t* read,
                                                   uint32_t timeout_ms);
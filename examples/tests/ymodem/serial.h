#include <libtock/tock.h>

bool libtock_ymodem_driver_exists(void);
returncode_t libtocksync_ymodem_write(const uint8_t* buffer, uint32_t length, uint32_t* written);
returncode_t libtocksync_ymodem_read(uint8_t* buffer, uint32_t length, uint32_t* read);
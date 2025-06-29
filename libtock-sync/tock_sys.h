#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int _tock_write(int fd, const void* buf, uint32_t count);

#ifdef __cplusplus
}
#endif

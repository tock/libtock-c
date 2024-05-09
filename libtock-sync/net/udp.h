#pragma once

#include <libtock/tock.h>
#include <libtock/net/udp.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: fix these names

returncode_t libtocksync_udp_send(void *buf, size_t len,
                    sock_addr_t *dst_addr);

returncode_t libtocksync_udp_recv(void *buf, size_t len, size_t* received_len);

#ifdef __cplusplus
}
#endif

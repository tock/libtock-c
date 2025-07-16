#pragma once

#include <libtock/net/udp.h>
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

bool libtocksync_udp_exists(void);

// TODO: fix these names

returncode_t libtocksync_udp_send(void* buf, size_t len,
                                  sock_addr_t* dst_addr);

returncode_t libtocksync_udp_recv(void* buf, size_t len, size_t* received_len);

#ifdef __cplusplus
}
#endif

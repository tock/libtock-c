#pragma once

#include <libtock/tock.h>
#include <libtock/net/udp.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: fix these names

returncode_t libtock_udp_send_sync(void *buf, size_t len,
                    sock_addr_t *dst_addr);

returncode_t libtock_udp_recv_sync(void *buf, size_t len, size_t* received_len);

#ifdef __cplusplus
}
#endif

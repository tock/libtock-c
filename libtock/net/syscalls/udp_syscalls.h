#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_UDP 0x30002

#define ALLOW_RO_TX  0

#define ALLOW_RW_RX     0
#define ALLOW_RW_CFG    1
#define ALLOW_RW_RX_CFG 2

#define SUBSCRIBE_RX 0
#define SUBSCRIBE_TX 1

#define COMMAND_GET_IFACES 1
#define UDP_COMMAND_SEND       2
#define COMMAND_BIND       3
#define COMMAND_GET_TX_LEN 4

bool libtock_udp_exists(void);

returncode_t libtock_udp_set_upcall_frame_received(subscribe_upcall callback, void* opaque);
returncode_t libtock_udp_set_upcall_frame_transmitted(subscribe_upcall callback, void* opaque);

returncode_t libtock_udp_set_readonly_allow(const uint8_t* buffer, uint32_t len);
returncode_t libtock_udp_set_readwrite_allow_rx(const uint8_t* buffer, uint32_t len);
returncode_t libtock_udp_set_readwrite_allow_cfg(const uint8_t* buffer, uint32_t len);
returncode_t libtock_udp_set_readwrite_allow_rx_cfg(const uint8_t* buffer, uint32_t len);


returncode_t libtock_udp_command_get_ifaces(size_t len);
returncode_t libtock_udp_command_send(void);
returncode_t libtock_udp_command_bind(void);
returncode_t libtock_udp_command_get_max_tx_len(int* len);

#ifdef __cplusplus
}
#endif

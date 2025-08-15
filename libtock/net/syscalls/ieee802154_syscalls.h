#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_IEEE802154 0x30001
#define ALLOW_RW_RX  0
#define ALLOW_RW_CFG 1

#define ALLOW_RO_TX  0

#define SUBSCRIBE_RX 0
#define SUBSCRIBE_TX 1

#define COMMAND_STATUS          1
#define COMMAND_SET_ADDR_SHORT  2
#define COMMAND_SET_ADDR_LONG   3
#define COMMAND_SET_PAN         4
#define COMMAND_SET_CHANNEL     5
#define COMMAND_SET_POWER       6
#define COMMAND_CONFIG_COMMIT   7

#define COMMAND_GET_ADDR_SHORT  8
#define COMMAND_GET_ADDR_LONG   9
#define COMMAND_GET_PAN         10
#define COMMAND_GET_CHANNEL     11
#define COMMAND_GET_POWER       12

#define COMMAND_MAX_NEIGHBORS            13
#define COMMAND_NUM_NEIGHBORS            14
#define COMMAND_GET_NEIGHBOR_ADDR_SHORT  15
#define COMMAND_GET_NEIGHBOR_ADDR_LONG   16
#define COMMAND_ADD_NEIGHBOR             17
#define COMMAND_REMOVE_NEIGHBOR          18

#define COMMAND_GET_MAX_KEYS   19
#define COMMAND_GET_NUM_KEYS   20
#define COMMAND_GET_KEY_LEVEL  21
#define COMMAND_GET_KEY_ID     22
#define COMMAND_GET_KEY        23
#define COMMAND_ADD_KEY        24
#define COMMAND_REMOVE_KEY     25
#define COMMAND_SEND           26
#define COMMAND_SEND_RAW       27

#define COMMAND_SET_ADDR_LONG_U64 28
#define COMMAND_GET_ADDR_LONG_U64 29
#define COMMAND_RADIO_ON 30
#define COMMAND_RADIO_OFF 31

// IEEE 802.15.4 subscribe upcalls syscalls //
returncode_t libtock_ieee802154_set_upcall_frame_received(subscribe_upcall callback, void* opaque);
returncode_t libtock_ieee802154_set_upcall_frame_transmitted(subscribe_upcall callback, void* opaque);

// IEEE 802.15.4 allow read only buffer syscalls //
returncode_t libtock_ieee802154_set_readonly_allow(const uint8_t* buffer, uint32_t len);

// IEEE 802.15.4 allow read write buffer syscalls //
returncode_t libtock_ieee802154_set_readwrite_allow_rx(uint8_t* buffer, uint32_t len);
returncode_t libtock_ieee802154_set_readwrite_allow_cfg(uint8_t* buffer, uint32_t len);

// Check for presence of the driver
bool libtock_ieee802154_driver_exists(void);

// IEEE 802.15.4 command syscalls //
returncode_t libtock_ieee802154_command_status(void);
returncode_t libtock_ieee802154_command_set_address_short(uint16_t addr_short);
returncode_t libtock_ieee802154_command_set_address_long(void);
returncode_t libtock_ieee802154_command_set_pan(uint16_t pan);
returncode_t libtock_ieee802154_command_set_channel(uint8_t channel);
returncode_t libtock_ieee802154_command_set_power(uint32_t power);
returncode_t libtock_ieee802154_command_config_commit(void);
returncode_t libtock_ieee802154_command_get_address_short(uint32_t* addr_short);
returncode_t libtock_ieee802154_command_get_address_long(void);
returncode_t libtock_ieee802154_command_get_pan(uint32_t* pan);
returncode_t libtock_ieee802154_command_get_channel(uint32_t* channel);
returncode_t libtock_ieee802154_command_get_power(uint32_t* power);
returncode_t libtock_ieee802154_command_get_max_neighbors(uint32_t* max_neighbors);
returncode_t libtock_ieee802154_command_get_number_neighbors(uint32_t* num_neighbors);
returncode_t libtock_ieee802154_command_get_neighbor_address_short(uint32_t index, uint32_t* addr_short);
returncode_t libtock_ieee802154_command_get_neighbor_address_long(uint32_t index);
returncode_t libtock_ieee802154_command_add_neighbor(uint16_t addr_short, uint32_t* index);
returncode_t libtock_ieee802154_command_remove_neighbor(uint32_t index);
returncode_t libtock_ieee802154_command_get_max_keys(uint32_t* max_keys);
returncode_t libtock_ieee802154_command_get_num_keys(uint32_t* num_keys);
returncode_t libtock_ieee802154_command_get_key_security_level(uint32_t index, uint32_t* level);
returncode_t libtock_ieee802154_command_get_key_id(uint32_t index);
returncode_t libtock_ieee802154_command_get_key(uint32_t index);
returncode_t libtock_ieee802154_command_add_key(uint32_t* index);
returncode_t libtock_ieee802154_command_remove_key(uint32_t index);
returncode_t libtock_ieee802154_command_send(uint16_t addr_short);
returncode_t libtock_ieee802154_command_send_raw(void);
returncode_t libtock_ieee802154_command_set_address_long_u64(uint64_t address);
returncode_t libtock_ieee802154_command_get_address_long_u64(uint64_t* address);
returncode_t libtock_ieee802154_command_radio_on(void);
returncode_t libtock_ieee802154_command_radio_off(void);

#ifdef __cplusplus
}
#endif

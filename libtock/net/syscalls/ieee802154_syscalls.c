#include "ieee802154_syscalls.h"

returncode_t libtock_ieee802154_set_upcall_frame_received(subscribe_upcall callback, void* opaque){
  subscribe_return_t sval = subscribe(DRIVER_NUM_IEEE802154, SUBSCRIBE_RX, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}
returncode_t libtock_ieee802154_set_upcall_frame_transmitted(subscribe_upcall callback, void* opaque){
  subscribe_return_t sval = subscribe(DRIVER_NUM_IEEE802154, SUBSCRIBE_TX, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_ieee802154_set_readonly_allow(const uint8_t* buffer, uint32_t len){
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_IEEE802154, ALLOW_RO_TX, buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_ieee802154_set_readwrite_allow_rx(uint8_t* buffer, uint32_t len){
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_IEEE802154, ALLOW_RW_RX, buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}
returncode_t libtock_ieee802154_set_readwrite_allow_cfg(uint8_t* buffer, uint32_t len){
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_IEEE802154, ALLOW_RW_CFG, buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

bool libtock_ieee802154_exists(void){
  return driver_exists(DRIVER_NUM_IEEE802154);
}

returncode_t libtock_ieee802154_command_status(void){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_STATUS, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_set_address_short(uint16_t addr_short){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_STATUS, addr_short, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_set_address_long(void){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SET_ADDR_LONG, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_set_pan(uint16_t pan){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SET_PAN, pan, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_set_channel(uint8_t channel){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SET_CHANNEL, channel, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_set_power(uint32_t power){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SET_POWER, power, 0);
  return tock_command_return_novalue_to_returncode(com);

}

returncode_t libtock_ieee802154_command_config_commit(void){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_CONFIG_COMMIT, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_get_address_short(uint32_t *addr_short){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_ADDR_SHORT, 0, 0);
  return tock_command_return_u32_to_returncode(com, addr_short);
}

returncode_t libtock_ieee802154_command_get_address_long(void){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_ADDR_LONG, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_get_pan(uint32_t *pan){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_PAN, 0, 0);
  return tock_command_return_u32_to_returncode(com, pan);
}

returncode_t libtock_ieee802154_command_get_channel(uint32_t *channel){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_CHANNEL, 0, 0);
  return tock_command_return_u32_to_returncode(com, channel);
}

returncode_t libtock_ieee802154_command_get_power(uint32_t *power){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_POWER, 0, 0);
  return tock_command_return_u32_to_returncode(com, power);
}

returncode_t libtock_ieee802154_command_get_max_neighbors(uint32_t *max_neighbors){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_MAX_NEIGHBORS, 0, 0);
  return tock_command_return_u32_to_returncode(com, max_neighbors);
}

returncode_t libtock_ieee802154_command_get_number_neighbors(uint32_t *neighbor_count){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_NUM_NEIGHBORS, 0, 0);
  return tock_command_return_u32_to_returncode(com, neighbor_count);
}

returncode_t libtock_ieee802154_command_get_neighbor_address_short(uint32_t index, uint32_t *addr_short){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_NEIGHBOR_ADDR_SHORT, index, 0);
  return tock_command_return_u32_to_returncode(com, addr_short);
}

returncode_t libtock_ieee802154_command_get_neighbor_address_long(uint32_t index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_NEIGHBOR_ADDR_LONG, index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_add_neighbor(uint16_t addr_short, uint32_t *index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_ADD_NEIGHBOR, addr_short, 0);
  return tock_command_return_u32_to_returncode(com, index);
}

returncode_t libtock_ieee802154_command_remove_neighbor(uint32_t index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_REMOVE_NEIGHBOR, index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_get_max_keys(uint32_t *max_key_count){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_MAX_KEYS, 0, 0);
  return tock_command_return_u32_to_returncode(com, max_key_count);
}

returncode_t libtock_ieee802154_command_get_num_keys(uint32_t *key_count){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_NUM_KEYS, 0, 0);
  return tock_command_return_u32_to_returncode(com, key_count);
}

returncode_t libtock_ieee802154_command_get_key_security_level(uint32_t index, uint32_t *level){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_KEY_LEVEL, index, 0);
  return tock_command_return_u32_to_returncode(com, level);
}

returncode_t libtock_ieee802154_command_get_key_id(uint32_t index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_KEY_ID, index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_get_key(uint32_t index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_GET_KEY, index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_add_key(uint32_t *index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_ADD_KEY, 0, 0);
  return tock_command_return_u32_to_returncode(com, index);
}

returncode_t libtock_ieee802154_command_remove_key(uint32_t index){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_REMOVE_KEY, index, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_send(uint16_t addr_short){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SEND, addr_short, 0);
  return tock_command_return_novalue_to_returncode(com);
}

returncode_t libtock_ieee802154_command_send_raw(void){
  syscall_return_t com = command(DRIVER_NUM_IEEE802154, COMMAND_SEND_RAW, 0, 0);
  return tock_command_return_novalue_to_returncode(com);
}

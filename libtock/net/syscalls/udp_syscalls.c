#include "udp_syscalls.h"

bool libtock_udp_exists(void){
  return driver_exists(DRIVER_NUM_UDP);
}

// UDP Subscribe Upcall Syscalls //
returncode_t libtock_udp_set_upcall_frame_received(subscribe_upcall callback, void* opaque){
  subscribe_return_t sval = subscribe(DRIVER_NUM_UDP, SUBSCRIBE_RX, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

returncode_t libtock_udp_set_upcall_frame_transmitted(subscribe_upcall callback, void* opaque){
  subscribe_return_t sval = subscribe(DRIVER_NUM_UDP, SUBSCRIBE_TX, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}

// UDP Read/Write Allow Syscalls //
returncode_t libtock_udp_set_readonly_allow(const uint8_t* buffer, uint32_t len){
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_UDP, ALLOW_RO_TX, (const void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}

returncode_t libtock_udp_set_readwrite_allow_rx(const uint8_t* buffer, uint32_t len){
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_UDP, ALLOW_RW_RX, (void *) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_udp_set_readwrite_allow_cfg(const uint8_t* buffer, uint32_t len){
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_UDP, ALLOW_RW_CFG, (void *) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

returncode_t libtock_udp_set_readwrite_allow_rx_cfg(const uint8_t* buffer, uint32_t len){
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_UDP, ALLOW_RW_RX_CFG, (void *) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}

// UDP Command Syscalls //
returncode_t libtock_udp_command_get_ifaces(size_t len){
  syscall_return_t cval = command(DRIVER_NUM_UDP, COMMAND_GET_IFACES, len, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_udp_command_send(void){
  syscall_return_t cval = command(DRIVER_NUM_UDP, UDP_COMMAND_SEND, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_udp_command_bind(void){
  syscall_return_t cval = command(DRIVER_NUM_UDP, COMMAND_BIND, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}

returncode_t libtock_udp_command_get_max_tx_len(int* len){
  syscall_return_t cval = command(DRIVER_NUM_UDP, COMMAND_GET_TX_LEN, 0, 0);
  return tock_command_return_u32_to_returncode(cval, (uint32_t*) len);
}
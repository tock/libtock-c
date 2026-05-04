#include <stdio.h>
#include <string.h>

#include <libtock-sync/services/alarm.h>
#include <libtock/defer.h>
#include <libtock/net/ieee802154.h>
#include <libtock/net/syscalls/ieee802154_syscalls.h>

#include "ieee802154.h"

#include "syscalls/ieee802154_syscalls.h"

bool libtocksync_ieee802154_exists(void) {
  return libtock_ieee802154_driver_exists();
}

returncode_t libtocksync_ieee802154_send(uint16_t         addr,
                                         security_level_t level,
                                         key_id_mode_t    key_id_mode,
                                         uint8_t*         key_id,
                                         const uint8_t*   payload,
                                         uint8_t          len) {
  returncode_t ret;
  bool acked;

  // Setup the CFG buffer with security parameters.
  uint8_t buf_cfg[11];
  buf_cfg[0] = level;
  buf_cfg[1] = key_id_mode;
  int bytes = libtock_ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(buf_cfg + 2, key_id, bytes);
  }

  ret = libtock_ieee802154_set_readwrite_allow_cfg(buf_cfg, sizeof(buf_cfg));
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);
  }

  ret = libtock_ieee802154_set_readonly_allow(payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readonly_allow(NULL, 0);
  }

  ret = libtock_ieee802154_command_send(addr);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_ieee802154_yield_wait_for_send(&acked);
  return ret;
}

returncode_t libtocksync_ieee802154_send_raw(
  const uint8_t* payload,
  uint8_t        len) {
  returncode_t ret;
  bool acked;

  ret = libtock_ieee802154_set_readonly_allow(payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readonly_allow(NULL, 0);
  }

  ret = libtock_ieee802154_command_send_raw();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_ieee802154_yield_wait_for_send(&acked);
  return ret;
}

returncode_t libtocksync_ieee802154_receive(const libtock_ieee802154_rxbuf* frame) {
  returncode_t ret;

  ret = libtock_ieee802154_set_readwrite_allow_rx((uint8_t*) frame, libtock_ieee802154_RING_BUFFER_LEN);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // receive upcall is only scheduled by the kernel if a frame is successfully received
  ret = libtocksync_ieee802154_yield_wait_for_recv();
  return ret;
}

returncode_t libtocksync_ieee802154_up(void) {
  // For the raw implementation of the 15.4 driver we need to turn on the radio.
  // If the kernel is managing the 15.4 stack, we likely do not need to do this.
  // In that case, this will error, but we ignore the error.
  libtock_ieee802154_command_radio_on();

  // Spin until radio is on.
  bool status;
  libtock_ieee802154_is_up(&status);

  while (!status) {
    libtocksync_alarm_delay_ms(10);
    libtock_ieee802154_is_up(&status);
  }
  libtocksync_alarm_delay_ms(10); // without this delay, immediate calls to send can still fail.
  return RETURNCODE_SUCCESS;
}

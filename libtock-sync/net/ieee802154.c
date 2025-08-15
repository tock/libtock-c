#include <stdio.h>

#include <libtock-sync/services/alarm.h>

#include "ieee802154.h"



returncode_t libtocksync_ieee802154_send(uint16_t         addr,
                                         security_level_t level,
                                         key_id_mode_t    key_id_mode,
                                         uint8_t*         key_id,
                                         const uint8_t*   payload,
                                         uint8_t          len,
                                         bool*            acked) {
  // Setup parameters in CFG
  BUF_CFG[0] = level;
  BUF_CFG[1] = key_id_mode;
  int bytes = libtock_ieee802154_key_id_bytes(key_id_mode);
  if (bytes > 0) {
    memcpy(BUF_CFG + 2, key_id, bytes);
  }

  // Allow CFG buffer to the kernel
  returncode_t ret = libtock_ieee802154_set_readwrite_allow_cfg((void*) BUF_CFG, 11);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readwrite_allow_cfg(NULL, 0);
  };

  // Allow payload buffer to the kernel
  ret = libtock_ieee802154_set_readonly_allow((void*) payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readonly_allow(NULL, 0);
  };

  // Issue the send command and wait for the transmission to be done.
  ret = libtock_ieee802154_command_send(addr);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_ieee802154_yield_wait_for_transmit(acked);
  return ret;
}


returncode_t libtocksync_ieee802154_send_raw(const uint8_t* payload,
                                             uint8_t        len,
                                             bool*          acked) {
  returncode_t ret;

  // Setup parameters in ALLOW_RO_TX.
  ret = libtock_ieee802154_set_readonly_allow((void*) payload, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readonly_allow(NULL, 0);
  };

  ret = libtock_ieee802154_command_send_raw();
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtocksync_ieee802154_yield_wait_for_transmit(acked);
  return ret;
}


returncode_t libtocksync_ieee802154_receive(const libtock_ieee802154_rxbuf* frame) {
  returncode_t ret;

  // Provide the ring buffer to the kernel
  ret = libtock_ieee802154_set_readwrite_allow_rx((uint8_t*) frame, libtock_ieee802154_RING_BUFFER_LEN);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_ieee802154_set_readwrite_allow_rx(NULL, 0);
  };

  int pans;
  int dst_addr;
  int src_addr;
  ret = libtocksync_ieee802154_yield_wait_for_receive(&pans, &dst_addr, &src_addr);
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

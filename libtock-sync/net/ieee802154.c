#include <libtock-sync/services/alarm.h>

#include "ieee802154.h"


#include <stdio.h>
struct ieee802154_receive_data {
  bool fired;
  int pan;
  int src_addr;
  int dest_addr;
};

static struct ieee802154_receive_data receive_result = { .fired = false };


struct ieee802154_send_data {
  bool fired;
  bool acked;
  statuscode_t status;
};

static struct ieee802154_send_data send_result     = { .fired = false };
static struct ieee802154_send_data send_result_raw = { .fired = false };

static void ieee802154_receive_done_cb(int pan, int src_addr, int dest_addr) {
  receive_result.fired     = true;
  receive_result.pan       = pan;
  receive_result.src_addr  = src_addr;
  receive_result.dest_addr = dest_addr;
}

static void ieee802154_send_done_cb(statuscode_t status, bool acked) {
  send_result.fired  = true;
  send_result.acked  = acked;
  send_result.status = status;
}

static void ieee802154_send_raw_done_cb(statuscode_t status, bool acked) {
  send_result_raw.fired  = true;
  send_result_raw.acked  = acked;
  send_result_raw.status = status;
}

returncode_t libtocksync_ieee802154_send(uint16_t         addr,
                                         security_level_t level,
                                         key_id_mode_t    key_id_mode,
                                         uint8_t*         key_id,
                                         const uint8_t*   payload,
                                         uint8_t          len) {
  send_result.fired = false;

  returncode_t ret = libtock_ieee802154_send(addr, level, key_id_mode, key_id, payload, len, ieee802154_send_done_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the frame to be sent
  yield_for(&send_result.fired);

  return tock_status_to_returncode(send_result.status);
}


returncode_t libtocksync_ieee802154_send_raw(
  const uint8_t* payload,
  uint8_t        len) {
  send_result_raw.fired = false;

  returncode_t ret = libtock_ieee802154_send_raw(payload, len, ieee802154_send_raw_done_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for the frame to be sent
  returncode_t sync_timeout_ret = libtocksync_alarm_yield_for_with_timeout(&send_result_raw.fired, 100);
  if (sync_timeout_ret != RETURNCODE_SUCCESS) {
    return sync_timeout_ret;
  }

  return tock_status_to_returncode(send_result_raw.status);
}

returncode_t libtocksync_ieee802154_receive(const libtock_ieee802154_rxbuf* frame) {
  receive_result.fired = false;

  returncode_t ret = libtock_ieee802154_receive(frame, ieee802154_receive_done_cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  // Wait for a frame
  yield_for(&receive_result.fired);

  // receive upcall is only scheduled by the kernel if a frame is successfully received
  return RETURNCODE_SUCCESS;
}

returncode_t libtocksync_ieee802154_up(void) {
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

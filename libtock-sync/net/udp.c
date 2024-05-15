#include <string.h>

#include "udp.h"

struct recv_data {
  bool fired;
  int val;
  statuscode_t status;
};

struct send_data {
  bool fired;
  statuscode_t status;
};

static struct send_data send_sync_result = { .fired = false };
static struct recv_data recv_sync_result = { .fired = false };

static void send_callback(statuscode_t ret) {
  send_sync_result.fired  = true;
  send_sync_result.status = ret;
}

static void recv_callback(statuscode_t ret, int len) {
  recv_sync_result.val    = len;
  recv_sync_result.fired  = true;
  recv_sync_result.status = ret;
}

returncode_t libtocksync_udp_send(void* buf, size_t len,
                                  sock_addr_t* dst_addr) {
  returncode_t ret;
  send_sync_result.fired = false;

  ret = libtock_udp_send(buf, len, dst_addr, send_callback);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&send_sync_result.fired);
  return tock_status_to_returncode(send_sync_result.status);
}

returncode_t libtocksync_udp_recv(void* buf, size_t len, size_t* received_len) {
  returncode_t ret;
  recv_sync_result.fired = false;

  ret = libtock_udp_recv(buf, len, recv_callback);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&recv_sync_result.fired);

  *received_len = recv_sync_result.val;
  return tock_status_to_returncode(recv_sync_result.status);
}

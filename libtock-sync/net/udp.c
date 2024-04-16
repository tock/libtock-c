#include <string.h>

#include "udp.h"

struct recv_data {
  bool fired;
  int val;
  returncode_t result;
};

struct send_data {
  bool fired;
  returncode_t result;
};

static struct send_data send_sync_result = { .fired = false };
static struct recv_data recv_sync_result = { .fired = false };

static void send_callback(returncode_t ret) {
  send_sync_result.fired  = true;
  send_sync_result.result = ret;
}

static void recv_callback(returncode_t ret, int len) {
  recv_sync_result.val    = len;
  recv_sync_result.fired  = true;
  recv_sync_result.result = ret;
}

returncode_t libtock_udp_send_sync(void *buf, size_t len,
                                   sock_addr_t *dst_addr) {
  returncode_t ret;
  send_sync_result.fired = false;

  ret = libtock_udp_send(buf, len, dst_addr, send_callback);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&send_sync_result.fired);
  return send_sync_result.result;
}

returncode_t libtock_udp_recv_sync(void *buf, size_t* len) {
  returncode_t ret;
  recv_sync_result.fired = false;

  ret = libtock_udp_recv(buf, len, recv_callback);
  if (ret != RETURNCODE_SUCCESS) return ret;

  yield_for(&recv_sync_result.fired);

  *len = recv_sync_result.val;
  return recv_sync_result.result;
}
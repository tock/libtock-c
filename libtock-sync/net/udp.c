#include <string.h>

#include <libtock/defer.h>
#include <libtock/net/syscalls/udp_syscalls.h>

#include "syscalls/udp_syscalls.h"
#include "udp.h"

bool libtocksync_udp_exists(void) {
  return libtock_udp_driver_exists();
}

returncode_t libtocksync_udp_send(void* buf, size_t len,
                                  __attribute__ ((unused)) sock_addr_t* dst_addr) {
  returncode_t ret;

  ret = libtock_udp_set_readonly_allow(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_udp_set_readonly_allow(NULL, 0);
  }

  ret = libtocksync_udp_yield_wait_for_send();
  return ret;
}

returncode_t libtocksync_udp_recv(void* buf, size_t len, size_t* received_len) {
  returncode_t ret;

  ret = libtock_udp_set_readwrite_allow_rx(buf, len);
  if (ret != RETURNCODE_SUCCESS) return ret;
  defer { libtock_udp_set_readwrite_allow_rx(NULL, 0);
  }

  ret = libtocksync_udp_yield_wait_for_recv(received_len);
  return ret;
}

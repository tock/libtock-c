#include "tock.h"
#include "udp.h"

const int UDP_DRIVER = 0x30002;

static const int ALLOW_RX     = 0;
static const int ALLOW_TX     = 1;
static const int ALLOW_CFG    = 2;
static const int ALLOW_RX_CFG = 3;

static const int SUBSCRIBE_RX = 0;
static const int SUBSCRIBE_TX = 1;

// COMMAND 0 is driver existence check
static const int COMMAND_GET_IFACES = 1;
static const int COMMAND_SEND       = 2;
static const int COMMAND_BIND       = 3;
static const int COMMAND_GET_TX_LEN = 4;

static unsigned char BUF_TX_CFG[2 * sizeof(sock_addr_t)];
static unsigned char zero_addr[2 * sizeof(sock_addr_t)];

int udp_bind(sock_handle_t *handle, sock_addr_t *addr, unsigned char *buf_bind_cfg) {
  // Pass interface to listen on and space for kernel to write src addr
  // of received packets
  // In current design, buf_bind_cfg will still be written with addresses of external
  // senders sending addresses to the bound port even if the client application has
  // not set up a receive callback on this port. Of course, the client application
  // does not have to read these addresses or worry about them.
  memcpy(&(handle->addr), addr, sizeof(sock_addr_t));
  int bytes = sizeof(sock_addr_t);
  int err   = allow(UDP_DRIVER, ALLOW_RX_CFG, (void *) buf_bind_cfg, 2 * bytes);
  if (err < 0) return err;

  memcpy(buf_bind_cfg + bytes, &(handle->addr), bytes);

  // Set up source address/port pair for sending. Store it in tx_cfg
  // Notably, the pair chosen must match the address/port to which the
  // app is bound, unless the kernel changes in the future to allow for
  // sending from a port to which the app is not bound.
  err = allow(UDP_DRIVER, ALLOW_CFG, (void *) BUF_TX_CFG, 2 * bytes);
  if (err < 0) return err;

  memcpy(BUF_TX_CFG, &(handle->addr), bytes);

  return command(UDP_DRIVER, COMMAND_BIND, 0, 0);
}

int udp_close(__attribute__ ((unused)) sock_handle_t *handle) {
  int bytes = sizeof(sock_addr_t);
  // call allow here to prevent any issues if close is called before bind
  // Driver 'closes' when 0 addr is bound to
  int err = allow(UDP_DRIVER, ALLOW_RX_CFG, (void *) zero_addr, 2 * bytes);
  if (err < 0) return err;

  memset(zero_addr, 0, 2 * bytes);
  err = command(UDP_DRIVER, COMMAND_BIND, 0, 0);
  return err;
}

static int tx_result;
static void tx_done_callback(int result,
                             __attribute__ ((unused)) int arg2,
                             __attribute__ ((unused)) int arg3,
                             void *ud) {
  tx_result      = result;
  *((bool *) ud) = true;
}

ssize_t udp_send_to(void *buf, size_t len,
                    sock_addr_t *dst_addr) {
  // Set dest addr
  // NOTE: bind() must be called previously for this to work
  // If bind() has not been called, command(COMMAND_SEND) will return ??
  int bytes = sizeof(sock_addr_t);
  memcpy(BUF_TX_CFG + bytes, dst_addr, bytes);

  // Set message buffer
  int err = allow(UDP_DRIVER, ALLOW_TX, buf, len);
  if (err < 0) return err;

  bool tx_done = false;
  err = subscribe(UDP_DRIVER, SUBSCRIBE_TX, tx_done_callback, (void *) &tx_done);
  if (err < 0) return err;

  err = command(UDP_DRIVER, COMMAND_SEND, 0, 0);
  if (err < 0) return err;
  // err == 1 indicates packet succesfully passed to radio synchronously.
  // However, wait for send_done to see if tx was successful, then return that result
  // err == 0 indicates packet will be sent asynchronously. Thus, 2 callbacks will be received.
  // the first callback will indicate if the packet was ultimately passed to the radio succesfully.
  // The second callback will only occur if the first callback is SUCCESS - and the second callback
  // in this case is the result of the tx itself (as reported by the radio).
  // However, it is the case in practice that if the first callback is success, the second callback
  // is received before the first callback finishes. Thus, no need to wait for two callbacks in either
  // case -- this design just ensures that errors in passing the packet down to the radio will still
  // be returned to the sender even when transmission occurs asynchronously.
  yield_for(&tx_done);
  return tx_result;
}

static int rx_result;
static void rx_done_callback(int result,
                             __attribute__ ((unused)) int arg2,
                             __attribute__ ((unused)) int arg3,
                             void *ud) {
  rx_result      = result;
  *((bool *) ud) = true;
}

ssize_t udp_recv_sync(void *buf, size_t len) {
  int err = allow(UDP_DRIVER, ALLOW_RX, (void *) buf, len);
  if (err < 0) return err;

  bool rx_done = false;
  err = subscribe(UDP_DRIVER, SUBSCRIBE_RX, rx_done_callback, (void *) &rx_done);
  if (err < 0) return err;

  yield_for(&rx_done);
  return rx_result;
}

ssize_t udp_recv(subscribe_cb callback, void *buf, size_t len) {

  // TODO: verify that this functions returns error if this app is not
  // bound to a socket yet. Probably allow should fail..?

  int err = allow(UDP_DRIVER, ALLOW_RX, (void *) buf, len);
  if (err < 0) return err;

  err = subscribe(UDP_DRIVER, SUBSCRIBE_RX, callback, NULL);
  return err;
}

int udp_list_ifaces(ipv6_addr_t *ifaces, size_t len) {

  if (!ifaces) return TOCK_EINVAL;

  int err = allow(UDP_DRIVER, ALLOW_CFG, (void *)ifaces, len * sizeof(ipv6_addr_t));
  if (err < 0) return err;

  return command(UDP_DRIVER, COMMAND_GET_IFACES, len, 0);
}

int udp_get_max_tx_len(void) {
  return command(UDP_DRIVER, COMMAND_GET_TX_LEN, 0, 0);
}


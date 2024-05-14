#pragma once

#include "../tock.h"
#include "syscalls/udp_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint16_t udp_port_t;

typedef struct ipv6_addr {
  uint8_t addr[16];
} ipv6_addr_t;

typedef struct sock_addr {
  ipv6_addr_t addr;
  udp_port_t port;
} sock_addr_t;

typedef struct sock_handle {
  sock_addr_t addr;
} sock_handle_t;


/// Callback for when a tx is completed.
typedef void (*libtock_udp_callback_send_done) (statuscode_t);

/// Callback for when a rx is completed.
typedef void (*libtock_udp_callback_recv_done) (statuscode_t, int);

// Creates a new datagram socket bound to an address.
// Returns 0 on success, negative on failure.
returncode_t libtock_udp_create_socket(sock_handle_t *handle, sock_addr_t *addr);

// Takes in an addess and a handle, and copies the address into
// the handle.
// Next, Binds to the address in handle if the address is not already
// bound by another port. Binding enables an app to receive on
// the bound port, and ensures that all sent packets will have
// the bound port as their src address.
// Returns 0 on successful bind, negative on failure.
returncode_t libtock_udp_bind(sock_handle_t *handle, sock_addr_t *addr, unsigned char *buf_bind_cfg);

// Closes a socket.
// Currently only one socket can exist per app, so this fn
// simply closes any connected socket
// Returns 0 on success, negative on failure.
returncode_t libtock_udp_close(sock_handle_t *handle);

// Receives messages from that socket asynchronously.
// The callback is passed the return code for the reception
// in addition to the length of the received packet.
// Returns 0 on successful bind, negative on failure.
returncode_t libtock_udp_recv(void *buf, size_t len, libtock_udp_callback_recv_done cb);

// Sends a message to the destination address asynchronously
// The callback is passed the return code for the transmission.
returncode_t libtock_udp_send(void *buf, size_t len,
                              sock_addr_t *dst_addr, libtock_udp_callback_send_done cb);

// Lists `len` interfaces at the array pointed to by `ifaces`.
// Returns the _total_ number of interfaces, negative on failure.
returncode_t libtock_udp_list_ifaces(ipv6_addr_t *ifaces, size_t len);

// Returns the maximum length udp payload that the app can transmit
returncode_t libtock_udp_get_max_tx_len(int* length);

#ifdef __cplusplus
}
#endif

/* vim: set sw=2 expandtab tw=80: */

#include <libtock/services/alarm.h>
#include <libtock/tock.h>
#include <libtock/util/streaming_process_slice.h>
#include <lwip/apps/httpd.h>
#include <lwip/dhcp.h>
#include <lwip/err.h>
#include <lwip/init.h>
#include <lwip/netif.h>
#include <lwip/prot/ethernet.h>
#include <lwip/sys.h>
#include <lwip/timeouts.h>
#include <stdio.h>
#include <string.h>

// ########## Settings

// Set this variable to `true` to enable debug print messages. These may slow
// down the application significantly.
#define DEBUG false

// Control whether the application shall provide a buffer for additional
// feedback on transmitted frames (e.g. their timestamps).
#define TX_FRAME_INFO_ENABLE 1

// Maximum size of a frame which can be transmitted with the
// `EthernetTapDriver`. Currently hard-coded to `1522 - 4` bytes, for
// an Ethernet frame with an 802.1q VLAN tag with a 1500 byte payload
// MTU, excluding the 4-byte FCS.
#define ETHERNET_MTU 1518

// How much space to allocate for frame receive buffers (in units of
// `ETHERNET_MTU + TAP_DRIVER_FRAME_HEADER_LEN`, at least 2):
#define RX_FRAME_BUFFER_FRAMES 8

// ########## Misc macros and constants

const uint32_t tap_driver_num = 0x30007;

#define debug_print(...)   \
  do {                     \
    if (DEBUG)             \
    printf(__VA_ARGS__); \
  } while (0)

// Transmission metadata feedback buffer len:
#if TX_FRAME_INFO_ENABLE
#define TAP_DRIVER_TX_FRAME_INFO_LEN 8
#endif

// Frame header prepended by the `EthernetTapDriver`:
#define TAP_DRIVER_FRAME_HEADER_LEN 12

// ########## Global buffers

// Buffer to transmit frames from / to the kernel:
static uint8_t tx_frame_buffer[ETHERNET_MTU];

// Two buffers used for for the RX_FRAMES streaming process slice. Each is able
// to hold a streaming process slice header, and `RX_FRAME_BUFFER_FRAMES / 2`
// Ethernet frames (encoded as TAP_DRIVER_FRAME_HEADER_LEN + payload, assumed to
// be ETHERNET_MTU bytes).
#define RX_FRAME_BUFFER_LEN \
  STREAMING_PROCESS_SLICE_HEADER_LEN \
  + ((ETHERNET_MTU + TAP_DRIVER_FRAME_HEADER_LEN) * (RX_FRAME_BUFFER_FRAMES / 2))
static uint8_t rx_frames_streaming_buffer_a[RX_FRAME_BUFFER_LEN];
static uint8_t rx_frames_streaming_buffer_b[RX_FRAME_BUFFER_LEN];

#if TX_FRAME_INFO_ENABLE
// Buffer for any additional feedback data from the TAP driver for transmitted
// frames (e.g. transmission timestamp for the PTP protocol).
uint8_t tx_frame_info_buffer[TAP_DRIVER_TX_FRAME_INFO_LEN];
#endif

// ########## Implementation

// Critical region protection function for lwip. Given that we aren't
// multithreaded and must voluntarily yield, this is a no-op.
sys_prot_t sys_arch_protect(void) {
  return NULL;
}

// Analogous to sys_arch_protect, this is also a no-op.
void sys_arch_unprotect(__attribute__((unused)) sys_prot_t pval) {}

// Provide a reference to the current time in milliseconds, such that LwIP can
// timeout TCP connections properly, etc.
uint32_t sys_now(void) {
  struct timeval tv;
  libtock_alarm_gettimeasticks(&tv);
  return (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
}

// Custom state of the LwIP tap driver netif:
typedef struct {
  uint8_t mac_addr[ETH_HWADDR_LEN];
  streaming_process_slice_state_t rx_streaming_state;
  bool rx_frame_received;
  bool tx_idle;
} tapif_state_t;

// TAP-driver frame reception upcall handler.
//
// We only use this as a mechanism for the kernel to inform the app that at
// least one frame has been placed into the streaming process slice. Hence this
// simply sets the `frame_received` flag passed in `data`:
static void frame_rx_upcall(__attribute__((unused)) int p0,
                            __attribute__((unused)) int p1,
                            __attribute__((unused)) int p2,
                            void*                       data) {
  struct netif* tapif  = data;
  tapif_state_t* state = tapif->state;

  state->rx_frame_received = true;
}

// A frame has been transmitted
static void frame_tx_upcall(int   statuscode_int,
                            int   flags_len_int,
                            int   transmission_id_int,
                            void* data) {
  uint32_t statuscode = (uint32_t)statuscode_int;
  __attribute__((unused)) uint16_t flags = (uint16_t)(flags_len_int >> 16);
  uint16_t len = (uint16_t)(flags_len_int & 0xFFFF);
  uint32_t transmission_id = (uint32_t)transmission_id_int;

  struct netif* tapif  = data;
  tapif_state_t* state = tapif->state;

  // Independent of success or error, this completes a transmission. We return
  // back to `tx_idle = true`:
  state->tx_idle = true;

  // Check if this is an error-upcall:
  if (statuscode != TOCK_STATUSCODE_SUCCESS) {
    printf("-> frame_tx_upcall error, statuscode: %04lx\r\n", statuscode);
    return;
  }

  debug_print("-> frame_tx_upcall: len %d, id %lu\r\n", len, transmission_id);
}

// LwIP packet (Ethernet frame) transmission callback:
static err_t tapif_output(struct netif* tapif,
                          struct pbuf*  p) {
  tapif_state_t* state = tapif->state;

  // Increment the link statistics
  LINK_STATS_INC(link.xmit);

  // Copy the frame into the kernel-shared buffer
  pbuf_copy_partial(p, tx_frame_buffer, p->tot_len, 0);

  // Allow tx_frame_buffer to TAP driver:
  allow_ro_return_t aoret = allow_readonly(tap_driver_num, 0, tx_frame_buffer,
                                           sizeof(tx_frame_buffer));
  if (!aoret.success) {
    printf("-> tapif_output failed allowing tx buffer: %d\r\n", aoret.status);
    return ERR_IF;
  }

  // Initiate the packet transmission. It is a synchronous operation, hence we
  // never race with another concurrent transmission and can
  // allow/command/unallow the transmission without waiting on a completion
  // upcall. However, we are only allowed to have one cocurrent pending
  // transmission at a time. Thus, wait for any concurrent transmission to
  // complete:
  yield_for(&state->tx_idle);

  // Now, indicate that we're busy transmitting a packet:
  state->tx_idle = false;

  syscall_return_t ret = command(tap_driver_num, 3, p->tot_len, 0);
  if (ret.type != TOCK_SYSCALL_SUCCESS) {
    printf("-> packet tx failed: %ld\r\n", ret.data[0]);

    // Synchronous error, transmission was aborted, we can transmit another
    // frame immediately:
    state->tx_idle = true;

    return ERR_IF;
  } else {
    return ERR_OK;
  }

  // Unallow tx_frame_buffer from TAP driver:
  aoret = allow_readonly(tap_driver_num, 0, NULL, 0);
  if (!aoret.success) {
    printf("-> tapif_output failed unallowing tx buffer: %d\r\n", aoret.status);
    return ERR_IF;
  }
}

// LWIP interface status update callback
static void tapif_status_callback(struct netif* netif) {
  printf("-> tapif_status: %s\r\n", ip4addr_ntoa(netif_ip4_addr(netif)));
}

// LWIP interface initialization hook
static err_t tapif_init(struct netif* netif) {
  tapif_state_t* tapif_state = netif->state;

  // Initialize the streaming process slice for receiving frames:
  returncode_t res = streaming_process_slice_init(&tapif_state->rx_streaming_state, tap_driver_num, 0,
                                                  rx_frames_streaming_buffer_a, sizeof(rx_frames_streaming_buffer_a),
                                                  rx_frames_streaming_buffer_b, sizeof(rx_frames_streaming_buffer_b));

  if (res != RETURNCODE_SUCCESS) {
    printf("-> tapif_init failed initializing streaming process slice: %d\r\n",
           res);
    return ERR_IF;
  }

#if TX_FRAME_INFO_ENABLE
  // Allow tx_pkt_info_buffer to TAP driver:
  allow_rw_return_t tx_frame_info_allow_ret =
    allow_readwrite(tap_driver_num, 1, tx_frame_info_buffer, sizeof(tx_frame_info_buffer));

  if (!tx_frame_info_allow_ret.success) {
    printf("-> tapif_init failed allowing tx pkt info buffer: %d\r\n",
           tx_frame_info_allow_ret.status);
    // The app might still work without this TX feedback, keep going.
  }
#endif

  // Register the interface hooks:
  netif->linkoutput = tapif_output;
  netif->output     = etharp_output;
  /* netif->output_ip6 = ethip6_output; */

  // Set interface flags:
  netif->mtu   = ETHERNET_MTU;
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
                 NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

  // Set the MAC address
  SMEMCPY(netif->hwaddr, tapif_state->mac_addr, ETH_HWADDR_LEN);
  netif->hwaddr_len = ETH_HWADDR_LEN;

  // Subscribe to incoming frame upcalls:
  subscribe_return_t sret;
  sret = subscribe(tap_driver_num, 0, frame_rx_upcall, netif);
  if (!sret.success) {
    printf("-> tapif_init failed subscribing rx upcall: %d\r\n", sret.status);
    return ERR_IF;
  }

  // Subscribe to outgoing frame upcalls
  sret = subscribe(tap_driver_num, 1, frame_tx_upcall, netif);
  if (!sret.success) {
    printf("-> tapif_init, failed subscribing tx upcalls: %d\r\n", sret.status);
    return ERR_IF;
  }

  printf("-> tapif_init: success\r\n");

  // Set link up immediately, we don't (yet) support link feedback from the
  // physical Ethernet adapter:
  netif_set_link_up(netif);

  return ERR_OK;
}

static void tapif_rx(struct netif* iface) {
  tapif_state_t* state = iface->state;

  // Deassert the pending frames flag _before_ we swap out the buffer. If we
  // race with a new reception, the worst that can happen is that we already
  // receive the frame, but the kernel reasserts the flag, which simply will
  // have the app swap the buffer again. If we were to do this after swapping
  // the buffer, we might deassert the flag despite there being a pending
  // frame waiting for reception:
  state->rx_frame_received = false;

  // Swap buffers and check if there is any new payload:
  uint8_t* current_ptr;
  uint32_t remaining_len;
  returncode_t res =
    streaming_process_slice_get_and_swap(&state->rx_streaming_state, &current_ptr, &remaining_len, NULL);
  if (res != RETURNCODE_SUCCESS) {
    printf("-> failed swapping frame receive buffer: %d\r\n", res);
    return;
  }

  // Process frames one by one, incrementing `current_ptr` and decrementing
  // `remaining_len`:
  while (remaining_len >= 12) {
    int iface_input_res;
    uint16_t flags, frame_len;
    uint64_t receive_ts;

    memcpy(&flags, current_ptr, sizeof(uint16_t));
    current_ptr   += sizeof(uint16_t);
    remaining_len -= sizeof(uint16_t);

    memcpy(&frame_len, current_ptr, sizeof(uint16_t));
    current_ptr   += sizeof(uint16_t);
    remaining_len -= sizeof(uint16_t);

    memcpy(&receive_ts, current_ptr, sizeof(uint64_t));
    current_ptr   += sizeof(uint64_t);
    remaining_len -= sizeof(uint64_t);

    if (frame_len > remaining_len) {
      printf("-> frame_len larger than remaining_len, illegal: %u vs. %lu\r\n",
             frame_len, remaining_len);
      return;
    }

    LINK_STATS_INC(link.recv);

    // Allocate a frame buffer from the global LWIP pool to copy the
    // received frame into.
    struct pbuf* received_frame = pbuf_alloc(PBUF_RAW, frame_len, PBUF_POOL);
    if (received_frame == NULL) {
      // We failed to allocate sufficient space in the pool
      printf("-> input_frames failed to allocate pbuf for %d bytes\r\n",
             frame_len);
      current_ptr   += frame_len;
      remaining_len -= frame_len;
      continue;
    }

    // Copy the frame into the frame buffer
    pbuf_take(received_frame, current_ptr, frame_len);
    current_ptr   += frame_len;
    remaining_len -= frame_len;

    iface_input_res = iface->input(received_frame, iface);
    if (iface_input_res != ERR_OK) {
      printf("-> iface.input failed, res = %d\r\n", iface_input_res);
      pbuf_free(received_frame);
    }
  }

  // The above loop should always process the entire payload:
  if (remaining_len != 0) {
    printf("-> input_frames has trailing bytes after consuming frames in "
           "streaming process slice (%lu bytes)\r\n",
           remaining_len);
  }
}

static void timeouts_alarm_callback(__attribute__((unused)) uint32_t now,
                                    __attribute__((unused)) uint32_t scheduled,
                                    void*                            opaque) {
  bool* alarm_fired = opaque;
  *alarm_fired = true;
}

int main(void) {
  printf("-> userspace tap network app\r\n");

  // Initialize the LWIP userspace networking stack
  lwip_init();

  // LwIP requires us to periodically invoke the `sys_check_timeouts`
  // function. Thus we set up a repeating alarm and check it in our main
  // loop.
  //
  // State for the repeating alarm callback to record that an alarm has fired:
  libtock_alarm_t timeouts_alarm_state;
  bool timeouts_alarm_fired = false;
  libtock_alarm_repeating_every_ms(100, timeouts_alarm_callback,
                                   &timeouts_alarm_fired,
                                   &timeouts_alarm_state);

  // Create a tap network device, consisting of two parts:
  // - the LwIP `struct netif`
  struct netif tapif;

  // - a custom `tapif_state_t`, containing state we use to implement the
  //   userspace side of the `EthernetTapDriver` capsule, including the state
  //   maintained for the RX streaming process slice
  tapif_state_t tapif_state = {
    .mac_addr = {0xda, 0xdf, 0x10, 0xcc, 0x45, 0x3d},
    .rx_frame_received  = false,
    .rx_streaming_state = {0},
    .tx_idle = true,
  };

  // IP address configuration:
  ip4_addr_t ip4_addr, ip4_netmask, ip4_gw;
  //
  // Static:
  IP4_ADDR(&ip4_addr, 192, 168, 1, 50);
  IP4_ADDR(&ip4_netmask, 255, 255, 255, 0);
  IP4_ADDR(&ip4_gw, 192, 168, 1, 1);
  // DHCP:
  /* ip4_addr = *IP4_ADDR_ANY; */
  /* ip4_netmask = *IP4_ADDR_ANY; */
  /* ip4_gw = *IP4_ADDR_ANY; */

  // Add the TAP network device. This will call the `tapif_init` function which
  // initializes allow buffers and upcalls. We pass in a reference to our
  // `tapif_state` struct, which will set the `(struct netif).state` field to
  // wire up our custom state to the LwIP netif struct.
  struct netif* netif_add_ret =
    netif_add(&tapif, &ip4_addr, &ip4_netmask, &ip4_gw, &tapif_state,
              tapif_init, netif_input);
  if (netif_add_ret == NULL) {
    printf("-> error initializing tapif\r\n");
    return 1;
  }

  tapif.name[0] = 't';
  tapif.name[1] = '0';

  netif_set_status_callback(&tapif, tapif_status_callback);
  netif_set_default(&tapif);
  netif_set_up(&tapif);

  printf("-> tap interface t0 added\r\n");

  // Uncomment the following line to obtain an IP address via DHCP:
  /* dhcp_start(&tapif); */

  // Start HTTP server (dummy application)
  httpd_init();

  // Main application loop
  while (1) {
    // Check for received frames and feed them to LwIP. This also resets
    // `tapif_state.rx_frame_received`:
    if (tapif_state.rx_frame_received) {
      tapif_rx(&tapif);
    }

    // Periodically check the LWIP timers
    if (timeouts_alarm_fired) {
      sys_check_timeouts();
      timeouts_alarm_fired = false;
    }

    // Wait for incoming packets or a timeout:
    while (tapif_state.rx_frame_received == false &&
           timeouts_alarm_fired == false) {
      yield();
    }
  }

  return 0;
}

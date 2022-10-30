/* vim: set sw=2 expandtab tw=80: */

#include <tock.h>
#include <stdio.h>
#include <string.h>
#include <internal/alarm.h>

#include <lwip/sys.h>
#include <lwip/prot/ethernet.h>
#include <lwip/netif.h>
#include <lwip/timeouts.h>
#include <lwip/init.h>
#include <lwip/dhcp.h>
#include <lwip/err.h>
#include <lwip/apps/httpd.h>

// Set this variable to `true` to enable debug print messages. These may slow
// down the application significantly.
#define DEBUG false
#define debug_print(...) \
    do { if (DEBUG) printf(__VA_ARGS__); } while (0)

// Control whether the application shall provide a buffer for additional
// feedback on transmitted packets (e.g. their timestamps).
#define TX_PACKET_INFO_ENABLE 1

// Miscellaneous constants:
#define ETHERNET_MTU 1522 // TODO: request from driver
#define TX_PACKET_INFO_LEN 8
const uint32_t tap_driver = 0x30003;

static uint8_t mac_addr[ETH_HWADDR_LEN] = {
    0xda, 0xdf, 0x10, 0xcc, 0x45, 0x3d };

// Global buffers for receive and transmit frames:
static uint8_t tx_frame_buffer[ETHERNET_MTU];
static uint8_t rx_frame_buffer[ETHERNET_MTU];

#if TX_PACKET_INFO_ENABLE
// Buffer for any additional feedback data from the TAP driver for transmitted
// packets (e.g. transmission timestamp for the PTP protocol).
uint8_t tx_packet_info_buffer[TX_PACKET_INFO_LEN];
#endif

// Define region protection code for lwip. Given that we aren't multithreaded
// and must voluntarily yield, this is a no-op.
sys_prot_t sys_arch_protect(void) {
    return NULL;
}

// In accordance with sys_arch_protect, this is also a no-op.
void sys_arch_unprotect(__attribute__((unused)) sys_prot_t pval) {}

// Provide a reference to the current time in milliseconds, such that LwIP can
// timeout TCP connections properly, etc.
uint32_t sys_now(void) {
    uint32_t now;
    alarm_internal_read(&now);
    return now;
}

// A received, but not yet processed LwIP packet. This is written in the packet
// RX callback and read + reset back to NULL in the main LwIP loop.
struct pbuf* pending_pkt = (struct pbuf*) NULL;

// TAP-driver packet reception upcall handler:
static void packet_rx_upcall(int p0, int p1, int p2,
			     __attribute__((unused)) void* data) {
    uint16_t len = (uint16_t) p0;
    uint16_t status = (uint16_t) p0;
    uint32_t ts_high = (uint32_t) p1;
    uint32_t ts_low = (uint32_t) p2;
    __attribute__((unused)) uint64_t timestamp =
	(uint64_t) ts_high << 32 | ts_low;

    // Check if this is an error upcall:
    if (status & (1 << 15)) {
	printf("-> packet_rx_upcall error, status: 0x%04x, len: 0x%04x\r\n",
	       status, len);
	return;
    }

    // Unallow the rx_frame_buffer from TAP driver to be able to access it:
    allow_rw_return_t awret =
	allow_readwrite(tap_driver, 0, NULL, 0);
    if (!awret.success) {
	printf("-> packet_rx_upcall failed unallowing rx buffer: %d\r\n",
	       awret.status);
	return;
    }

    // We must drop this packet if we can't store it because there's another
    // packet to be processed (i.e., pending_pkt != NULL).
    if (pending_pkt == NULL) {
	// Allocate a packet buffer from the global LWIP pool to copy the packet
	// into.
	pending_pkt = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

	if (pending_pkt != NULL) {
	    // Copy the frame into the packet buffer
	    pbuf_take(pending_pkt, rx_frame_buffer, len);
	} else {
	    // We failed to allocate sufficient space in the pool
	    printf("-> packet_rx_upcall failed to allocate pbuf\r\n");
	}
    } else {
	// Drop the packet and log a debug message:
	debug_print("-> packet dropped, queue full\r\n");
    }

    // Re-allow the rx_frame_buffer to the TAP driver:
    awret = allow_readwrite(tap_driver, 0, rx_frame_buffer,
			    sizeof(rx_frame_buffer));
    if (!awret.success) {
	printf("-> packet_rx_upcall failed allowing rx buffer: %d\r\n",
	       awret.status);
	return;
    }

    // Acknowledge packet reception (such that the kernel may insert a new
    // packet into the rx buffer):
    syscall_return_t ret = command(tap_driver, 7, 0, 0);
    if (ret.type != TOCK_SYSCALL_SUCCESS) {
	printf("-> packet acknowledgement failed\r\n");
    } else {
	debug_print("-> received and acked packet with len 0x%04x status 0x%04x"
		    "\r\n", len, status);
    }
}

// A packet has been transmitted
static void packet_tx_upcall(int status_len, int p_pkt_id,
			     __attribute__((unused)) int arg2,
			     __attribute__((unused)) void* data) {
    // Currently the kernel does not set len correctly!
    uint16_t status = (uint16_t) (status_len >> 16);
    __attribute__((unused)) uint16_t len = (uint16_t) status_len;
    __attribute__((unused)) uint32_t pkt_id = (uint32_t) p_pkt_id;

    // Check if this is an error-upcall:
    if (status & (1 << 15)) {
	printf("-> packet_tx_upcall error, status: %02x\r\n", status);
	return;
    }

    // If the kernel has been able to convey data in the tx packet info buffer
    // (i.e., the previous one has been acknowledged and it the allowed buffer
    // is of sufficient size), acknowledge it. This enables the driver to write
    // metadata for the next transmitted packet.
    if (status & (1 << 14)) {
	syscall_return_t ret = command(tap_driver, 8, 0, 0);
	if (ret.type != TOCK_SYSCALL_SUCCESS) {
	    printf("-> packet_tx_upcall: tx pkt info acknowledge failed\r\n");
	}
    }

    debug_print("-> packet_tx_upcall: len %d, id %lu\r\n", len, pkt_id);
}

// LwIP packet transmission callback:
static err_t tapif_output(__attribute__((unused)) struct netif *tapif,
			  struct pbuf *p) {
    // Increment the link statistics
    LINK_STATS_INC(link.xmit);

    // Copy the frame into the kernel-shared buffer
    pbuf_copy_partial(p, tx_frame_buffer, p->tot_len, 0);

    // Allow tx_frame_buffer to TAP driver:
    allow_ro_return_t aoret =
	allow_readonly(tap_driver, 0, tx_frame_buffer, sizeof(tx_frame_buffer));
    if (!aoret.success) {
	printf("-> tapif_output failed allowing tx buffer: %d\r\n",
	       aoret.status);
	return ERR_IF;
    }

    // Initiate the packet transmission. It is a synchronous operation, hence we
    // never race with another concurrent transmission:
    syscall_return_t ret = command(tap_driver, 6, p->tot_len, 0);
    if (ret.type != TOCK_SYSCALL_SUCCESS) {
	printf("-> packet tx failed: %ld\r\n", ret.data[0]);
	return ERR_IF;
    } else {
	return ERR_OK;
    }

    // Unallow tx_frame_buffer from TAP driver:
    aoret = allow_readonly(tap_driver, 0, NULL, 0);
    if (!aoret.success) {
	printf("-> tapif_output failed unallowing tx buffer: %d\r\n",
	       aoret.status);
	return ERR_IF;
    }
}

// LWIP interface status update callback
static void tapif_status_callback(struct netif *netif) {
    printf("-> tapif_status: %s\r\n", ip4addr_ntoa(netif_ip4_addr(netif)));
}

// LWIP interface initialization hook
static err_t tapif_init(struct netif *netif) {
    // Allow rx_frame_buffer to TAP driver:
    allow_rw_return_t awret =
	allow_readwrite(tap_driver, 0, rx_frame_buffer,
			sizeof(rx_frame_buffer));
    if (!awret.success) {
	printf("-> tapif_init failed allowing rx buffer: %d\r\n", awret.status);
	return ERR_IF;
    }

#if TX_PACKET_INFO_ENABLE
    // Allow tx_pkt_info_buffer to TAP driver:
    awret = allow_readwrite(tap_driver, 1, tx_packet_info_buffer,
			    sizeof(tx_packet_info_buffer));
    if (!awret.success) {
	printf("-> tapif_init failed allowing tx pkt info buffer: %d\r\n",
	       awret.status);
	// This is not necessarily critical, the app might still work without
	// this TX feedback.
    }
#endif

    // Register the interface hooks and flags
    netif->linkoutput = tapif_output;
    netif->output     = etharp_output;
    //netif->output_ip6 = ethip6_output;
    netif->mtu        = ETHERNET_MTU;
    netif->flags      =
	NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET
	| NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

    // Set the MAC address
    SMEMCPY(netif->hwaddr, mac_addr, ETH_HWADDR_LEN);
    netif->hwaddr_len = ETH_HWADDR_LEN;

    // Subscribe to incoming packet upcalls
    subscribe_return_t sret;
    sret = subscribe(tap_driver, 1, packet_rx_upcall, NULL);
    if (!sret.success) {
	printf("-> tapif_init failed subscribing rx upcall: %d\r\n",
	       sret.status);
	return ERR_IF;
    }

    // Subscribe to outgoing packet upcalls
    sret = subscribe(tap_driver, 2, packet_tx_upcall, NULL);
    if (!sret.success) {
	printf("-> tapif_init, failed subscribing tx upcalls: %d\r\n",
	       sret.status);
	return ERR_IF;
    }

    // Set this application to be the active user of the TAP network driver
    syscall_return_t ret = command(tap_driver, 1, 0, 0);
    if (ret.type != TOCK_SYSCALL_SUCCESS) {
	printf("-> tapif_init, failed getting exclusive access: %ld\r\n",
	       ret.data[0]);
	return ERR_IF;
    }

    printf("-> tapif_init: success\r\n");

    // Set link up immediately, we don't (yet) support link feedback from the
    // physical Ethernet adapter:
    netif_set_link_up(netif);

    return ERR_OK;
}

int main(void) {
    struct netif tapif;

    printf("-> userspace tap network app\r\n");

    // Initialize the LWIP userspace networking stack
    lwip_init();

    // Add the TAP network device
    netif_add(&tapif, IP4_ADDR_ANY, IP4_ADDR_ANY, IP4_ADDR_ANY, NULL,
	      tapif_init, netif_input);
    IP4_ADDR(&(tapif.ip_addr), 192, 168, 1, 50);
    IP4_ADDR(&(tapif.netmask), 255, 255, 255, 0);
    IP4_ADDR(&(tapif.gw), 192, 168, 1, 1);
    tapif.name[0] = 't';
    tapif.name[1] = '0';
    netif_set_status_callback(&tapif, tapif_status_callback);
    netif_set_default(&tapif);
    netif_set_up(&tapif);
    printf("-> tap interface t0 added\r\n");

    // Uncomment the following line to obtain an IP address via DHCP:
    //dhcp_start(&tapif);

    // Start HTTP server (dummy application)
    httpd_init();

    // Main application loop
    while(1) {
	// Check for received frames, and feed them to LWIP
	if(pending_pkt != NULL) {
	    LINK_STATS_INC(link.recv);

	    if(tapif.input(pending_pkt, &tapif) != ERR_OK) {
		pbuf_free(pending_pkt);
	    }

	    pending_pkt = NULL;
	}

	// Periodically check the LWIP timers
	sys_check_timeouts();

	// Wait for callbacks
	//
	// TODO: we should register a periodic callback to un-yield the process
	// every now and then, for LwIP to perform its periodic tasks
	// (e.g. check for timeouts).
	yield();
    }

    return 0;
}

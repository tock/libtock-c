// Don't operate in operating system mode
#define NO_SYS 1

// In turn, we can't use the seuqential-style APIs
#define LWIP_SOCKET 0
#define LWIP_NETCONN 0

// We want to have an interface status callback
#define LWIP_NETIF_STATUS_CALLBACK 1

// We provide our own timers
#define LWIP_TIMERS 1
#define LWIP_TIMERS_CUSTOM 0

// Use the libc allocator
#define MEM_LIBC_MALLOC 1
#define MEMP_MEM_MALLOC 1

// Align to word boundary
#define MEM_ALIGNMENT 4

// Support required protocols
#define LWIP_ARP 1
#define LWIP_ICMP 1
#define LWIP_BROADCAST_PING 1
#define LWIP_DHCP 1
#define LWIP_IGMP 1

#define LWIP_BROADCAST_PING 1
#define LWIP_MULTICAST_PING 1

// The LwIP PBUF data structure can be extended with additional fields:
// struct custom_pbuf_data {
//     uint64_t timestamp;
// };
// #define LWIP_PBUF_CUSTOM_DATA struct custom_pbuf_data custom_data;

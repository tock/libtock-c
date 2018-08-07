IP Sensor App
=============

An example app for platforms with sensors and an 802.15.4 radio that broadcasts
periodic sensor readings over the network. Currently, it sends raw 802.15.4
packets with statically configured PAN, source and destination addresses, but
as support is added for 6lowpan, Thread, etc, this app will evolve to use those
instead.

## Running

There are two options for testing this application. The first is to
use the link layer reception test, which provides more debugging information.
The second is to use the UDP layer reception test, which is more
representative of a real use of the userland IP layer. A description
of both options follows:

### Link Layer reception

Program the kernel on two imixs. On one, program the `radio_rx` app in
`userland/examples/tests/ieee802154/radio_rx` with the `PRINT_PAYLOAD` and
`PRINT_STRING` options enabled (this app simply prints received 802.15.4
packets to the console). On the other, program the `ip_sense` app.

You'll see packets printed on the console of the form:

```
Packet destination PAN ID: 0xabcd
Packet destination address: 0x0802
Packet source PAN ID: 0xabcd
Packet source address: 0x1540
Received packet with payload of 28 bytes from offset 11
2848 deg C; 3457%; 500 lux;

Packet destination PAN ID: 0xabcd
Packet destination address: 0x0802
Packet source PAN ID: 0xabcd
Packet source address: 0x1540
Received packet with payload of 28 bytes from offset 11
2848 deg C; 3456%; 500 lux;
```

### UDP Layer reception

Program the kernel on two imixs. On one, program the `udp_rx` app in
`userland/examples/tests/udp/udp_rx/udp_rx`.
On the other, program the `ip_sense` app.

You'll see packets printed on the console of the form:

```
[RF233] Received packet, sending to client
2 deg C; 1%; 3 lux;2 deg C; 1%; 3 lux;
```

The second line is simply the payload of the received UDP packet

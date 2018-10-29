UDP Button Press App
=============

An example app for platforms with a 802.15.4 radio that broadcasts
button presses over the network. Currently, it sends UDP packets
using 6lowpan to a single neighbor with an IP address known ahead of time.
This app is buggy in that it busy waits for a button press to occur.

## Running

To run this app, simply place the IP address of the destination node in the dst\_addr struct.
Notably, until Tock has neighbor discovery implemented, you also have to configure
the destination MAC address in the kernel (in boards/imix/src/main.rs).

### UDP Layer Reception Test

The best way to test this app is by using UDP reception on another Imix.
Program the kernel on two imixs. On one, program the `sensys_udp_rx` app in
`userland/examples/sensys_udp_rx.
On the other, program this app.

You'll see packets printed on the console.
These lines contain the payload of the received UDP packet.

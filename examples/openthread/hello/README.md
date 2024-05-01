# OpenThread Hello 

The openthread_hello application serves to demonstrate a working, albeit, incomplete 
implementation of OpenThread in libtock-c. As currently configured, this app successfully
configures, joins, and remains attached to a Thread network. Furthermore this app is 
able to send and receive UDP packets over the network. This README serves as a guide for
setting up a Thread network for this app to join, describes the current build system 
used, and details the numerous TODOs (and shortcomings) of the current implementation/build
system. The __OpenThread Port for Tock__ subsection provides context while the __Thread Network
Setup__ and __Tock OpenThread Setup__ provide "how to" guides for setting up the network
for the Tock device to attach to and the Tock device as well. This app will only work when using 
the nrf52840dk board.

## OpenThread Port for Tock

### Overview
The Tock/OpenThread port is implemented in the `libtock-c/libopenthread` directory. OpenThread 
utilizes a CMake build system. To port OpenThread to Tock, we alter the compile arguments 
provided to OpenThread, initiate the OpenThread build system, and then use the compiled 
static library as an external library within the `libtock-c` `make` build system. 

The OpenThread static library implements the logic needed to create, attach to, maintain,
and operate within a Thread network. OpenThread utilizes a platform abstraction layer 
that provides OpenThread with the needed "access" to hardware primitives (e.g. 
IEEE 802.15.4 radio, TRNG, alarm, flash). The platform abstraction layer "connects"
OpenThread to use the relevant `libtock-c` functionality. This is implemented in the
`libtock-c/libopenthread/platform` directory.

### TODOs and Shortcomings

- The Tock Thread device disconnects from the network (i.e. does not send child update request 
and subsequently times out) after ~7 minutes. This bug has been traced to an issue with the alarm
implementation but has not been resolved.
- The code size is quite large as is for the current implementation. There are numerous 
opportunities to reduce this (use hardware crypto instead of mbed crypto library).
- Channel switching is not yet implemented

## Thread Network Setup

A Thread app without a network to attach to and other nodes to talk to demonstrates very little.
As such, here are some steps for setting up a Thread network using OpenThread and an 
nrf52840dk. These steps are a simplified version of the complete tutorial [here](https://openthread.io/codelabs/openthread-hardware#13).
This requires one nrf52840dk to serve as the OpenThread router and one nrf52840dk to serve
as the Tock OpenThread minimal thread device (aka child).

### Install needed dependencies

You will need `nrfjprog`, `arm-none-eabi-gcc`, `screen` utilities in addition to the segger J-Link software.
The installation guides can be found [here](https://openthread.io/codelabs/openthread-hardware#1). _NOTE: 
the `arm-none-eabi-gcc` tooling is installed if you have previously used Tock/libtock-c_

In addition to these utilities, you will also need to clone the ot-nrf528xx repo:

```console
$ git clone --recursive https://github.com/openthread/ot-nrf528xx.git
```

### Build OpenThread for Nordic Device

We will now compile and build the Nordic OpenThread binary. This will be flashed to the 
nrf52840dk acting as the router. 

```console
$ cd ot-nrf528xx
$ script/build nrf52840 UART_trans
$ cd build/bin
$ arm-none-eabi-objcopy -O ihex ot-cli-ftd ot-cli-ftd.hex
$ nrfjprog -f nrf52 -s {YOUR_JTAG_ID} --verify --chiperase --program ot-cli-ftd.hex --reset
```

NOTES:
- depending on how you installed and configured nrfjprog, this may need to be run as a script
within the nrfjprog directory
- YOUR_JTAG_ID can be found on the white sticker near the center of your board this is the 
bottom number (e.g. 683780909).

### Nordic OpenThread CLI

Congratulations, you have now flashed the OpenThread firmware to the router. We will now 
use the command line interface to interact with and start the Thread network.

First, we must find which USB the device is attached to. (This can easily be found with the 
`$ ls /dev/ttyACM*` command with the nrf52840dk powered off and then on). 

With this {NUMBER}, we now:

```console
$ screen /dev/ttyACM{NUMBER} 115200
```

This will open the OpenThread CLI running on the nrf52840dk. Press enter and you 
should see 

```console
>
```

We now run a series of commands to initialize the Thread network on this device. 

```console
> dataset init new
```

```console
> dataset networkkey 00112233445566778899aabbccddeeff
```

```console
> dataset channel 26
```

```console
> dataset panid 0xabcd
```

```console
> dataset commit active
```

We now confirm these changes were correctly entered. The following command
will display the dataset:
```console
> dataset
```

We now must initialize the device's IP address

```console
> ifconfig up
```

Finally, we start the Thread network. This will begin wih a series of Link Requests
and Parent Requests. After which, this device will become a router. 
```console
> thread start
```

To confirm this has correctly occurred, we wait around 15 seconds and run the 
command:

```console
> state
```

If the device has correctly been configured, this should return `leader`. Our 
router is now active and will form the Thread network our Tock device will attach 
to.

## Tock OpenThread Setup

Now that the OpenThread router is active, we can begin setting up the Tock 
OpenThread device. _(Note: it is easiest to have a separate terminal session
for interacting with router through the OpenThread CLI above)_

Before building the OpenThread library, we must first obtain the OpenThread 
submodule.

```console
$ cd libtock-c/examples/openthread_hello
$ make
```

This will initiate the build process that will build the OpenThread and 
platform library. Following this, install the app to the board with `tockloader install`.

## UDP Send / Receive Functionality 

_NOTE: there is on occassion strange behavior if the port is not correctly closed. If this 
occurs it is best to restart the thread network (i.e. `thread stop` reboot device and 
reinitialize the network)_

### Tock UDP Receive Demonstration

On initialization, the ipaddr is printed to the Tock console.

```console 
$ tockloader listen
```

```console
[IPADDR] fe80:0:0:0:e094:2134:8353:928f
```

The above value will be unique to the given instance (OpenThread generates this from
the EUI-64 upon each initialization). This value is the {TOCK_DEVICE_IPADDR}

We then configure and send packets from the Nordic OpenThread router.

```console 
> udp open
```

```console
> udp connect {TOCK_DEVICE_IPADDR} 1212
```

```console
> udp send hellothere
```

```console 
> udp close
```

### Tock UDP Send Demonstration

We must first configure the Nordic OpenThread router to receive through the CLI.

```console
> udp open
```
```console
> udp bind :: 1212
```

In the main loop, uncomment `sendUDP()`.

You should see something of the form:

```console
34 bytes from fdef:f8ba:cb5b:f692:c37d:1fbe:82b0:abab 1212 Hello OpenThread World from Tock!
```

```console
> udp close
```

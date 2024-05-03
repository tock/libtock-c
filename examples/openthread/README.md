# OpenThread and Tock

## Overview
The Tock/OpenThread port is implemented in the `libtock-c/libopenthread` directory.

The OpenThread static library implements the logic needed to create, attach to, maintain,
and operate within a Thread network. OpenThread utilizes a platform abstraction layer 
that provides OpenThread with the needed "access" to hardware primitives (e.g. 
IEEE 802.15.4 radio, TRNG, alarm, flash). The platform abstraction layer "connects"
OpenThread to use the relevant `libtock-c` functionality. This is implemented in the
`libtock-c/libopenthread/platform` directory.

Each app demonstrates the helper methods and OpenThread APIs needed to configure and 
use the Thread network.

## Thread Network Setup

A Thread app without a network to attach to and other nodes to talk to demonstrates very little.
As such, here are some steps for setting up a Thread network using OpenThread and an 
nrf52840dk. These steps are a simplified version of the complete tutorial [here](https://openthread.io/codelabs/openthread-hardware#13).
This requires one nrf52840dk to serve as the OpenThread router and one nrf52840dk to serve
as the Tock OpenThread minimal thread device (aka child). Tock's current OpenThread port 
has been thoroughly tested as a minimal thread device. However, the Tock build system possess
the tooling need to compile a full thread device OpenThread app.

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
submodule. This will automatically occur as part of building the app.

```console
$ cd libtock-c/examples/openthread_hello
$ make
```

This will initiate the build process that will build the OpenThread and 
platform library. Following this, install the app to the board with `tockloader install`.

### TODOs and Shortcomings
- The code size is quite large as is for the current implementation. There are numerous 
opportunities to reduce this (use hardware crypto instead of mbed crypto library).
- Channel switching is not yet implemented
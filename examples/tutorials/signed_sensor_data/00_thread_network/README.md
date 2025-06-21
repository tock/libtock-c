# UDP Receive Functionality

Please refer to the README in the above directory for a general Tock/OpenThread
overview and guide to setting up the OpenThread app.

_NOTE: there is on occasion strange behavior if the port is not correctly
closed. If this occurs it is best to restart the thread network (i.e. `thread
stop` reboot device and reinitialize the network)_. This is due to strange
behavior with the OpenThread CLI on the nordic device.

## Tock UDP Receive Demonstration

On initialization, the IP address is printed to the Tock console.

```console
$ tockloader listen
```

```console
[IPADDR] fe80:0:0:0:e094:2134:8353:928f
```

The above value is unique to the given instance (OpenThread generates this from
the EUI-64 upon each initialization). This value is the `{TOCK_DEVICE_IPADDR}`.

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

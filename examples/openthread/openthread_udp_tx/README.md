# UDP Send Functionality 

Please refer to the README in the above directory for a general Tock/OpenThread overview
and guide to setting up the OpenThread app.

_NOTE: there is on occassion strange behavior if the port is not correctly closed. If this 
occurs it is best to restart the thread network (i.e. `thread stop` reboot device and 
reinitialize the network)_. This is due to strange behavior with the OpenThread CLI 
on the nordic device.

## Tock UDP Send Demonstration

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
# Relay Request Round Robin - libtocksync

Uses the synchronous libtocksync drivers for string name registration/discovery
(IPC Registry String Name) and requests/responses (IPC Relay Request).

This is intended to be loaded three separate times with separate SERVICE
configurations. The end result will be three processes connected in round-robin
fashion, Service0 -> Service1 -> Service2 -> (back to Service0). Each receives
from the prior service and sends to the next service. To break the deadlock,
Service2 always sends first while the others always receive first.

To make this easier to test, a custom Make command has been added to this
application. To build all three apps and load them onto your board use `make
ROUNDROBIN`. When all three are loaded the expected behavior is:

```
Service2: Starting.
Service0: Starting.
Service1: Starting.
Service0: IPC Registry String Name capsule exists.
Service1: IPC Registry String Name capsule exists.
Service0: IPC Relay Request capsule exists.
Service1: IPC Relay Request capsule exists.
Service0: IPC String Name registered.
Service1: IPC String Name registered.
Service0: Couldn't find service "Service2" yet.
Service1: IPC String Name discovered service "Service0". IPC ID: 0000000000000000
Service2: IPC Registry String Name capsule exists.
Service1: Couldn't find service "Service2" yet.
Service2: IPC Relay Request capsule exists.
Service2: IPC String Name registered.
Service2: IPC String Name discovered service "Service1". IPC ID: 0000000000000001
Service2: IPC String Name discovered service "Service0". IPC ID: 0000000000000000
tock$ Service0: IPC String Name discovered service "Service2". IPC ID: 0000000000000002
Service0: IPC String Name discovered service "Service1". IPC ID: 0000000000000001
Service2: Got response from 0000000000000000! Value is now: 1
Service1: IPC String Name discovered service "Service2". IPC ID: 0000000000000002
Service0: Got response from 0000000000000001! Value is now: 2
Service1: Got response from 0000000000000002! Value is now: 3
Service2: Got response from 0000000000000000! Value is now: 4
Service0: Got response from 0000000000000001! Value is now: 5
...
```


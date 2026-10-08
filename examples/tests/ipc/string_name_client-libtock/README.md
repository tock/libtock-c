# String Name Client - libtock

Discovers an IPC service via a provided string name using the asynchronous
libtock IPC Registry String Name driver.

This is intended to be loaded alongside [string_name_server-libtock](../string_name_server-libtock). When
loaded with the String Name Server the expected behavior is:

```
Initialization complete. Entering main loop.
SERVER: Starting server.
CLIENT: Starting client.
SERVER: IPC Registry String Name capsule exists.
CLIENT: IPC Registry String Name capsule exists.
CLIENT: Discovering: "Test_Service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Service not found.
SERVER: Registering with string name: "Test_Service"
SERVER: Registration started.
SERVER: Registration complete. Succeeded!
CLIENT: Discovering: "Test_Service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Succeeded! Found IPC ID: 0000000000000001
```

Note that above IPC IDs may not match depending on the order the apps are
loaded and the configuration of your board.


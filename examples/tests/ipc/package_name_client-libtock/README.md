# Package Name Client - libtock

Discovers an IPC service via TBF Header Package Name using the asynchronous
libtock IPC Registry Package Name driver.

This is intended to be loaded alongside [package_name_server-libtock](../package_name_server-libtock). When
loaded with the Package Name Server the expected behavior is:

```
SERVER: Starting server.
CLIENT: Starting client.
SERVER: IPC Registry Package Name capsule exists.
CLIENT: IPC Registry Package Name capsule exists.
CLIENT: Discovering: "tbf_package_name_service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Service not found.
SERVER: Registering with my package name (see Makefile).
SERVER: Registration started.
SERVER: Registration complete. Succeeded!
CLIENT: Discovering: "tbf_package_name_service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Succeeded! Found IPC ID: 0000000000000001
```

Note that above IPC IDs may not match depending on the order the apps are
loaded and the configuration of your board.


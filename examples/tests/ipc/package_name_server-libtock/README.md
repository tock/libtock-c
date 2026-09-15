# Package Name Server - libtock

Registers as an IPC service via TBF Header Package Name using the asynchronous
libtock IPC Registry Package Name driver.

The Package Name for this server is specified in the [Makefile](./Makefile) as
`PACKAGE_NAME = tbf_package_name_service`.

This is intended to be loaded alongside [package_name_client-libtock](../package_name_client-libtock). When
loaded with the Package Name Client the expected behavior is:

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


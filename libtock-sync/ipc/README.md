# Libtock-sync IPC

This directory holds synchronous drivers for interprocess communication (IPC) support.

Currently the following drivers exist:

* **IPC Registry Package Name**

  Provides process registration/discovery via TBF header package names.

* **IPC Registry String Name**

  Provides process registration/discovery via arbitrary string names.

* **IPC Relay Request**

  Provides single-copy allow-to-allow requests and responses between processes.

  For client request functionality with a timeout, see [libtock-sync/services/ipc_relay_request_timeout](../services/ipc_relay_request_timeout.h)


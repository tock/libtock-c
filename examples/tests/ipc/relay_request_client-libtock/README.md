# Relay Request Client - libtock

Discovers an IPC service via a provided string name using the asynchronous
libtock IPC Registry String Name driver and then periodically sends requests to
it using the asynchronous libtock IPC Relay Request driver.

This is intended to be loaded alongside [relay_request_server-libtock](../relay_request_server-libtock). When
loaded with the Relay Request Server the expected behavior is:

```
SERVER: Starting server.
CLIENT: Starting client.
SERVER: IPC Registry String Name capsule exists.
CLIENT: IPC Registry String Name capsule exists.
SERVER: IPC Relay Request capsule exists.
CLIENT: IPC Relay Request capsule exists.
SERVER: Enabled request waiting callbacks.
CLIENT: Discovering: "Request_Service"
SERVER: Registering with string name: "Request_Service"
CLIENT: Discovery started.
SERVER: Registration started.
CLIENT: Discovery complete. Service not found.
SERVER: Registration complete. Succeeded!
tock$ CLIENT: Discovering: "Request_Service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Succeeded! Found IPC ID: 0000000000000001
CLIENT: Sending request with data { AA BB CC DD EE FF }
CLIENT: Request initiated.
SERVER: New request waiting from IPC ID: 0000000000000000
SERVER: Got a request from IPC ID: 0000000000000000
SERVER: Request data {AA BB CC DD EE FF }
CLIENT: Waiting for response (1 of 10)
CLIENT: Waiting for response (2 of 10)
CLIENT: Waiting for response (3 of 10)
SERVER: Sent response.
CLIENT: Received response. Data { 12 34 56 78 }
CLIENT: Sending request with data { AA BB CC DD EE FF }
...
```

Note that above IPC IDs may not match depending on the order the apps are
loaded and the configuration of your board.

## Fault Handling

This app is designed to handle faults gracefully. Communication is
intentionally slowed down to provide time to manually trigger faults between
certain actions to see the response. Using the Process Console is the easiest
way to intentionally cause a fault to test this. If your board is configured
with the process Fault Policy `RestartFaultPolicy`, then after one process
restarts discovery will be re-initiated and communication will resume.

For example, you can fault the server with the command `fault
relay_request_server-libtock`.

```
SERVER: Got a request from IPC ID: 0000000000000000
SERVER: Request data {AA BB CC DD EE FF }
CLIENT: Waiting for response (1 of 10)
CLIENT: Waiting for response (2 of 10)
CLIENT: Waiting for response (3 of 10)
SERVER: Sent response.
CLIENT: Received response. Data { 12 34 56 78 }
$ fault relay_request_server-libtock

Process relay_request_server-libtock now faulted
SERVER: Starting server.
SERVER: IPC Registry String Name capsule exists.
SERVER: IPC Relay Request capsule exists.
SERVER: Enabled request waiting callbacks.
SERVER: Registering with string name: "Request_Service"
SERVER: Registration started.
SERVER: Registration complete. Succeeded!
CLIENT: Sending request with data { AA BB CC DD EE FF }
CLIENT: Server disappeared? Discover it again.
CLIENT: Discovering: "Request_Service"
CLIENT: Discovery started.
CLIENT: Discovery complete. Succeeded! Found IPC ID: 0000000000000002
CLIENT: Sending request with data { AA BB CC DD EE FF }
CLIENT: Request initiated.
SERVER: New request waiting from IPC ID: 0000000000000000
SERVER: Got a request from IPC ID: 0000000000000000
SERVER: Request data {AA BB CC DD EE FF }
```


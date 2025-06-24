Writing a libtock-c System Call Driver
======================================

This guide covers how to implement support in `libtock` for a new system call
driver.


## Setup


- `[name]`: String which identifies the system call driver.
- `[category]`: The general type of the system call.

The categories are:

- `crypto`: Cryptography interfaces.
- `display`: Screens and other displays.
- `interface`: Human-computer interfaces.
- `kernel`: Tock-specific kernel-level drivers.
- `net`: Networking drivers.
- `peripherals`: Interfaces for chip peripherals.
- `sensors`: Sensors.
- `storage`: Various storage drivers.


## Syscall APIs

All supported system calls (i.e., command, allow, subscribe, etc.) for a
particular driver must be wrapped in simple functions.

| Characteristic   | Value                                           |
|------------------|-------------------------------------------------|
| Location         | `libtock/[category]/syscalls`                   |
| Source File Name | `libtock/[category]/syscalls/[name]_syscalls.c` |
| Header File Name | `libtock/[category]/syscalls/[name]_syscalls.h` |

### Header File

The `[name]_syscalls.h` file must contain a `#define` called `DRIVER_NUM_[NAME]`
with the driver number.

_All_ header files must be wrapped in `extern "C" { ... }` if the header file
is used in a C++ app.

#### Example:

```c
#pragma once

#include "../../tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_[NAME] 0x00000

// Other defines and signatures go here.

#ifdef __cplusplus
}
#endif
```

### Upcalls

Each supported upcall must have its own function.

The signature is:

```c
returncode_t libtock_[name]_set_upcall(subscribe_upcall callback, void* opaque);
```

If only one upcall is supported, the function name must be `[name]_set_upcall`.

If more than one upcall is supported, the function names must start with
`libtock_[name]_set_upcall_` followed by a description of what the upcall is
used for.


#### Example:

```c
returncode_t libtock_[name]_set_upcall(subscribe_upcall callback, void* opaque) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_[NAME], 0, callback, opaque);
  return tock_subscribe_return_to_returncode(sval);
}
```

### Allows

Each supported allow (readonly, readwrite, userspace_read) must have its own
function.

The signature is:

- Read-Only Allow:
    ```c
    returncode_t libtock_[name]_set_readonly_allow(const uint8_t* buffer, uint32_t len);
    ```

- Read-Write Allow:
    ```c
    returncode_t libtock_[name]_set_readwrite_allow(uint8_t* buffer, uint32_t len);
    ```

- Userspace Read Allow:
    ```c
    returncode_t libtock_[name]_set_userspace_read_allow(uint8_t* buffer, uint32_t len);
    ```

If only one allow is supported, the function name must be
`libtock_[name]_set_[type]_allow`.

If more than one allow is supported, the function names must start with
`libtock_[name]_set_[type]_allow_` followed by a description of what the allow
is used for.

#### Example:

```c
returncode_t libtock_[name]_set_readonly_allow_[desc](const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}
```

```c
returncode_t libtock_[name]_set_readwrite_allow_[desc](uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}
```

```c
returncode_t libtock_[name]_set_userspace_read_allow_[desc](uint8_t* buffer, uint32_t len) {
  allow_userspace_r_return_t aval = allow_userspace_read(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_userspace_r_return_to_returncode(aval);
}
```

### Commands

Each supported command must have its own function.

The signature is:

```c
returncode_t libtock_[name]_command_[desc](<arguments>);
```

For every command, the function names must start with `libtock_[name]_command_`
followed by a description of what the command is used for.

### Example:

```c
returncode_t libtock_[name]_command_[desc](void) {
  syscall_return_t cval = command(DRIVER_NUM_[NAME], 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
```


### Exists

There must be a function to check for syscall driver existence.

Signature:

```
bool libtock_[name]_exists(void);
```

Example:

```c
bool libtock_[name]_exists(void) {
  return driver_exists(DRIVER_NUM_[NAME]);
}
```


## Asynchronous APIs

All common system call operations should have asynchronous versions.

These asynchronous APIs must not use or include any internal/global state.

| Characteristic   | Value                         |
|------------------|-------------------------------|
| Location         | `libtock/[category]`          |
| Source File Name | `libtock/[category]/[name].c` |
| Header File Name | `libtock/[category]/[name].h` |

### Header Files

The `[name].h` header file must look like:

```c
#pragma once

#include "../tock.h"
#include "syscalls/[name]_syscalls.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function signatures go here.

#ifdef __cplusplus
}
#endif
```

The `[name].h` header file must include the syscalls header.

### Defining a Callback for Asynchronous Operations

For every type of callback the system call can generate, the library must have a
function signature defined as a type. The format is:

```c
typedef void (*libtock_[name]_callback_[desc])(returncode_t, int);
```

### Functions for Operations

Every operation the libtock library should expose must be defined. The return
type should generally be `returncode_t`. As these are asynchronous operations,
they should have the last argument be a callback function pointer.

```c
returncode_t libtock_[name]_[desc](<arguments>, libtock_[name]_callback_[desc] cb);
```

### Example


For example, a library called "sensor" with a sensor read operation should look
like this:

Define a suitable callback for the library:

```c
typedef void (*libtock_sensor_callback_reading)(returncode_t, int);
```

Define a upcall function to be passed to the kernel:

```c
static void sensor_temp_upcall(int                          ret,
                               int                          val,
                               __attribute__ ((unused)) int unused0,
                               void*                        opaque) {
  libtock_sensor_callback_reading cb = (libtock_sensor_callback_reading) opaque;
  cb(ret, val);
}
```

Define an external function for applications to use to run the asynchronous
operation:

```c
returncode_t libtock_sensor_read(libtock_sensor_callback_reading cb) {
  returncode_t ret;

  ret = libtock_sensor_set_upcall(sensor_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = libtock_sensor_command_read();
  return ret;
}
```

## Synchronous APIs

Most system call interfaces will want to provide a synchronous API as well. This
requires another file for syscalls.

All synchronous APIs MUST use the
[Yield-WaitFor](https://book.tockos.org/trd/trd104-syscalls.html#413-yield-waitfor)
(`yield_wait_for()`) variant of the yield syscall. This ensures predictable
behavior for `libtock-sync` users because Yield-WaitFor ensures that no other
application upcall will run until the synchronous API has finished.

### Synchronous Syscall APIs

| Characteristic   | Value                                                |
|------------------|------------------------------------------------------|
| Location         | `libtock-sync/[category]/syscalls`                   |
| Source File Name | `libtock-sync/[category]/syscalls/[name]_syscalls.c` |
| Header File Name | `libtock-sync/[category]/syscalls/[name]_syscalls.h` |

### Header File

The `[name]_syscalls.h` must be wrapped in `extern "C" { ... }` if the header
file is used in a C++ app.

#### Example:

```c
#pragma once

#include <libtock/tock.h>
#include <libtock/[category]/syscalls/[name]_syscalls.h>

#ifdef __cplusplus
extern "C" {
#endif

// Signatures go here.

#ifdef __cplusplus
}
#endif
```

### Yield-WaitFor

Each supported upcall must have a matching Yield-WaitFor call. This function
calls `yield_wait_for()` and interprets the `yield_waitfor_return_t` return
value arguments.

The signature is:

```c
returncode_t libtocksync_[name]_yield_wait_for(<arguments>);
```

If only one upcall is supported, the function name must be
`libtocksync_[name]_yield_wait_for`.

If more than one upcall is supported, the function names must start with
`libtocksync_[name]_yield_wait_for_` followed by the same description of what
the upcall is used for from the `libtock` library.

The arguments must be appropriately named pointers to data types returned by the
upcall, except for any returned ReturnCode. The ReturnCode must be returned from
the function.

#### Example:

```c
returncode_t libtocksync_[name]_yield_wait_for(int* value) {
  yield_waitfor_return_t ret;
  ret = yield_wait_for(DRIVER_NUM_[NAME], 0);
  if (ret.data0 != RETURNCODE_SUCCESS) return ret.data0;

  *value = (int) ret.data1;
  return RETURNCODE_SUCCESS;
}
```

### Synchronous APIs

The asynchronous operations should have matching synchronous versions using
Yield-WaitFor internally.

| Characteristic   | Value                              |
|------------------|------------------------------------|
| Location         | `libtock-sync/[category]`          |
| Source File Name | `libtock-sync/[category]/[name].c` |
| Header File Name | `libtock-sync/[category]/[name].h` |

### Header Files

The libtock-sync `[name].h` header file must look like:

```c
#pragma once

#include "syscalls/temperature_syscalls.h"
#include <libtock/tock.h>

#ifdef __cplusplus
extern "C" {
#endif

// Function signatures go here.

#ifdef __cplusplus
}
#endif
```

### Synchronous APIs

For our sensor example:

Create a struct to be set by the asynchronous callback:

```c
struct data {
  bool fired;
  int val;
  returncode_t result;
};

static struct data result = { .fired = false };
```

Define a static callback function to pass to the asynchronous implementation:

```c
static void sensor_cb(returncode_t ret, int val) {
  result.val = val;
  result.fired = true;
  result.result = ret;
}
```

Define the external function in the `libtocksync_` name space for the sync
operation:

```c
returncode_t libtocksync_sensor_read(int* val) {
  returncode_t err;

  err = libtock_sensor_command_read(sensor_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the operation to finish.
  err = libtock_temperature_yield_wait_for(val);
  return err;
}
```

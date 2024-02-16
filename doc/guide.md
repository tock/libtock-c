Writing a libtock-c System Call Driver
======================================

This guide covers how to implement support in `libtock` for a new system call
driver.


## Setup


- `[name]`: String which identifies the system call driver.
- `[category]`: The general type of the system call.


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

#### Example:

```c
#define DRIVER_NUM_[NAME] 0x00000
```

### Upcalls

Each supported upcall must have its own function.

The signature is:

```c
returncode_t set_upcall(subscribe_upcall callback, void* opaque);
```

If only one upcall is supported, the function name must be `[name]_set_upcall`.

If more than one upcall is supported, the function names must start with
`[name]_set_upcall_` followed by a description of what the upcall is used for.


#### Example:

```c
returncode_t [name]_set_upcall(subscribe_upcall callback, void* opaque) {
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
    int set_readonly_allow(const uint8_t* buffer, uint32_t len);
    ```

- Read-Write Allow:
    ```c
    int set_readwrite_allow(uint8_t* buffer, uint32_t len);
    ```

- Userspace Read Allow:
    ```c
    int set_userspace_read_allow(uint8_t* buffer, uint32_t len);
    ```

If only one allow is supported, the function name must be
`[name]_set_[type]_allow`.

If more than one allow is supported, the function names must start with
`[name]_set_[type]_allow_` followed by a description of what the allow is used
for.

#### Example:

```c
returncode_t [name]_set_readonly_allow_[desc](const uint8_t* buffer, uint32_t len) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_ro_return_to_returncode(aval);
}
```

```c
returncode_t [name]_set_readwrite_allow_[desc](uint8_t* buffer, uint32_t len) {
  allow_rw_return_t aval = allow_readwrite(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_rw_return_to_returncode(aval);
}
```

```c
returncode_t [name]_set_userspace_read_allow_[desc](uint8_t* buffer, uint32_t len) {
  allow_userspace_r_return_t aval = allow_userspace_read(DRIVER_NUM_[NAME], 0, (void*) buffer, len);
  return tock_allow_userspace_r_return_to_returncode(aval);
}
```

### Commands

Each supported command must have its own function.

The signature is:

```c
returncode_t command(<arguments>);
```

For every command, the function names must start with `[name]_command_` followed
by a description of what the command is used for.

### Example:

```c
returncode_t [name]_command_[desc](void) {
  syscall_return_t cval = command(DRIVER_NUM_[NAME], 1, 0, 0);
  return tock_command_return_novalue_to_returncode(cval);
}
```


### Exists

There must be a function to check for syscall driver existence.

Signature:

```
bool [name]_exists(void);
```

Example:

```c
bool [name]_exists(void) {
  return driver_exists(DRIVER_NUM_[NAME]);
}
```


## Asynchronous APIs

All common system call operations should have asynchronous versions.

| Characteristic   | Value                         |
|------------------|-------------------------------|
| Location         | `libtock/[category]`          |
| Source File Name | `libtock/[category]/[name].c` |
| Header File Name | `libtock/[category]/[name].h` |

For example, a sensor read operation should look like this:

Define a suitable callback for the library:

```c
typedef void (*sensor_callback)(returncode_t, int);
```

Define a upcall function to be passed to the kernel:

```c
static void sensor_temp_upcall(int                          ret,
                               int                          val,
                               __attribute__ ((unused)) int unused0,
                               void*                        opaque) {
  sensor_callback cb = (sensor_callback) ud;
  cb(ret, val);
}
```

Define an external function for applications to use to run the asynchronous
operation:

```c
returncode_t sensor_read(sensor_callback cb) {
  int ret;

  ret = sensor_set_upcall(sensor_temp_upcall, cb);
  if (ret != RETURNCODE_SUCCESS) return ret;

  ret = sensor_command_read();
  return ret;
}
```

## Synchronous APIs

Most system call interfaces will want to provide a synchronous API as well.

| Characteristic   | Value                              |
|------------------|------------------------------------|
| Location         | `libtock-sync/[category]`          |
| Source File Name | `libtock-sync/[category]/[name].c` |
| Header File Name | `libtock-sync/[category]/[name].h` |

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
  result.temp = val;
  result.fired = true;
  result.result = ret;
}
```

Define the external function in the `libtocksync_` name space for the sync
operation:

```c
returncode_t libtocksync_sensor_read(int* val) {
  int err;
  result.fired = false;

  err = sensor_read(sensor_cb);
  if (err != RETURNCODE_SUCCESS) return err;

  // Wait for the callback.
  yield_for(&result.fired);
  if (result.result != RETURNCODE_SUCCESS) return result.result;

  *val = result.val;
  return RETURNCODE_SUCCESS;
}
```

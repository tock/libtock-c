Core Libtock Library
====================

The libtock library provides core functionality for writing Tock apps in C. In
particular this library provides three main components:

1. The `crt0.c` file which executes first when an app first starts. This
   configures memory to initialize the C runtime.
2. Interfaces for libc to use common C functions (e.g., `printf()`).
3. Drivers for the system call APIs the Tock kernel provides (e.g., timers and
   sensors).

The libtock library is **complete asynchronous**, meaning it does not call
`yield()`. All exposed functionality can safely be used for asynchronous
operation. Using the libtock library exclusively allows an app to avoid mixing
asynchronous and synchronous operations.

_Note_: Users who prefer synchronous functions can use the libtock-sync library.

Library Usage
-------------

The format of the libtock library is documented in the [style
guide](../doc/guide.md).

To include an API in your app, you should include its header file:

```c
#include <libtock/{category}/{api}.h>
```

All library function names start with `libtock_{api}_`. General usage
functionality is provide via the `{api}.h` header file. All of the low level
syscall operations are found in `libtock/{category}/syscalls/{api}_syscalls.h`
if you need to call that driver's system calls directly.

Functions that start an asynchronous operation take a callback function pointer
as their final argument. Each function pointer has a type signature which is
documented in the `{api}.h` header file.

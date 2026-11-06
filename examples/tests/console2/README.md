Console2 Test App
=================

This app tests writing to a second serial console, using the same
`capsules/core/src/console.rs` syscall driver.

Second Console Driver Num
-------------------------

The second console driver num is created by ORing the number 1 shifted 24 bits
left with the normal console driver num.

```c
#define DRIVER_NUM_CONSOLE2 (DRIVER_NUM_CONSOLE | 0x01000000)
```

Expected Output
---------------

In the second serial port:

```
The is the second serial console. (0)
The is the second serial console. (1)
The is the second serial console. (2)
The is the second serial console. (3)
The is the second serial console. (4)
The is the second serial console. (5)
The is the second serial console. (6)
```

In the normal serial port:

```
[Console2] Writing to second serial console.
```

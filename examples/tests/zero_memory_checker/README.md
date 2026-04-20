Test `zero_memory_checker`
====================

See if we can write to the process's memory, reboot, and then read the same values back.
If not, then memory is being initialized.

Expected output
---------------

```
[ZMC] Look if memory is zeroed.
[ZMC] Able to allocate 2048 bytes
[ZMC] Checking index 0 (0x20009e8c): 00
[ZMC] Checking index 1024 (0x2000a28c): 00
[ZMC] Checking index 2047 (0x2000a68b): 00
[ZMC] Memory was zero! This is good. Try to write then reboot and check again.
[ZMC] If this loops continuously then things are working.
[ZMC] Writing to memory with arbitrary values
[ZMC] Rebooting
[ZMC] Look if memory is zeroed.
[ZMC] Able to allocate 2048 bytes
[ZMC] Checking index 0 (0x20009e8c): 00
[ZMC] Checking index 1024 (0x2000a28c): 00
[ZMC] Checking index 2047 (0x2000a68b): 00
[ZMC] Memory was zero! This is good. Try to write then reboot and check again.
[ZMC] If this loops continuously then things are working.
[ZMC] Writing to memory with arbitrary values
[ZMC] Rebooting
[ZMC] Look if memory is zeroed.
[ZMC] Able to allocate 2048 bytes
[ZMC] Checking index 0 (0x20009e8c): 00
[ZMC] Checking index 1024 (0x2000a28c): 00
[ZMC] Checking index 2047 (0x2000a68b): 00
[ZMC] Memory was zero! This is good. Try to write then reboot and check again.
[ZMC] If this loops continuously then things are working.
[ZMC] Writing to memory with arbitrary values
```

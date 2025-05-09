MPU No LibC
===========

This test verifies that an application can read and write all of the memory it
has been allocated in ram, and can read all of its memory in flash.

This test is raw assembly, and circumvents the usual libtock-c crt0 and any
other userspace startup. Of note, the application will never call `brk` or
`sbrk` or any other `memop` which would change the application memory layout
beyond the initial configuration provided by the kernel.

The application uses low-level debug to print its progress. A successful run
will see debug numbers [0,10], and then terminate the process with a successful
exit code.

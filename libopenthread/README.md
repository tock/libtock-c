## OpenThread Library for Tock: 

'libopenthread' is the OpenThread implementation for Tock. `libopenthread` takes the standard [openthread repository](https://github.com/openthread/openthread.git) and alters compiler and linker details to allow libtock-c to interact with the OpenThread library.

'libopenthread' is the compiled OpenThread static library files found in `libtock-c/libopenthread/build/cortex-m4/libopenthread.a`.

To compile the `libopenthread` library files, run the `create_libopenthread.sh` script. This script abstracts and invokes the OpenThread CMake build system. After completing the build, this script then combines the relevent static library files into a single static library so as to simplify the external library import process.

For now, `libopenthread` is only supported for the nrf52840dk board and configures OpenThread as a Full Thread Device (FTD). 

Please refer to the `libtock-c/examples/openthread_hello` for an example of how to include the static library. The `./create_ot_tock` script must be run prior to a `libtock-c` application using OpenThread.

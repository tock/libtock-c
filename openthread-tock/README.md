## OpenThread Library for Tock: 

'openthread-tock' is the OpenThread implementation for Tock. `openthread-tock` takes the standard [openthread repository](https://github.com/openthread/openthread.git) and alters compiler and linker details to allow libtock-c to interact with the OpenThread library.

'openthread-tock' is the compiled OpenThread static library files found in `libtock-c/openthread-tock/build/cortex-m4/openthread-tock.a`.

To compile the `openthread-tock` library files, run the `create_openthread-tock.sh` script. This script abstracts and invokes the OpenThread CMake build system. After completing the build, this script then combines the relevent static library files into a single static library so as to simplify the external library import process.

For now, `openthread-tock` is only supported for the nrf52840dk board and configures OpenThread as a Full Thread Device (FTD). 

Please refer to the `libtock-c/examples/openthread_hello` for an example of how to include the static library. The `./create_ot_tock` script must be run prior to a `libtock-c` application using OpenThread.

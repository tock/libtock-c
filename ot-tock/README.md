## OpenThread Library for Tock: 

'ot-tock' is the OpenThread implementation for Tock. `ot-tock` takes the standard [openthread repository](https://github.com/openthread/openthread.git) and alters compiler and linker details to allow libtock-c to interact with the OpenThread library.

'ot-tock' uses OpenThread static library files found in [TODO]

To compile the `ot-tock` library files, run the `create_ot-tock` script. This script abstracts and invokes the OpenThread CMake build system. After completing the build, this script then combines the relevent static library files into a single static library so as to simplify the external library import process.

For now, `ot-tock` is only supported for the nrf52840dk board. 

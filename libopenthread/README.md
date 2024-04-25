## LibOpenthread - OpenThread Library for Tock: 

`libopenthread` is the OpenThread implementation for Tock. `libopenthread` takes the standard [openthread repository](https://github.com/openthread/openthread.git) and utilizes the Tock build system to create a static library. Openthread requires a platform specific abstraction layer in addition to a number of third party dependencies. These static libraries must all be built to provide libopenthread. The static libraries produced include:
- libopenthread-{mtd/ftd}.a
- platform.a
- libmbedtls.a
- libtcplp.a

There are two methods to compile the `libopenthread` library files:
- Include the libopenthread static libraries in the app's Makefile (see `examples/openthread/hello/Makefile`). Upon running `make` in the app directory, libopenthread will be built.
- Run `make ftd` or `make mtd` in the `libtock-c/libopenthread`

Given the size of the OpenThread library, it may be beneficial to only build the library for your needed target as opposed to the default libtock-c option of building all architectures. To specify this, add `TOCK_TARGETS=cortex-m4` as an argument when invoking `make` or to the Makefile.

For now, `libopenthread` is only supported for the nrf52840dk board. 

NOTE: `make` will not detect changes in the libopenthread directory or subdirectories if invoked from within the app directory. As such, if you modify files in the libopenthread directory, be sure to invoke `make` within the libopenthread directory.

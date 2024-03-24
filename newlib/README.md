Tock Userland LibC
==================

[Newlib](https://sourceware.org/newlib/) is a port of the C standard library
designed for embedded systems. Tock uses a version of Newlib compiled to support
the position independent code that Tock applications require.

Compiling a new version of Newlib
---------------------------------

This folder contains scripts to build and package newlib for libtock-c. Note,
however, that successfully building usable newlib images is heavily dependent on
the specific toolchain (both ARM and RISC-V) that you have installed on your
machine. Specifically, we are aware of two issues that can arise:

1. For ARM, the specific architectures that your GCC was compiled with support
   for are the versions of newlib that will be compiled. As far as I know this
   is not configurable. If you run `arm-none-eabi-gcc -print-multi-lib` you will
   see the architectures.
2. For RISC-V, using a new toolchain (e.g. gcc 13) to compile the libraries, and
   then using those libraries with an older toolchain (e.g. gcc 10) seems to
   cause failures.

To try to ensure reproducibility, we include a Dockerfile that should be able to
build the same newlib that we distribute as part of the libtock-c build system.

### Simple Directions

If you want to build and package newlib locally you can use the provided
`Makefile`.

You must choose by setting the variable `NEWLIB_VERSION` with the version you
want to compile. The releases are listed
[here](http://sourceware.org/pub/newlib/).

Then:

    $ make NEWLIB_VERSION=4.3.0.20230120

When the build finishes (it takes a while), a zip folder named
`libtock-newlib-<version>.zip` will contain the built libraries. You can move
that folder to the `libtock-c/lib` directory to use the new version of newlib.

### Docker

To help with reproducibility, we provide Dockerfiles for various versions of
newlib that can generate the compiled version of libtock-newlib.

```bash
cd docker/docker-newlib-<version>
./docker-create.sh
```

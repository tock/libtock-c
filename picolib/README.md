Tock Userland libc  - Picolibc
==============================

[Picolibc](https://github.com/picolibc/picolibc) is a port of the C standard
library designed for embedded systems. Tock uses a version of Picolibc compiled
to support the position independent code that Tock applications require.

Compiling a new version of Picolibc
-----------------------------------

This folder contains scripts to build and package picolibc for libtock-c.

### Simple Directions

If you want to build and package picolibc locally you can use the provided
`Makefile`.

You must choose by setting the variable `PICOLIB_VERSION` with the version you
want to compile. The releases are listed
[here](https://github.com/picolibc/picolibc/releases).

Then:

    $ make PICOLIB_VERSION=1.8.5

When the build finishes (it takes a while), a zip folder named
`libtock-picolib-<version>.zip` will contain the built libraries. You can move
that folder to the `libtock-c/lib` directory to use the new version of newlib.

### Docker

To help with reproducibility, we provide Dockerfiles for various versions of
newlib that can generate the compiled version of libtock-picolib.

```bash
cd docker/docker-picolib-<version>
./docker-create.sh
```

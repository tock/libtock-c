C++ Libraries for libtock-c
===========================

Similarly to libc, we provide a pre-built version of the libc++ libraries so we
can compile with specific flags enabled.

Basic Instructions
------------------

You can create your own libc++ libraries by running `make` in this folder.

1. Install dependencies:

        sudo apt install libmpc-dev

2. Choose the version of GCC and newlib you want and run:

        make GCC_VERSION=13.2.0 NEWLIB_VERSION=4.3.0.20230120


Docker Instructions
-------------------

To help ensure reproducibility, we also include a Dockerfile which can be used
to create the libc++ libraries.

```bash
cd docker/docker-libc++-<version>
./docker-create.sh
```

libtock-libc++ Structure
------------------------

To create the precompiled library we build GCC for both ARM (arm-none-eabi) and
RISC-V (riscv64-unknown-elf) and package the resulting headers and libraries.
We preserve the folder structure from GCC.

```
libtock-libc++-$(GCC_VERSION)/arm
       /arm-none-eabi/include/c++/$(GCC_VERSION)                   # Headers
       /arm-none-eabi/lib/thumb/<sub arch>/nofp                    # libstdc++.a
       /lib/gcc/arm-none-eabi/$(GCC_VERSION)/thumb/<sub arch>/nofp # libgcc.a

libtock-libc++-$(GCC_VERSION)/riscv
       /riscv64-unknown-elf/include/c++/$(GCC_VERSION)              # Headers
       /riscv64-unknown-elf/lib/<sub arch>/ilp32                    # libstdc++.a
       /lib/gcc/riscv64-unknown-elf/$(GCC_VERSION)/<sub arch>/ilp32 # libgcc.a
```

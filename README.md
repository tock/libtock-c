[![Build Status](https://travis-ci.org/tock/libtock-c.svg?branch=master)](https://travis-ci.org/tock/libtock-c)
[![irc](https://img.shields.io/badge/irc-%23tock-lightgrey.svg)](https://kiwiirc.com/client/irc.freenode.net/tock)

Tock Userland
=============

This directory contains libraries and example applications for developing
Tock apps that sit above the kernel.


Prerequisites
-------------

1. If you have not yet done so, it might be a good idea to start with
   the [TockOS getting started
   guide](https://github.com/tock/tock/blob/master/doc/Getting_Started.md),
   which will lead you through the installation of some tools that
   will be useful for developing and deploying applications on
   TockOS. In particular, it will give you a rust environment
   (required to install `elf2tab`) and `tockloader`, which you need to
   deploy applications on most boards.

   And it will of course give you a board with TockOS installed which
   you can use to run the applications found in this repository.

   So, if you haven't been there before, just head over there until it
   sends you back here.

1. Clone this repository.

    ```
    $ git clone https://github.com/tock/libtock-c
    $ cd libtock-c
    ```

1. The main requirement to build the C applications in this repository is having
   cross compilers for embedded targets. You will need an `arm-none-eabi`
   toolchain for Cortex-M targets, and `riscv64-unknown-elf` toolchain for
   RISC-V targets.

   **MacOS**:
   ```
   $ brew tap ARMmbed/homebrew-formulae && brew update && brew install arm-none-eabi-gcc
   $ brew tap riscv/riscv && brew update && brew install riscv-gnu-toolchain --with-multilib
   ```

   You can also download a pre-built RISC-V toolchain
   [here](http://cs.virginia.edu/~bjc8c/archive/gcc-riscv64-unknown-elf-8.3.0-mac.zip).

   **Ubuntu (18.04LTS or later)**:
   ```
   $ sudo apt install gcc-arm-none-eabi
   ```

   You will also need a `riscv64` toolchain. You can download a pre-built
   toolchain
   [here](http://cs.virginia.edu/~bjc8c/archive/gcc-riscv64-unknown-elf-8.3.0-linux.zip),
   or follow [these
   instructions](https://github.com/riscv/riscv-gnu-toolchain#installation-newlib)
   (just the "Installation (Newlib)") and pass the `--enable-multilib` flag to
   `./configure` to build your own.

   **Arch**:
   ```
   $ sudo pacman -Syu gcc make arm-none-eabi-gcc riscv64-elf-gcc riscv32-elf-newlib
   ```

1. You will also need an up-to-date version of
   [elf2tab](https://crates.io/crates/elf2tab). The build system will install
   and update this automatically for you, but you'll need Rust's
   [cargo](https://doc.rust-lang.org/cargo/getting-started/installation.html)
   installed. If you have followed the getting started guide, everything should
   be in place.

1. You will also likely need [Tockloader](https://github.com/tock/tockloader), a
   tool for programming apps onto boards. If you haven't installed it
   during the TockOS getting started guide:

   MacOS:
   ```
   $ pip3 install tockloader
   ```

   Ubuntu:
   ```
   $ pip3 install tockloader --user
   ```

Compiling and Running Applications
----------------------------------

To compile all the examples, switch to the `examples` directory and
execute the build script:

    $ cd examples
    $ ./build_all.sh

This will install `elf2tab` if it is not yet installed and compile all the
examples for cortex-m0, cortex-m3, cortex-m4, and rv32imac. It does this because
the compiler emits slightly (or significantly) different instructions for each
variant. When installing the application, `tockloader` will select the correct
version for the architecture of the board being programmed.

The build process will ultimately create a `tab` file (a "Tock Application
Bundle") for each example application. The `tab` contains the
executable code for the supported architectures and can be
deployed to a board using `tockloader`. For example to one of the
Nordic development boards:

```
$ tockloader install --board nrf52dk --jlink blink/build/blink.tab
Installing apps on the board...
Using known arch and jtag-device for known board nrf52dk
Finished in 2.567 seconds
```

You can remove an application with

    $ tockloader uninstall --board nrf52dk --jlink blink

or remove all installed applications with

    $ tockloader uninstall --board nrf52dk --jlink

Tock applications are designed to be generic and run on any Tock-compatible
board. However, compiled applications typically depend on specific drivers,
which not all boards provide. For example, some applications expect an IEEE
802.15.4 radio interface which not all boards support. If you load an
application onto a board that does not support every driver/system call it
uses, some system calls will return error codes (`ENODEVICE` or `ENOSUPPORT`).

Next Steps
----------

The next step is to read the [overview](doc/overview.md) that
describes how applications in TockOS are structured and then look at
some of the examples in detail. The description of the [compilation
environment](doc/compilation.md) may also be of interest.

License
-------

Licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or
  http://www.apache.org/licenses/LICENSE-2.0)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or
  http://opensource.org/licenses/MIT)

at your option.

Contributions
-------------

We welcome contributions from all. We use the bors-ng bot to manage, approve,
and merge PRs. In short, when someone replies `bors r+`, your PR has been
approved and will be automatically merged. If a maintainer replies
`bors delegate+`, then you have been granted the authority to mark your own
PR for approval (usually this will happen if there are some trivial changes
required). For a full list of bors commands,
[see the bors documentation](https://bors.tech/documentation/).

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall
be dual licensed as above, without any additional terms or conditions.

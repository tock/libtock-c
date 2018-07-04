[![Build Status](https://travis-ci.org/tock/libtock-c.svg?branch=master)](https://travis-ci.org/tock/libtock-c)
[![irc](https://img.shields.io/badge/irc-%23tock-lightgrey.svg)](https://kiwiirc.com/client/irc.freenode.net/tock)

Tock Userland
=============

This directory contains libraries and example applications for developing
Tock apps that sit above the kernel.


Getting Started
---------------

- The main requirement is a cross compiler for embedded targets: `arm-none-eabi-`.

  MacOS:
  ```
  $ brew tap ARMmbed/homebrew-formulae && brew update && brew install arm-none-eabi-gcc
  ```

  Ubuntu:
  ```
  $ sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa && sudo apt update && sudo apt install gcc-arm-embedded
  ```

- You will also need an up-to-date version of [elf2tab](https://crates.io/crates/elf2tab).
The build system will install and update this automatically for you, but you'll need Rust's
[cargo](https://doc.rust-lang.org/cargo/getting-started/installation.html) installed.

- You will also likely need [Tockloader](https://github.com/tock/tockloader), a
tool for programming apps onto boards:

  MacOS:
  ```
  $ pip3 install tockloader
  ```

  Ubuntu:
  ```
  $ pip3 install tockloader --user
  ```


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

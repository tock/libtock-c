[![Build Status](https://travis-ci.org/tock/libtock-c.svg?branch=master)](https://travis-ci.org/tock/libtock-c)
[![irc](https://img.shields.io/badge/irc-%23tock-lightgrey.svg)](https://kiwiirc.com/client/irc.freenode.net/tock)

Tock Userland
=============

This directory contains libraries and example applications for developing
Tock apps that sit above the kernel.


Getting Started
---------------

The main requirement is a cross compiler for embedded targets: `arm-none-eabi-`.

MacOS:
```
$ brew tap ARMmbed/homebrew-formulae && brew update && brew install arm-none-eabi-gcc
```

Ubuntu:
```
$ sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa && sudo apt update && sudo apt install gcc-arm-embedded
```

You will also likely need [Tockloader](https://github.com/tock/tockloader), a
tool for programming apps onto boards:

MacOS:
```
$ pip3 install tockloader
```

Ubuntu:
```
$ pip3 install tockloader --user
```

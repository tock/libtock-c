Libtock Services
================

This directory holds higher-level services that build on top of the raw syscalls
defined in other folders in the `libtock` directory.

Unlike the other libtock interfaces, services defined in this folder can have
internal state, though any functionality that requires calls to `yield()`
still belongs in `libtock-sync`.

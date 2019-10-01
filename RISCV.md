RISCV
======

1. Download the toolchain from https://www.sifive.com/boards ("Prebuilt RISC‑V
GCC Toolchain")

2. Put the toolchain somewhere and update Configuration.mk so that it can find it.


OR

1. Using brew seems to work, but takes forever:

    ```
    brew install riscv-gnu-toolchain --with-multilib
    ```


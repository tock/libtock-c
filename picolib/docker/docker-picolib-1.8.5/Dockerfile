###
### Dockerfile to build libtock-picolib-1.8.5
###

# We get version 12 of arm-none-eabi-gcc/riscv64-unknown-elf-gcc.
FROM debian:bookworm

LABEL maintainer="Tock Project Developers <tock-dev@googlegroups.com>"
LABEL version="0.1"
LABEL description="Dockerfile to build libtock-c picolib 1.8.5."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive
# Update Ubuntu Software repository
RUN apt update

# Install our toolchains
RUN apt install -y gcc-arm-none-eabi gcc-riscv64-unknown-elf

# Install needed tools
RUN apt install -y git build-essential ninja-build meson zip

# Clone the libtock-c source so we can use the build scripts
RUN git clone https://github.com/tock/libtock-c -b make-precompiled-picolib
RUN cd libtock-c && git fetch && git checkout 6dc8907938e7daa307650ba6577a59c25214ef3f

# Actually build the toolchain
RUN cd libtock-c/picolib && make PICOLIB_VERSION=1.8.5

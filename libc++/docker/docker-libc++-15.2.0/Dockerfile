FROM debian:forky

LABEL maintainer="Tock Project Developers <tock-dev@googlegroups.com>"
LABEL version="0.1"
LABEL description="Dockerfile to build libtock-c libc++."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive
# Update Ubuntu Software repository
RUN apt update

# Install our toolchains
RUN apt install -y gcc-arm-none-eabi gcc-riscv64-unknown-elf

# Install needed tools
RUN apt install -y git build-essential wget rsync zip texinfo

# Install needed library
RUN apt install -y libmpc-dev file

# Clone the libtock-c source so we can use the build scripts
RUN git clone https://github.com/tock/libtock-c
RUN cd libtock-c && git fetch && git checkout 1b40e388916cffe5d655dddbe31083bbe4ffca3d

# Actually build the toolchain
RUN cd libtock-c/libc++ && make arm GCC_VERSION=15.2.0 NEWLIB_VERSION=4.6.0.20260123 -j16
RUN cd libtock-c/libc++ && make riscv-medlow GCC_VERSION=15.2.0 NEWLIB_VERSION=4.6.0.20260123 -j16
RUN cd libtock-c/libc++ && make riscv-medany GCC_VERSION=15.2.0 NEWLIB_VERSION=4.6.0.20260123 -j16
RUN cd libtock-c/libc++ && make GCC_VERSION=15.2.0 NEWLIB_VERSION=4.6.0.20260123 -j16

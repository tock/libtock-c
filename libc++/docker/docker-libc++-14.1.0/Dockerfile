FROM debian:trixie

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
RUN cd libtock-c && git fetch && git checkout 26f2f52841c726a2b38d51f3de93a64f753e1a36

# Actually build the toolchain
RUN cd libtock-c/libc++ && make GCC_VERSION=14.1.0 NEWLIB_VERSION=4.4.0.20231231 -j16

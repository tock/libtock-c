# Use debian so we get version 12 of the compilers.
FROM debian:bookworm

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
RUN git clone https://github.com/tock/libtock-c -b make-precompiled
RUN cd libtock-c && git fetch && git checkout 6dc8907938e7daa307650ba6577a59c25214ef3f

# Actually build the toolchain
RUN cd libtock-c/libc++ && make GCC_VERSION=13.2.0 NEWLIB_VERSION=4.3.0.20230120

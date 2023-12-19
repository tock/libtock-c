###
### Dockerfile to build libtock-newlib-4.2.0.20211231
###

# Use Ubuntu 22.04 so we get version 10 of arm-none-eabi-gcc/riscv64-unknown-elf-gcc.
FROM ubuntu:22.04

LABEL maintainer="Tock Project Developers <tock-dev@googlegroups.com>"
LABEL version="0.1"
LABEL description="Dockerfile to build libtock-c newlib 4.2.0.20211231."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive
# Update Ubuntu Software repository
RUN apt update

# Install our toolchains
RUN apt install -y gcc-arm-none-eabi gcc-riscv64-unknown-elf

# Install needed tools
RUN apt install -y git build-essential wget rsync zip

# Clone the libtock-c source so we can use the build scripts
RUN git clone https://github.com/tock/libtock-c -b make-precompiled
RUN cd libtock-c && git fetch && git checkout e8cb1bc98695b36b331ac977124c00ea160ceb76

# Actually build the toolchain
RUN cd libtock-c/newlib && make NEWLIB_VERSION=4.2.0.20211231

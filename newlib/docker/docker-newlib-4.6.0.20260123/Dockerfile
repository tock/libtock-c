###
### Dockerfile to build libtock-newlib-4.6.0.20260123
###

FROM debian:forky

LABEL maintainer="Tock Project Developers <tock-dev@googlegroups.com>"
LABEL version="0.1"
LABEL description="Dockerfile to build libtock-c newlib 4.6.0.20260123."

# Disable Prompt During Packages Installation
ARG DEBIAN_FRONTEND=noninteractive
# Update Ubuntu Software repository
RUN apt update

# Install our toolchains
RUN apt install -y gcc-arm-none-eabi gcc-riscv64-unknown-elf

# Install needed tools
RUN apt install -y git build-essential wget rsync texinfo zip

# Clone the libtock-c source so we can use the build scripts
RUN git clone https://github.com/tock/libtock-c
RUN cd libtock-c && git fetch && git checkout ec5030b2e20a13546568ab9a17d28633e180b198

# Actually build the toolchain
RUN cd libtock-c/newlib && make arm NEWLIB_VERSION=4.6.0.20260123
RUN cd libtock-c/newlib && make riscv-medlow NEWLIB_VERSION=4.6.0.20260123
RUN cd libtock-c/newlib && make riscv-medany NEWLIB_VERSION=4.6.0.20260123
RUN cd libtock-c/newlib && make NEWLIB_VERSION=4.6.0.20260123

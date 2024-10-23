###
### Dockerfile to build libtock-newlib-4.4.0.20231231
###

FROM debian:trixie

LABEL maintainer="Tock Project Developers <tock-dev@googlegroups.com>"
LABEL version="0.1"
LABEL description="Dockerfile to build libtock-c newlib 4.4.0.20231231."

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
RUN cd libtock-c && git fetch && git checkout 26f2f52841c726a2b38d51f3de93a64f753e1a36

# Actually build the toolchain
RUN cd libtock-c/newlib && make NEWLIB_VERSION=4.4.0.20231231

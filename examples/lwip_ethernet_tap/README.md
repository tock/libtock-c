# LwIP Tap Ethernet Example

This example demonstrates running a webserver using LwIP, using an Ethernet MAC
exposed through the [`ethernet_tap`][ethernet-tap-capsule] kernel capsule
driver.

## Running with the QEMU RISC-V 32 bit board

You can run this app with the QEMU RISC-V 32 bit `virt` board. This board has
support for QEMU's VirtIO network device, and can use a userspace network stack
to forward a local host-port to the LwIP network stack running in this
application.

First, build this application:

```
libtock-c/examples/lwip_ethernet_tap $ make
  CC        main.c
  LD        build/rv32imac/rv32imac.0x80100080.0x80210000.elf
 E2T        build/lwip_ethernet_tap.tab
Application size report for arch family rv32i:
   text    data     bss     dec     hex filename
  71256     104   48976  120336   1d610 build/rv32imac/rv32imac.0x80100080.0x80210000.elf
```

Then, in the kernel run the QEMU board with this application:
```
tock/boards/qemu_rv32_virt $ make run-app \
        APP=$LIBTOCK_C/examples/lwip_ethernet_tap/build/rv32imac/rv32imac.0x80100080.0x80210000.tbf \
        NETDEV=SLIRP NETDEV_SLIRP_ARGS='hostfwd=tcp::8081-192.168.1.50:80'
    Finished `release` profile [optimized + debuginfo] target(s) in 0.10s
   text    data     bss     dec     hex filename
  89088      48   41908  131044   1ffe4 $TOCK/target/riscv32imac-unknown-none-elf/release/qemu_rv32_virt

Running QEMU emulator version 9.1.3 (tested: 7.2.0) with
  - kernel $TOCK/kernel/target/riscv32imac-unknown-none-elf/release/qemu_rv32_virt.elf
  - app $LIBTOCK_C/examples/lwip_ethernet_tap/build/rv32imac/rv32imac.0x80100080.0x80210000.tbf
To exit type C-a x

qemu-system-riscv32 \
  -machine virt \
  -semihosting \
  -global driver=riscv-cpu,property=smepmp,value=true \
  -global virtio-mmio.force-legacy=false \
  -device virtio-rng-device \
  -netdev user,id=n0,net=192.168.1.0/24,dhcpstart=192.168.1.255,hostfwd=tcp::8081-192.168.1.50:80 \
  -device virtio-net-device,netdev=n0 \
  -nographic \
  -bios $TOCK/target/riscv32imac-unknown-none-elf/release/qemu_rv32_virt.elf \
  -device loader,file=$LIBTOCK_C/examples/lwip_ethernet_tap/build/rv32imac/rv32imac.0x80100080.0x80210000.tbf,addr=0x80100000
QEMU RISC-V 32-bit "virt" machine, initialization complete.
- Found VirtIO EntropySource device, enabling RngDriver
- Found VirtIO NetworkCard device, enabling EthernetTapDriver
Entering main loop.
-> userspace tap network app
-> tapif_init: success
-> tapif_status: 192.168.1.50
-> tap interface t0 added
tock$
```

Following this, you should be able to navigate to `http://127.0.0.1:8081` and
see LwIP's default web page.

[ethernet-tap-capsule]: https://github.com/tock/tock/blob/master/capsules/extra/src/ethernet_tap.rs
[qemu-rv32-board]: https://github.com/tock/tock/tree/master/boards/qemu_rv32_virt

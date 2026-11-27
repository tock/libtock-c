#!/bin/bash
# Run the stress test against a RISC-V kernel under QEMU.
#
#   ./qemu.sh <kernel.elf> [seconds]
#
# Prints the first failure it sees and exits 1, or exits 0 if the kernel
# survived the whole run. Run `make` first.
set -u

KERNEL=$1
SECS=${2:-60}
APP=$(dirname "$0")/build/rv32imac/rv32imac.0x80100080.0x80300000.tbf

if [ ! -f "$APP" ]; then echo "no app, run 'make' first ($APP)" >&2; exit 1; fi

# one-insn-per-tb: by default TCG only checks for interrupts at translation
# block boundaries, so an interrupt can only ever arrive at the head of a basic
# block. This masks races.
event=$(timeout --foreground -s KILL "$SECS" qemu-system-riscv32 \
  -machine virt -semihosting -accel tcg,one-insn-per-tb=on \
  -global driver=riscv-cpu,property=smepmp,value=true \
  -global virtio-mmio.force-legacy=false \
  -device virtio-rng-device -device virtio-keyboard-device \
  -bios "$KERNEL" -device loader,file="$APP",addr=0x80100000 \
  -nographic -monitor none -serial stdio </dev/null 2>&1 |
  awk '
    { print > "/dev/stderr" }
    # A second banner means the kernel faulted the app and restarted it.
    /switch_stress:/ && ++n > 1 { print "app restarted after a fault"; exit }
    /FAIL:|panicked|had a fault|Instruction access fault/ { print substr($0, 1, 120); exit }
  ')

if [ -n "$event" ]; then echo "FAIL: $event"; exit 1; fi
echo "ok: survived ${SECS}s"

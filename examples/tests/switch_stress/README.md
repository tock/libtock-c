# `switch_stress`

Stress test for kernel/app context switch.

This test has been built to catch two concrete issues:

- An incorrect implementation of the `svc` trap handler for ARMv7-M devices
  (Cortex-M3/4/7, etc.) where a tail-chained trap handler together with an
  interrupt that arrives during hardware-stacking of the userspace registers
  on an `svc` can skip over a system call handler.

  https://github.com/tock/tock/issues/3109
  https://github.com/tock/tock/pull/5193

- A bug in the RISC-V `switch_to_user` code where an interrupt in the critical
  section causes the kernel to believe the trap arrived in usermode, whereas
  it actually arrived in kernel mode. This causes a fault the next time the
  kernel switches to that app.

  https://github.com/tock/tock/pull/2308

This app attempts to fuzz those code paths by calling a driver that does not
exist in a loop, while an alarm keeps firing interrupts, which end up being
delivered as upcalls.

Each such system call jumps a random distance into a nop sled before trapping
and waits a random amount of time first, to reliably trigger bugs that require
interrupts and/or traps to arrive at a very specific moment in time.

For a correct kernel, the app will continue working, seeing `Failure` return
code for every system call and having its registers clobbered. For the two bugs
above, the system will either fault, or the app will detect that one or more
of its sentinel values in the registers haven't been clobbered, causing it
to raise an error.

The app works on RISC-V and Thumb-2 systems (not ARMv6-M / Cortex-M0).

## Running It

```sh
make
tockloader install build/switch_stress.tab
tockloader listen
```

Under QEMU, for RISC-V:

```sh
make
./qemu.sh /path/to/qemu_rv32_virt.elf 60
```

QEMU will return `0` on success, and `1` for failure.

## Output Format

```
switch_stress: 8388608 Hz alarm, 628736 ticks/1024 iters, sweeping dt 2..314368
calls=4096 alarms=608 echoes=0 bad=0 band=10 dt=2456
```

- `echoes` counts syscalls that returned their own arguments (non-zero = error)
- `bad` counts syscalls that returned something else unexpected.
- `band` and `dt` are the current alarm deadline; see below.

## Alarm Sweep

We want interrupts to arrive within the critical section(s) of the context
switch code. We cannot aim at one directly: the app arms the alarm, keeps
making dummy syscalls, and hopes the interrupt lands while the kernel is
midway through one of those later calls.

That puts the useful deadline on the scale of a single probe call (a dummy
syscall plus its random padding):

- too short, and it expires before the app even resumes, so the interrupt is
  already pending and gets taken at the same point every time
- too long, and the alarm fires too rarely to coincide with anything

A probe call is 0.3 alarm ticks on an nRF52840 but ~1000 under QEMU, so there
is no constant to use. The app times one probe call at startup, then sweeps
the deadline from 1/256x to 256x that, doubling at each step. Each step is a
band, reported as `band=` in the output. Within a band the wait is
re-randomized, and the app moves to the next band every 256 alarms.

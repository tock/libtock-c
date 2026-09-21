#include <stdint.h>
#include <stdio.h>

#include <libtock/peripherals/syscalls/alarm_syscalls.h>
#include <libtock/tock.h>

// Dummy driver number (must not exist), also serves as sentinel value.
#define BOGUS_DRIVER 0x5A5A5u

// Each probe jumps into a nop sled with `NOPS` instructions, to randomize the
// time at which a syscall is taken.
#define NOPS 16
#define STR_(x) #x
#define STR(x) STR_(x)

// PROBE builds one probe: load sentinel arguments, jump `a0` nops into the
// sled, then trap. It returns the syscall's first return word.
#if defined(__thumb__)
__asm__ (
  ".syntax unified\n"
  ".thumb\n"
  ".section .text.probes, \"ax\", %progbits\n"
  ".macro PROBE name, svcn\n"
  "  .balign 4\n"                      // keep `adr` 4-aligned: its result is
  "  .thumb_func\n"                    // Align(PC,4)-relative
  "  .global \\name\n"
  "\\name:\n"
  "  adr  r12, 8f\n"
  "  add  r12, r12, r0, lsl #1\n"      // 2 bytes per nop
  "  orr  r12, r12, #1\n"              // keep the Thumb bit
  "  movw r0, #0xA5A5\n"
  "  movt r0, #0x0005\n"
  "  movw r1, #0x1234\n"
  "  movw r2, #0x2222\n"
  "  movt r2, #0x1111\n"
  "  movw r3, #0x4444\n"
  "  movt r3, #0x3333\n"
  "  bx   r12\n"
  "8:\n"
  "  .rept " STR(NOPS) "\n"
  "  nop\n"
  "  .endr\n"
  "  svc  \\svcn\n"
  "  bx   lr\n"
  ".endm\n"
  );
#elif defined(__riscv) && __riscv_xlen == 32
__asm__ (
  ".section .text.probes, \"ax\", %progbits\n"
  ".macro PROBE name, svcn\n"
  "  .global \\name\n"
  "\\name:\n"
  "  lla  t0, 8f\n"
  "  slli t1, a0, 2\n"               // 4 bytes per nop, see norvc below
  "  add  t0, t0, t1\n"
  "  li   a0, 0x5A5A5\n"
  "  li   a1, 0x1234\n"
  "  li   a2, 0x11112222\n"
  "  li   a3, 0x33334444\n"
  "  li   a4, \\svcn\n"
  "  jr   t0\n"
  "8:\n"
  "  .option push\n"
  "  .option norvc\n"
  "  .rept " STR(NOPS) "\n"
  "  nop\n"
  "  .endr\n"
  "  ecall\n"
  "  .option pop\n"
  "  ret\n"
  ".endm\n"
  );
#else
#error "switch_stress needs Thumb-2 or RV32"
#endif

__asm__ (
  ".section .text.probes, \"ax\", %progbits\n"
  "PROBE probe_command, 2\n"
  "PROBE probe_allow_ro, 4\n"
  );

extern uint32_t probe_command(uint32_t nops);
extern uint32_t probe_allow_ro(uint32_t nops);

// How far ahead to set the alarm.
//
// We arm the alarm, keep making dummy syscalls, and hope the interrupt lands
// while the kernel is midway through one of those later calls.
//
// So the deadline has to be on the scale of a single probe call (a dummy
// syscall plus its random padding):
//
//   too short -> it expires before the app even resumes, so the interrupt is
//                already pending and gets taken at the same point every time
//   too long  -> the alarm fires too rarely to coincide with anything
//
// A probe call is 0.3 alarm ticks on an nRF52840 but ~1000 under QEMU, so
// there is no constant to use. calibrate() times one at startup and we sweep
// deadlines from 1/256x to 256x it, doubling each step ("bands").
#define BANDS        17    // deadlines from 1/256 to 256 probe calls
#define CENTER        8    // band CENTER is about one probe call
#define BAND_ALARMS 256    // advance the sweep after this many alarms...
#define BAND_ITERS 16384   // ...or this many probe calls, whichever is first

static uint32_t alarms;
static uint32_t rng   = 0x5eed;
static uint32_t dt_lo = 1;     // low edge of the current band, in ticks
static uint32_t k_iter;        // alarm ticks per 1024 probe calls

static uint32_t next_rand(void) {
  rng = rng * 1103515245u + 12345u;
  return rng;
}

// Low edge of band b, in alarm ticks.
static uint32_t band_edge(uint32_t b) {
  uint32_t shift = 10 + CENTER - b;                     // 10 undoes k_iter's x1024
  uint32_t t     = shift >= 32 ? 0 : k_iter >> shift;   // shifting >= 32 is UB
  return t ? t : 1;                                     // never arm for 0 ticks
}

static void rearm(void) {
  uint32_t actual;
  // Uniform in [dt_lo, 2*dt_lo), so arrivals spread across the whole band.
  libtock_alarm_command_set_relative(dt_lo + (next_rand() >> 11) % dt_lo, &actual);
}

static void alarm_cb(int now, int sched, int unused, void* ud) {
  (void)now;
  (void)sched;
  (void)unused;
  (void)ud;
  alarms++;
  rearm();
}

// One syscall to the bogus driver, with a random nop count and a random
// pre-trap delay. Bit 0 of `r` picks the syscall class.
static uint32_t run_probe(uint32_t r) {
  uint32_t nops = (r >> 8) % NOPS;
  for (volatile uint32_t d = (r >> 26) & 0x3f; d; d--) {}
  return (r & 1) ? probe_allow_ro(nops) : probe_command(nops);
}

// What a correct kernel returns: Failure for command, Failure u32 u32 for
// allow_readonly.
static uint32_t expected(uint32_t r) {
  return (r & 1) ? 2 : 0;
}

// How long one probe call takes, in alarm ticks x1024.
static uint32_t calibrate(void) {
  uint32_t r = 0x2308;

  // Time more and more calls, until enough ticks pass to divide accurately.
  for (uint32_t n = 256; ; n *= 2) {
    uint32_t t0 = 0, t1 = 0;
    libtock_alarm_command_read(&t0);
    for (uint32_t i = 0; i < n; i++) {
      r = r * 1103515245u + 12345u;
      run_probe(r);
    }
    libtock_alarm_command_read(&t1);

    uint32_t elapsed = t1 - t0;    // unsigned, so counter wrap is fine
    // 1024 ticks is accurate enough. Give up at 65536 calls, since a slow
    // alarm clock could need millions of them to tick that far.
    if (elapsed >= 1024 || n >= 65536) return (uint32_t)(((uint64_t)elapsed << 10) / n);
  }
}

int main(void) {
  uint32_t calls = 0, echoes = 0, bad = 0;
  uint32_t band = CENTER, band_alarms = 0, band_iters = 0;
  uint32_t r = 0x3109, to_yield = 4, freq = 0;

  libtock_alarm_command_get_frequency(&freq);
  k_iter = calibrate();
  dt_lo  = band_edge(band);
  printf("switch_stress: %lu Hz alarm, %lu ticks/1024 iters, sweeping dt %lu..%lu\n",
         (unsigned long)freq, (unsigned long)k_iter,
         (unsigned long)band_edge(0), (unsigned long)(2 * band_edge(BANDS - 1)));

  libtock_alarm_set_upcall(alarm_cb, NULL);
  rearm();

  for (uint32_t i = 1; ; i++) {
    if (alarms - band_alarms >= BAND_ALARMS || i - band_iters >= BAND_ITERS) {
      band        = (band + 1) % BANDS;
      dt_lo       = band_edge(band);
      band_alarms = alarms;
      band_iters  = i;
    }

    r = next_rand();
    uint32_t got = run_probe(r);
    calls++;

    if (got == BOGUS_DRIVER) {
      if (echoes++ == 0) {
        printf("FAIL: syscall returned its own arguments after %lu calls (band %lu, dt=%lu)\n",
               (unsigned long)calls, (unsigned long)band, (unsigned long)dt_lo);
      }
    } else if (got != expected(r)) {
      bad++;
    }

    // Let upcalls run, including the alarm's, which re-arms it.
    if (--to_yield == 0) {
      yield_no_wait();
      to_yield = 1 + ((r >> 4) & 7);
    }

    if ((i & 0xfff) == 0) {
      printf("calls=%lu alarms=%lu echoes=%lu bad=%lu band=%lu dt=%lu\n",
             (unsigned long)calls, (unsigned long)alarms, (unsigned long)echoes,
             (unsigned long)bad, (unsigned long)band, (unsigned long)dt_lo);
    }
  }
}

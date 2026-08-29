#include <led.h>
#include <timer.h>

#if defined(__thumb__)
static syscall_return_t packed(uint32_t count, uint32_t *data,
                         bool stop_error) {
  register uint32_t r0 __asm__ ("r0") = count;
  register uint32_t *r1 __asm__ ("r1") = data;
  register uint32_t r2 __asm__ ("r2") = stop_error;
  register uint32_t r3 __asm__ ("r3") = 0;
  register uint32_t rtype __asm__ ("r0");
  register uint32_t rv1 __asm__ ("r1");
  register uint32_t rv2 __asm__ ("r2");
  register uint32_t rv3 __asm__ ("r3");
  __asm__ volatile (
    "svc 0xfe"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}
#elif defined(__riscv)
static syscall_return_t packed(uint32_t count, uint32_t *data,
                         bool stop_error) {
  register uint32_t a0  __asm__ ("a0") = count;
  register uint32_t *a1  __asm__ ("a1") = data;
  register uint32_t a2  __asm__ ("a2") = stop_error;
  register uint32_t a3  __asm__ ("a3") = 0;
  register uint32_t a4  __asm__ ("a4") = 0xfe;
  register int rtype __asm__ ("a0");
  register int rv1 __asm__ ("a1");
  register int rv2 __asm__ ("a2");
  register int rv3 __asm__ ("a3");
  __asm__ volatile (
    "ecall\n"
    : "=r" (rtype), "=r" (rv1), "=r" (rv2), "=r" (rv3)
    : "r" (a0), "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  syscall_return_t rval = {rtype, {rv1, rv2, rv3}};
  return rval;
}
#endif

int main(void) {

    uint32_t data[25*5];

    bool light = true;

    while (true) {

    for (int i=0; i < 25; i++) {
      data[0 + 5*i] = 2;
      data[1 + 5*i] = 4;
      data[2 + 5*i] = (light?2:3);
      data[3 + 5*i] = i;
      data[4 + 5*i] = 0;
    }
    syscall_return_t res = packed(25, data, true);

    int executed_syscalls = 25;

    if (res.type == TOCK_SYSCALL_FAILURE_U32) {
      printf ("Executed %ld syscalls\n", 25 - res.data[0]); 
      executed_syscalls = 25 - res.data[0];
    }

    light = !light;

    for (int i = 0; i < executed_syscalls; i++) {
      printf ("syscall %d res %ld\n", i, data[1 + 5*i]);
    }

    // This delay uses an underlying timer in the kernel.
    delay_ms(250);
  }
}

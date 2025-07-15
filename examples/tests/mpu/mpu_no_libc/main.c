/* vim: set sw=2 expandtab tw=80: */

// Allow _start to go undeclared
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"

__attribute__ ((section(".start"), used))
__attribute__ ((naked))
__attribute__ ((noreturn))
void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
  __asm__ volatile (
    // This minimal test evaluates whether memory protection is configured
    // as-expected. It uses the low-level debug (LLD) mechanism to print
    // incremental numbers as it progresses, but provides no other output as it is
    // all assembly to precisely control behavior (and never sets up a stack
    // beyond the minimal one provided by the kernel).

    // First, save all the variables in hardware-stacked, callee-save registers
#if defined(__thumb__)
    "movs r4, r0\n"       // r4 = app_start
    "movs r5, r1\n"       // r5 = mem_start
    "movs r6, r2\n"       // r6 = memory_len
    "movs r7, r3\n"       // r7 = app_heap_break
    "mov  r8, sp\n"       // r8 = stack pointer
#elif defined(__riscv)
    "mv   s0, a0\n"       // s0 = app_start
    "mv   s1, a1\n"       // s1 = mem_start
    "mv   s2, a2\n"       // s2 = memory_len
    "mv   s3, a3\n"       // s3 = app_heap_break
    "mv   s4, sp\n"       // s4 = stack pointer
#else
#error Unknown arch. Note: Subsequent code assumes only two archs.
#endif

    // Note that the test has commenced
#if defined(__thumb__)
    "movs r2, #0\n"       // LLD Command 2, Arg 1: Number to print (0)
    "bl lld_print_r2\n"
#else
    "li   a2, 0\n"
    "jal ra, lld_print_a2\n"
#endif

    // Print all of the interesting starting values
    //  1. app_start: where app starts in flash
    //  2. mem_start: where app-accesible RAM starts
    //  3. memory_len: *total* length of app allocation (incl grant)
    //  4. app_heap_break: max app-accesible address
    //  5. the initial stack pointer when process was created
    //  6. the current stack pointer (should be same as 5)
#if defined(__thumb__)
    "movs r2, #1\n"
    "movs r3, r4\n"
    "bl lld_print_r2r3\n"
    "movs r2, #2\n"
    "movs r3, r5\n"
    "bl lld_print_r2r3\n"
    "movs r2, #3\n"
    "movs r3, r6\n"
    "bl lld_print_r2r3\n"
    "movs r2, #4\n"
    "movs r3, r7\n"
    "bl lld_print_r2r3\n"
    "movs r2, #5\n"
    "movs r3, r8\n"
    "bl lld_print_r2r3\n"
    "movs r2, #6\n"
    "mov  r3, sp\n"
#else
    "li   a2, 1\n"
    "mv   a3, s0\n"
    "jal ra, lld_print_a2a3\n"
    "li   a2, 2\n"
    "mv   a3, s1\n"
    "jal ra, lld_print_a2a3\n"
    "li   a2, 3\n"
    "mv   a3, s2\n"
    "jal ra, lld_print_a2a3\n"
    "li   a2, 4\n"
    "mv   a3, s3\n"
    "jal ra, lld_print_a2a3\n"
    "li   a2, 5\n"
    "mv   a3, s4\n"
    "jal ra, lld_print_a2a3\n"
    "li   a2, 6\n"
    "mv   a3, sp\n"
    "jal ra, lld_print_a2a3\n"
#endif

    // 7. Read all app-accessible memory: [mem_start , app_heap_break)
    // Assumed invariant: app_heap_break >= [mem_start + 4]
#if defined(__thumb__)
    "movs r2, #7\n"
    "bl lld_print_r2\n"

    "movs r3, r5\n"         // r3 (loop var) = mem_start
    "test_7_loop:\n"
    "ldr r2, [r3]\n"        // read *r3
    "add r3, r3, #4\n"      // increment r3=r3+4
    "cmp r3, r7\n"
    "ble test_7_loop\n"     // if r3 < app_heap_break[r7], loop
#else
    "li   a2, 7\n"
    "jal  ra, lld_print_a2\n"

    "mv   t1, s1\n"
    "test_7_loop:\n"
    "lw   t0, 0(t1)\n"
    "addi t1, t1, 4\n"
    "blt  t1, s3, test_7_loop\n"
#endif

    // 8. Write all app-accessible memory: [mem_start , app_heap_break)
    // Assumed invariant: app_heap_break >= [mem_start + 4]
#if defined(__thumb__)
    "movs r2, #8\n"
    "bl lld_print_r2\n"

    "movs r3, r5\n"         // r3 (loop var) = mem_start
    "test_8_loop:\n"
    "str r2, [r3]\n"        // read *r3
    "add r3, r3, #4\n"      // increment r3=r3+4
    "cmp r3, r7\n"
    "ble test_8_loop\n"     // if r3 < app_heap_break[r7], loop
#else
    "li   a2, 8\n"
    "jal  ra, lld_print_a2\n"

    "mv   t1, s1\n"
    "test_8_loop:\n"
    "sw   t0, 0(t1)\n"
    "addi t1, t1, 4\n"
    "blt  t1, s3, test_8_loop\n"
#endif

    // 9. Read all app-accessible flash: [app_start , memop(6))
    // Flash length is not a startup argument, so we must use
    // memop(6) to get the end of app flash first.
#if defined(__thumb__)
    "movs r2, #9\n"
    "bl lld_print_r2\n"

    "movs r0, #6\n"
    "movs r1, #0\n"
    "svc 5\n"
    "movs r11, r1\n"

    "movs r2, #9\n"
    "movs r3, r11\n"
    "bl lld_print_r2r3\n"

    "movs r3, r4\n"           // r3 (loop var) = app_start
    "test_9_loop:\n"
    "ldr r2, [r3]\n"          // read *r3
    "add r3, r3, #4\n"        // increment r3=r3+4
    "cmp r3, r11\n"
    "ble test_9_loop\n"       // if r3 < memop(6)[r11], loop
#else
    "li   a2, 9\n"
    "jal  ra, lld_print_a2\n"

    "li   a0, 6\n"
    "li   a1, 0\n"
    "li   a4, 5\n"
    "ecall\n"
    "mv   t2, a1\n"

    "li   a2, 9\n"
    "mv   a3, t2\n"
    "jal  ra, lld_print_a2a3\n"

    "mv   t1, s0\n"
    "test_9_loop:\n"
    "lw   t0, 0(t1)\n"
    "addi t1, t1, 4\n"
    "blt  t1, t2, test_9_loop\n"
#endif

    // 10. Return success
    // Indicate all tests are done and exit.
#if defined(__thumb__)
    "movs r2, #10\n"
    "bl lld_print_r2\n"

    "movs r0, #0\n"           // exit-terminate
    "movs r1, #0\n"           // completion: sucess
    "svc 6\n"                 // Exit syscall
#else
    "li   a2, 10\n"
    "jal  ra, lld_print_a2\n"

    "li   a0, 0\n"
    "li   a1, 0\n"
    "li   a4, 6\n"
    "ecall\n"
#endif

    // -1. Unreachable/error
#if defined(__thumb__)
    "loop_error:\n"
    "movs r2, #0\n"
    "sub r2, r2, #1\n"
    "bl lld_print_r2\n"
    "b loop_error\n"
#else
    "loop_error:\n"
    "li   a2, -1\n"
    "jal  ra, lld_print_a2\n"
    "j    loop_error\n"
#endif

    // Function to invoke LLD print of one number
    // r2/a2 must be set to desired number
    // may clobber any caller-save b/c of syscall rules
#if defined(__thumb__)
    "lld_print_r2:\n"
    "movs r0, #8\n"       // LLD is driver number 8
    "movs r1, #2\n"       // LLD Command 2: Print one number
    "svc 2\n"             // Invoke Command syscall
    "bx lr\n"             // Return.
#else
    "lld_print_a2:\n"
    "li   a0, 8\n"       // LLD is driver number 8
    "li   a1, 2\n"       // LLD Command 2: Print one number
    "li   a4, 2\n"       // Command is syscall 2
    "ecall\n"             // Invoke Command syscall
    "jr ra\n"             // Return.
#endif

    // Function to invoke LLD print of two numbers
    // r2,r3/a2,a3 must be set to desired number
    // may clobber any caller-save b/c of syscall rules
#if defined(__thumb__)
    "lld_print_r2r3:\n"
    "movs r0, #8\n"       // LLD is driver number 8
    "movs r1, #3\n"       // LLD Command 3: Print one number
    "svc 2\n"             // Invoke Command syscall
    "bx lr\n"             // Return.
#else
    "lld_print_a2a3:\n"
    "li   a0, 8\n"       // LLD is driver number 8
    "li   a1, 3\n"       // LLD Command 3: Print one number
    "li   a4, 2\n"       // Command is syscall 2
    "ecall\n"             // Invoke Command syscall
    "jr ra\n"             // Return.
#endif
    );
}

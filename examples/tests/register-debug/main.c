// Crash on load with known register values.

int main(void) {
#if defined(__thumb__)
  // ARM
  #if __ARM_ARCH >= 7
  __asm__ volatile (
    "ldr r0, =0xabc04320\n"
    "ldr r1, =0xabc04321\n"
    "ldr r2, =0xabc04322\n"
    "ldr r3, =0xabc04323\n"
    "ldr r4, =0xabc04324\n"
    "ldr r5, =0xabc04325\n"
    "ldr r6, =0xabc04326\n"
    "ldr r7, =0xabc04327\n"
    "ldr r8, =0xabc04328\n"
    "ldr r9, =0xabc04329\n"
    "ldr r10, =0xabc0432a\n"
    "ldr r11, =0xabc0432b\n"
    "ldr r12, =0xabc0432c\n"
    "str r1, [r0]\n"
    );
  #else
  __asm__ volatile (
    "ldr r0, =0xabc04320\n"
    "ldr r1, =0xabc04321\n"
    "ldr r2, =0xabc04322\n"
    "ldr r3, =0xabc04323\n"
    "ldr r4, =0xabc04324\n"
    "ldr r5, =0xabc04325\n"
    "ldr r6, =0xabc04326\n"
    "ldr r7, =0xabc04327\n"
    "str r1, [r0]\n"
    );
  #endif

#elif defined(__riscv)
  // RISC-V
  __asm__ volatile (
    "li  x1,  0xabc04321\n"
    "li  x2,  0xabc04322\n"
    "li  x3,  0xabc04323\n"
    "li  x4,  0xabc04324\n"
    "li  x5,  0xabc04325\n"
    "li  x6,  0xabc04326\n"
    "li  x7,  0xabc04327\n"
    "li  x8,  0xabc04328\n"
    "li  x9,  0xabc04329\n"
    "li  x10, 0xabc0432a\n"
    "li  x11, 0xabc0432b\n"
    "li  x12, 0xabc0432c\n"
    "li  x13, 0xabc0432d\n"
    "li  x14, 0xabc0432e\n"
    "li  x15, 0xabc0432f\n"
    "li  x16, 0xabc04330\n"
    "li  x17, 0xabc04331\n"
    "li  x18, 0xabc04332\n"
    "li  x19, 0xabc04333\n"
    "li  x20, 0xabc04334\n"
    "li  x21, 0xabc04335\n"
    "li  x22, 0xabc04336\n"
    "li  x23, 0xabc04337\n"
    "li  x24, 0xabc04338\n"
    "li  x25, 0xabc04339\n"
    "li  x26, 0xabc0433a\n"
    "li  x27, 0xabc0433b\n"
    "li  x28, 0xabc0433c\n"
    "li  x29, 0xabc0433d\n"
    "li  x30, 0xabc0433e\n"
    "li  x31, 0xabc0433f\n"
    "sw  x2, 0(x1)\n"
    );
#endif
}

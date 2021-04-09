// Do nothing. yield() immediately.

// Define the `_start_` signature.
void _start(void* app_start, void* mem_start, void* memory_len, void* app_heap_break);

void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
  // invalid syscall
  __asm__ volatile (
      "svc 0xbd       \n"
      :
      :
      : "memory", "r0", "r1", "r2", "r3", "r12", "lr"
      );
}

// Do nothing. yield() immediately.

// Define the `_start_` signature.
void _start(void* app_start, void* mem_start, void* memory_len, void* app_heap_break);

void _start(void* app_start __attribute__((unused)),
            void* mem_start __attribute__((unused)),
            void* memory_len __attribute__((unused)),
            void* app_heap_break __attribute__((unused))) {
  // Yield.
  __asm__ volatile (



      // try to brk the most amount of memory we can
      "add r6, r1, r2\n"   // r6 = memory_end

      "brk:\n"
      "movs r0, #0\n"  // memop::brk
      "movs r1, r6\n"  // brk address
      "svc 4\n"

      "cmp r0, #0 \n"
      "beq done     \n"

      // subtract 4, try again
      "sub r6, r6, #4 \n"
      "b brk          \n"


      "done: \n"
      // yield
      "svc 0       \n"

      :
      :
      : "memory", "r0", "r1", "r2", "r3", "r12", "lr"
      );
}

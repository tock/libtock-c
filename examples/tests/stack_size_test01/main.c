int main(void) {
  printf("Stack Test App\n");

  register uint32_t stack_pointer;
  asm volatile ("MOV %0, sp\n" : "=r" (stack_pointer));

  printf("Current stack pointer: 0x%lx\n", stack_pointer);

  return 0;
}

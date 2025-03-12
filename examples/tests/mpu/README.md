MPU Tests
=========

Tests that the MPU restricts process accesses correctly.

MPU test applications are necessarily somewhat fine-grained, as each MPU
violation results in termination of the test process.

As it can be annoying to flash myriad apps during development, the tests in the
top-level directory attempt to exercise all of the reads and writes of regions
that _should_ work during normal operation (and allow use of a button to inject
faults for interactive testing). The `unit/` folder holds all of the smaller
tests which attempt to violate access rules at each of the boundaries.

 ```text
      Perms?    Application Memory Layout     Relevant Memop(s)       Test

 ╔═   ╒RWX════════ ← mem_start + mem_len.....{3-mem_end} get          unit/mpu_ram_end
 ║    │...┆Grant Pointers
 ║    │...┆──────
 ║    │...┆Process Control Block
 ║ ╔═ │...┆──────
 ║    │...┆Grant Regions
 ║ D  │...┆
 ║ Y  │...┆  ↓
 ║ N  │...┆──────  ← kernel_memory_break.....{6-grant start} get      unit/mpu_grant_start
 ║ A  │...┆
 ║ M  │rw.┆──────  ← app_break...............{0-brk, 1-sbrk}          unit/mpu_heap_end
 ║ I  │rw.┆                                    get or move
 ║ C  │rw.┆  ↑
 ║    │rw.┆ Heap
 ║ ╚═ │rw.┆──────  ← app_heap_start..........{11-heap bot} set        (debug facilty; untested)
      │rw.┆ Data
 F    │rw.┆──────  ← data_start_pointer......{10-stack top} set       (debug facilty; untested)
 I    │rw.┆Stack
 X    │rw.┆  ↓
 E    │rw.┆
 D    │rw.┆──────  ← current_stack_pointer
      │rw.┆
 ╚═══ ╘═══════════ ← memory_start............{2-mem_start} get        unit/mpu_stack_growth (needs refinement)

 ╔═   ╒RWX ═══════ ← app end.................{5-flash end} get        unit/mpu_flash_end
 ║    │r.x┆
      │r.x┆ app
 F    │r.x┆
 I  ╔ │r.x┆──────
 X  T │r..┆
 E  B │rw.┆ ┍──
 D  F  rw.┆ ┋   Writeable flash
      │rw.┆ ┍── regions in the               {9-region[N] end} get    (todo)
 ║  H │rw.┆ ┝── TBF header                   {8-region[N] start} get  (todo)
 ║  D │rw.┆ ┕──
 ║  R │r..┆                                  {7-#flash regions} get   (todo)
 ║    │r..┆
 ╚═ ╚ ╘═══════════ ← app start...............{4-flash start} get      unit/mpu_flash_start
```

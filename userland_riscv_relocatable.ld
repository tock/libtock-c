/* Userland Layout for Relocatable 32-bit RISC-V Apps
 *
 * A linker script where the code is linked to run from the app's RAM. crt0
 * copies the code there at startup to make this work.
 */

/* Memory Spaces Definitions, 448K flash, 128K ram */
PROG_LENGTH = 0x00070000;
RAM_LENGTH  = 0x00020000;

EXTERN(_start)
ENTRY(_start_flash)

/* Note: The application loader will select the actual location in flash where
 * the app is placed.
 */
MEMORY {
    FLASH (rx) : ORIGIN = 0x80000000, LENGTH = PROG_LENGTH
    SRAM (RWX) : ORIGIN = 0x00000000, LENGTH = RAM_LENGTH
}

SECTIONS {
    /* Section for just the app crt0 header.
     * This must be first so that the app can find it.
     */
    .crt0_header :
    {
        /* elf2tab checks this symbol to see if the app is linked for a fixed
         * flash address. */
        _flash_origin = .;
        /**
         * Populate the header expected by `crt0`:
         *
         *  struct hdr {
         *    uint32_t got_sym_start;
         *    uint32_t got_start;
         *    uint32_t got_size;
         *    uint32_t data_sym_start;
         *    uint32_t data_start;
         *    uint32_t data_size;
         *    uint32_t bss_start;
         *    uint32_t bss_size;
         *    uint32_t reldata_start;
         *    uint32_t stack_size;
         *    uint32_t text_sym_start;
         *    uint32_t text_start;
         *    uint32_t text_size;
         *  };
         */
        /* Offset of GOT symbols in flash from the start of the application
         * binary. */
        LONG(LOADADDR(.got) - ORIGIN(FLASH));
        /* Offset of where the GOT section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_got - ORIGIN(SRAM));
        /* Size of GOT section. */
        LONG(SIZEOF(.got));
        /* Offset of data symbols in flash from the start of the application
         * binary. */
        LONG(LOADADDR(.data) - ORIGIN(FLASH));
        /* Offset of where the data section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_data - ORIGIN(SRAM));
        /* Size of data section. */
        LONG(SIZEOF(.data));
        /* Offset of where the BSS section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_bss - ORIGIN(SRAM));
        /* Size of BSS section */
        LONG(SIZEOF(.bss));
        /* First address offset after program flash, where elf2tab places
         * .rela.data section */
        LONG(LOADADDR(.endflash) - ORIGIN(FLASH));
        /* The size of the stack requested by this application */
        LONG(STACK_SIZE);
        /* Offset of the code in flash from the start of the application
         * binary. */
        LONG(LOADADDR(.text) - ORIGIN(FLASH));
        /* Offset of where the code will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_text - ORIGIN(SRAM));
        /* Size of the code. */
        LONG(SIZEOF(.text));
    } > FLASH =0xFF

    /* App state section. Used for persistent app data.
     * We put this first so that if the app code changes but the persistent
     * data doesn't, the app_state can be preserved.
     */
    .wfr.app_state :
    {
        KEEP (*(.app_state))
        . = ALIGN(4); /* Make sure we're word-aligned here */
    } > FLASH =0xFF

    /* Need to reserve room for the stack in the linker file. This makes the
     * _got addresses used by the compiler match what they will be when the
     * app is loaded into memory.
     */
    .stack :
    {
        /* elf2tab requires that the `_SRAM_ORIGIN` symbol be present to
		 * mark the first address in the SRAM memory. Since ELF files do
		 * not really need to specify this address as they only care about
		 * loading into flash, we need to manually mark this address for
		 * elf2tab. elf2tab will use it to add a fixed address header in the
		 * TBF header if needed.
		 */
	    _sram_origin = .;

        /* Be conservative about our alignment for the stack. Different
         * architectures require different values (8 for ARM, 16 for RISC-V),
         * so we choose the largest value. In practice, this likely will not
         * matter since the start of SRAM is unlikely to be at a very peculiar
         * address.
         */
        . = ALIGN(16);
        _stack = .;
        . = _stack + STACK_SIZE;
        . = ALIGN(16);
    } > SRAM

    /* Text section, Code! It is stored in flash, and crt0 copies it to RAM,
     * right after the stack.
     */
    .text :
    {
        . = ALIGN(4);
        _text = .;
        KEEP (*(.start))
        *(.text*)
        *(.rodata*)
        *(.srodata*) /* for RISC-V */
        KEEP (*(.syscalls))
        _etext = .;
        *(.gcc_except_table*)
        . = ALIGN(4); /* Make sure we're word-aligned here */
    } > SRAM AT > FLASH

    /* Global Offset Table */
    .got :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _got = .;
        *(.got*)
        *(.got.plt*)
        . = ALIGN(4);
    } > SRAM AT > FLASH

    /* Data section, static initialized variables
     *  Note: This is placed in Flash after the text section, but needs to be
     *  moved to SRAM at runtime
     */
    .data :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _data = .;
        KEEP(*(.data*))
        /* Include the "small data" in the data section. Otherwise it will be
         * dropped when the TBF is created.
         */
        KEEP(*(.sdata*))
        . = ALIGN(4); /* Make sure we're word-aligned at the end of flash */
    } > SRAM AT > FLASH

    /* BSS section, static uninitialized variables */
    .bss :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _bss = .;
        KEEP(*(.bss*))
        KEEP(*(.sbss*)) /* for RISC-V */
        *(COMMON)
        . = ALIGN(4);
    } > SRAM

    /* End of flash. */
    .endflash :
    {
    } > FLASH
}

/*
 * Defines location of start in flash (distance is same as distance from start in RAM).
 */
_start_flash = LOADADDR(.text) + (_start - ADDR(.text));

ASSERT(_got <= _bss, "
The GOT section must be before the BSS section for crt0 setup to be correct.");
ASSERT(_data <= _bss, "
The data section must be before the BSS section for crt0 setup to be correct.");

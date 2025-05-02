OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

_sstack = 0x00010000;    /* start of RAM */
_estack = 0x10010000;    /* end of RAM */

MEMORY
{
    ram : ORIGIN = 0x10000, LENGTH = 0x10000000
}
SECTIONS
{
    /DISCARD/ : { *(.comment .note) }
    .ram_vectors (0x10000 + 0x00000000) : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
    __ram_exc_heap_memsize = 0;
}

OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(reset_vector)
MEMORY
{
    ram(xrw) : ORIGIN = 0x80000, LENGTH = 1024M
}

_Min_Heap_Size = 0xa00000;   /* required amount of heap  */

SECTIONS
{
    . = 0x00080000;
    .ram_vectors : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    }  > ram
    __ram_vectors_lma = LOADADDR(.ram_vectors);
    __ram_exc_heap_memsize = 0;

    .got ALIGN(0x4) : { *(.got.plt) *(.got) }  > ram

    .gcc_except_table ALIGN (0x8) : { . = .; } > ram
    .gcc_except_table : { KEEP(*(.gcc_except_table*)) } > ram
    .exception_ranges ALIGN (0x8) : ONLY_IF_RW { *(.exception_ranges .exception_ranges*) }  > ram

    .ARM.extab ALIGN(0x4) : { *(.ARM.extab* .gnu.linkonce.armextab.*) }  > ram

    /* Exception handling  */
    PROVIDE (__EH_FRAME_BEGIN__ = LOADADDR(.eh_frame));
    .eh_frame ALIGN (0x8) : { KEEP (*(.eh_frame)) *(.eh_frame.*);  }  > ram

    .fast_text ALIGN(0x4) : { __fast_text_start = ABSOLUTE(.); . = .;
    } > ram

    __fast_text_end = ABSOLUTE(.);

    .fast_data ALIGN(0x4) : { __fast_data_start = ABSOLUTE(.); . = .;
    } > ram

    __fast_data_end = ABSOLUTE(.);

    __fast_end = .;

    /* text/read-only data */
    .text ALIGN(0x1000) : {
        __text_start = .;
        *(.text* *.text.* .sram.text.glue_7* .gnu.linkonce.t.*)
        __text_end = .;
    }  > ram

    .rodata ALIGN(0x1000) : {
        __rodata_start = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        __rodata_end = .;
    } > ram

    .data ALIGN(0x1000) : {
        /* writable data  */
        __ram_data_start = .;
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(0x4);
        KEEP(*( SORT (.liteos.table.*)));
    } > ram

    .ctors : ALIGN(0x4) {
        __ctor_list__ = .;
        KEEP (*(.ctors .init_array))
        __ctor_end__ = .;
    } > ram
    .dtors : ALIGN(0x4) {
        __dtor_list__ = .;
        KEEP (*(.dtors .fini_array))
        __dtor_end__ = .;
        __ram_data_end = __dtor_end__;
    } > ram

    /* unintialized data (in same segment as writable data) */
    .bss ALIGN(0x800) : {
        __int_stack_start = .;
        *(.int_stack);
        . = ALIGN(0x4);
        KEEP(*(.bss.prebss.*))
        . = ALIGN(0x8);
        __bss_start = .;
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(0x8);
        __bss_end = .;
    } > ram

    .heap : {
        . = ALIGN(0x40);
        __exc_heap_start = .;
        __exc_heap_end = . + __ram_exc_heap_memsize;
        . = __exc_heap_end;
        . = ALIGN(0x40);
        __heap_start = .;
    } > ram

    . += _Min_Heap_Size;
    __LOS_HEAP_ADDR_END__ = .;
    /* Strip unnecessary stuff */
    /DISCARD/ 0 : { *(.comment .note) }
}

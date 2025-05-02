OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(system_vectors)

SECTIONS
{
    . = 0x00008000;
    .ram_vectors : {
        __ram_vectors_vma = .;
        KEEP (*(.vectors))
    } 
    __ram_vectors_lma = LOADADDR(.ram_vectors);
    __ram_exc_heap_memsize = 0;

    .got ALIGN(0x4) : { *(.got.plt) *(.got) } 

    .gcc_except_table ALIGN (0x8) : { . = .; }   .gcc_except_table : { KEEP(*(.gcc_except_table*)) }
    .exception_ranges ALIGN (0x8) : ONLY_IF_RW { *(.exception_ranges .exception_ranges*) } 

    .ARM.extab ALIGN(0x4) : { *(.ARM.extab* .gnu.linkonce.armextab.*) } 

    /* Exception handling  */
    PROVIDE (__EH_FRAME_BEGIN__ = LOADADDR(.eh_frame));
    .eh_frame ALIGN (0x8) : { KEEP (*(.eh_frame)) *(.eh_frame.*);  } 

    .fast_text ALIGN(0x4) : { __fast_text_start = ABSOLUTE(.); . = .;}

    __fast_text_end = ABSOLUTE(.);

    .fast_data ALIGN(0x4) : { __fast_data_start = ABSOLUTE(.); . = .;}

    __fast_data_end = ABSOLUTE(.);

    __fast_end = .;

    /* text/read-only data */
    .text ALIGN(0x1000) : {
        __text_start = .;
        *(.text* *.text.* .sram.text.glue_7* .gnu.linkonce.t.*)
        __text_end = .;
    } 

    .rodata ALIGN(0x1000) : {
        __rodata_start = .;
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        __rodata_end = .;
    }

    .data ALIGN(0x1000) : {
        /* writable data  */
        __ram_data_start = .;
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(0x4);
        KEEP(*( SORT (.liteos.table.*)));
    }

    .ctors : ALIGN(0x4) {
        __ctor_list__ = .;
        KEEP (*(.ctors .init_array))
        __ctor_end__ = .;
    }
    .dtors : ALIGN(0x4) {
        __dtor_list__ = .;
        KEEP (*(.dtors .fini_array))
        __dtor_end__ = .;
        __ram_data_end = __dtor_end__;
    }

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
    }

    /* Strip unnecessary stuff */
    /DISCARD/ 0 : { *(.comment .note) }
}


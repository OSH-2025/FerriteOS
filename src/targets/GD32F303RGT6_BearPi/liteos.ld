ENTRY(Reset_Handler)

_estack = 0x20018000;
__los_stack_size__ = 0x400;

MEMORY
{
FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
.isr_vector :
{
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
} >FLASH


.text :
{
    . = ALIGN(4);
    __text_start = .;
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    __text_end = .;
} >FLASH

.rodata :
{
    . = ALIGN(4);
    __rodata_start = .;
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
    __rodata_end = .;
} >FLASH

_si_liteos_vector_data = LOADADDR(.vector_ram);

.vector_ram :
{
    . = ORIGIN(RAM);
    _s_liteos_vector = .;
    *(.data.vector)
    _e_liteos_vector = .;
} > RAM AT> FLASH

_sidata = LOADADDR(.data);

.data ALIGN(0x1000):
{
    __ram_data_start = .;
    . = ALIGN(4);
    *(.data)
    *(.data*)
    KEEP(*( SORT (.liteos.table.*)));
    . = ALIGN(4);
    __ram_data_end = .;
} >RAM AT> FLASH

. = ALIGN(4);
.bss :
{
    __bss_start = .;
    *(.bss .bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
} >RAM

. = ALIGN(8);
__los_heap_addr_start__ = .;
__los_heap_addr_end__ = ORIGIN(RAM) + LENGTH(RAM) - __los_stack_size__ - 1;

__fast_end = .;

/DISCARD/ :
{
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
}

.ARM.attributes 0 : { *(.ARM.attributes) }
}



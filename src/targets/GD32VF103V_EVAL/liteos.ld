OUTPUT_ARCH( "riscv" )

ENTRY( _start )

/* Highest address of the user mode stack */
_estack = 0x20008000;

/* Specify the memory areas */
MEMORY
{
FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{

  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  } >FLASH

  .ilalign         :
  {
    . = ALIGN(4);
    PROVIDE( _ilm_lma = . );
  } >FLASH

  .ialign         :
  {
    PROVIDE( _ilm = . );
  } >FLASH

  .text           :
  {
    . = ALIGN(4);
    __text_start = .;
    *(.rodata .rodata.*)  
    *(.text.unlikely .text.unlikely.*)
    *(.text.startup .text.startup.*)
    KEEP(*(.itcm.text))
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.gnu.linkonce.t.*)
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
    __text_end = _etext;
  } >FLASH
  
  .fini :
  {
    KEEP (*(SORT_NONE(.fini)))
  } >FLASH

  . = ALIGN(4);

  PROVIDE (__etext = .);
  PROVIDE (_etext = .);/*0x80022c8*/
  PROVIDE (etext = .);/*0x80022c8*/
  PROVIDE( _eilm = . );
  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    __rodata_start = .;

    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
    __rodata_end = .;
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array     :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array     :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  } >FLASH

  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  } >FLASH

    . = ALIGN(4);
    PROVIDE( _eilm = . );

  .lalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data_lma = . );
  } >FLASH

  .dalign         :
  {
    . = ALIGN(4);
    PROVIDE( _data = . );
  } >RAM

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data ALIGN(0x1000):
  {
    __ram_data_start = _sdata;
    //. = ALIGN(4);
    . = ORIGIN(RAM);
    _sdata = .;        /* create a global symbol at data start */
    *(.data.vector) 
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.rdata) 
    *(.gnu.linkonce.r.*)
    *(.gnu.linkonce.d.*)
    . = ALIGN(8);
    PROVIDE( __global_pointer$ = . + 0x800); 
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
    . = ALIGN(8);
    KEEP(*( SORT (.liteos.table.*)));
    *(.srodata.cst16)
    *(.srodata.cst8)
    *(.srodata.cst4)
    *(.srodata.cst2)
    *(.srodata .srodata.*)
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
    __ram_data_end = _edata;
  } >RAM AT> FLASH

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    __bss_start = _sbss;
    *(.bss)
    *(.sbss*)
    *(.gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
    __bss_end = _ebss;
  } >RAM

  . = ALIGN(8);
  __los_heap_addr_start__ = .;
  __los_heap_addr_end__ = ORIGIN(RAM) + LENGTH(RAM) - 0x400 - 1;
  __irq_stack_top = .;
  __nmi_stack_top = .;
  __exc_stack_top = .;
  PROVIDE( _sp = ORIGIN(RAM) + LENGTH(RAM) ); 
__fast_end = .;
  /* Remove information from the standard libraries 0x400 */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

}


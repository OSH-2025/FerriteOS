MEMORY
{
  iram (RX) : org = 0x40100000, len = 0xC000
  irom (RX) : org = 0x40200010 + (0x10000 & (0x100000 - 1)),
                                     len = 0xf0000 - 0x10
  dram (RW) : org = 0x3FFE8000, len = 0x18000
}
ENTRY(main);
SECTIONS
{
  .iram0.vectors :
  {
    _init_start = ABSOLUTE(.);
    KEEP(*(.SystemInfoVector.text));
    . = 0x10;
    KEEP(*(.DebugExceptionHandler.text));
    . = 0x20;
    KEEP(*(.NMIExceptionHandler.text));
    . = 0x30;
    KEEP(*(.KernelExceptionHandler.text));
    . = 0x50;
    KEEP(*(.UserExceptionHandler.text));
    . = 0x70;
    KEEP(*(.DoubleExceptionHandler.text));
    . = ALIGN (16);

  } > iram

  .iram0.text :
  {
    _iram_text_start = ABSOLUTE(.);

    *( .iram .iram* .iram1 .iram1.* .literal .text)

    _iram_text_end = ABSOLUTE(.);
  } > iram

  .dram0.data :
  {
    _data_start = ABSOLUTE(.);
    __ram_data_start = ABSOLUTE(.);
    *(.data1)
    *(.sdata)
    *(.sdata.*)
    KEEP (*(SORT(.liteos.table.*)));
    *(.dram0 .dram0.*)

    *( .data  .data.* )
    __ram_data_end = ABSOLUTE(.);
    _data_end = ABSOLUTE(.);
    . = ALIGN(4);
  } > dram

  /* Shared RAM */
  .dram0.bss (NOLOAD) :
  {
    . = ALIGN (8);
    __bss_start = ABSOLUTE(.);

    *(EXCLUDE_FILE(*libcore.a) .bss EXCLUDE_FILE(*libcore.a) .bss.* EXCLUDE_FILE(*libcore.a )COMMON)
    *(.sbss)
    *(.sbss.*)
    . = ALIGN (8);
    __bss_end =  ABSOLUTE(.);
    . += 0x2000;
    __heap_start = ABSOLUTE(.);
  } > dram

  .flash.text :
  {
    
    _stext = .;
    _text_start = ABSOLUTE(.);
    __text_start = ABSOLUTE(.);
    *( .literal.*  .text  .text.*)
    __text_end = ABSOLUTE(.);
    _text_end = ABSOLUTE(.);
    _etext = .;
  } >irom

  .flash.rodata ALIGN(4) :
  {
    __rodata_start = ABSOLUTE(.);
    . = 0x8;
    *(.rodata_desc .rodata_desc.*)
    *(.rodata .rodata.*)
    *(.gnu.version_r)
 
    __init_array_start = ABSOLUTE(.);
    KEEP (.ctors*)
    __init_array_end = ABSOLUTE(.);
    __rodata_end = ABSOLUTE(.);

  } >irom

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
}

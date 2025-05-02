.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global  g_pfnVectors
.global  Default_Handler

.word  __los_heap_addr_start__
.word  __los_heap_addr_end__

.word _si_liteos_vector_data
.word _s_liteos_vector
.word _e_liteos_vector

.word  _sidata
.word  __ram_data_start
.word  __ram_data_end
.word  __bss_start
.word  __bss_end

    .section  .text
    .global __LOS_HEAP_ADDR_START__
    .global __LOS_HEAP_ADDR_END__
__LOS_HEAP_ADDR_START__:
    .word  __los_heap_addr_start__
__LOS_HEAP_ADDR_END__:
    .word  __los_heap_addr_end__

    .section  .text.Reset_Handler
    .weak  Reset_Handler
    .type  Reset_Handler, %function
Reset_Handler:
    cpsid i
    ldr   sp, =_estack

    movs  r1, #0
    b  LoopCopyVectorInit

CopyVectorInit:
    ldr   r3, =_si_liteos_vector_data
    ldr   r3, [r3, r1]
    str   r3, [r0, r1]
    adds   r1, r1, #4

LoopCopyVectorInit:
    ldr   r0, =_s_liteos_vector
    ldr   r3, =_e_liteos_vector
    adds   r2, r0, r1
    cmp   r2, r3
    bcc   CopyVectorInit

    movs  r1, #0
    b  LoopCopyDataInit

CopyDataInit:
    ldr  r3, =_sidata
    ldr  r3, [r3, r1]
    str  r3, [r0, r1]
    adds  r1, r1, #4

LoopCopyDataInit:
    ldr  r0, =__ram_data_start
    ldr  r3, =__ram_data_end
    adds  r2, r0, r1
    cmp  r2, r3
    bcc  CopyDataInit
    ldr  r2, =__bss_start
    b  LoopFillZerobss

FillZerobss:
    movs  r3, #0
    str  r3, [r2], #4

LoopFillZerobss:
    ldr  r3, = __bss_end
    cmp  r2, r3
    bcc  FillZerobss

    bl  main
    bx  lr
.size  Reset_Handler, .-Reset_Handler

    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
    b  Infinite_Loop
    .size  Default_Handler, .-Default_Handler

    .section  .isr_vector,"a",%progbits
    .type  g_pfnVectors, %object
    .size  g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word  _estack
    .word  Reset_Handler
    .word  NMI_Handler
    .word  HardFault_Handler
    .word  MemManage_Handler
    .word  BusFault_Handler
    .word  UsageFault_Handler
    .word  0
    .word  0
    .word  0
    .word  0
    .word  SVC_Handler
    .word  DebugMon_Handler
    .word  0
    .word  PendSV_Handler
    .word  SysTick_Handler

    .weak      NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak      HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak      MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler

    .weak      BusFault_Handler
    .thumb_set BusFault_Handler,Default_Handler

    .weak      UsageFault_Handler
    .thumb_set UsageFault_Handler,Default_Handler

    .weak      SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak      DebugMon_Handler
    .thumb_set DebugMon_Handler,Default_Handler

    .weak      PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak      SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler

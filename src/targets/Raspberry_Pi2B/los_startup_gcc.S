#include "arch/exception.h"
#include "arch/regs.h"

/* stack */
.global __exc_stack_top
.global __irq_stack_top
.global __fiq_stack_top
.global __svc_stack_top
.global __abt_stack_top
.global __undef_stack_top
.global __exc_stack
.global __irq_stack
.global __fiq_stack
.global __svc_stack
.global __abt_stack
.global __undef_stack

/* param0 is stack top, param1 is stack size, param2 is magic num */
.macro STACK_MAGIC_SET param0, param1, param2
    ldr     r0, =\param0
    mov     r1, \param1
    ldr     r2, =\param2
    bl      excstack_magic
.endm

.code   32
.section ".vectors","ax"
.globl system_vectors
system_vectors:

    b   _reset
    b   _osExceptUndefInstrHdl
    b   _osExceptSwiHdl
    b   _osExceptPrefetchAbortHdl
    b   _osExceptDataAbortHdl
    b   _osExceptAddrAbortHdl
    b   OsIrqHandler
    b   _osExceptFiqHdl

.text
/* reset entry */
.globl _reset
_reset:
    mrs r0, cpsr_all
    eor r0, r0, #CPSR_HYP_MODE
    tst r0, #CPSR_MODE_MASK
    bic r0, r0, #CPSR_MODE_MASK
    orr r0, r0, #CPSR_SVC_MODE | CPSR_INT_DISABLE
    bne 1f
    ldr r1, =common
    msr ELR_hyp, r1
    msr SPSR_hyp, r0
    eret
1:  msr cpsr_c, r0

common:
    mrc p15, 0, r0, c1, c0, 1 
    bic r0, r0, #0x40
    mcr p15, 0, r0, c1, c0, 1

    mcr     p15, #0, r0, c8, c7, #0    @ invalidate tlb
    mrc     p15, #0, r0, c1, c0, #0
    bic     r0, r0, #1
    mcr     p15, #0, r0, c1, c0, #0    @ clear mmu bit
    dsb

    /* enable fpu+neon */
    LDR     r0, =(0xF << 20)
    MCR     p15, 0, r0, c1, c0, 2

    MOV     r3, #0x40000000
    VMSR    FPEXC, r3

    /* set the cpu to SVC32 mode and disable interrupt */
    mrs     r0, cpsr
    bic     r0, r0, #0x1f
    orr     r0, r0, #0x13
    msr     cpsr_c, r0

    ldr     r0, =__svc_stack_top
    mov     sp, r0

    STACK_MAGIC_SET __undef_stack, #OS_EXC_UNDEF_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __abt_stack, #OS_EXC_ABT_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __irq_stack, #OS_EXC_IRQ_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __fiq_stack, #OS_EXC_FIQ_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __svc_stack, #OS_EXC_SVC_STACK_SIZE, OS_STACK_MAGIC_WORD
    STACK_MAGIC_SET __exc_stack, #OS_EXC_STACK_SIZE, OS_STACK_MAGIC_WORD

    /* clear .bss */
    mov     r0,#0                   /* get a zero                       */
    ldr     r1,=__bss_start         /* bss start                        */
    ldr     r2,=__bss_end           /* bss end                          */

bss_loop:
    cmp     r1,r2                   /* check if data to clear           */
    strlo   r0,[r1],#4              /* clear 4 bytes                    */
    blo     bss_loop                /* loop until done                  */

    b main

.text
/*
 * set magic num to stack top for all cpu
 * r0 is stack top, r1 is stack size, r2 is magic num
 */
excstack_magic:
    mov     r3, #0
excstack_magic_loop:
    str     r2, [r0]
    add     r0, r0, r1
    add     r3, r3, #1
    cmp     r3, #CORE_NUM
    blt     excstack_magic_loop
    bx      lr

.section ".int_stack", "wa", %nobits
.align  3

__undef_stack:
    .space OS_EXC_UNDEF_STACK_SIZE 
__undef_stack_top:

__abt_stack:
    .space OS_EXC_ABT_STACK_SIZE
__abt_stack_top:

__fiq_stack:
    .space OS_EXC_FIQ_STACK_SIZE
__fiq_stack_top:

__irq_stack:
    .space OS_EXC_IRQ_STACK_SIZE
__irq_stack_top:

__svc_stack:
    .space OS_EXC_SVC_STACK_SIZE
__svc_stack_top:

__exc_stack:
    .space OS_EXC_STACK_SIZE
__exc_stack_top:


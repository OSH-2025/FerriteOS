#include "arch/regs.h"

    .section    .DispatchHandler.text, "ax"
    .align    4
    .global    _OsExceptionDispatch
    .extern    g_excHandlerTable

    .begin no-schedule

_OsExceptionDispatch:
    movi    a4, g_excHandlerTable                                        // Address of exc handler table
    movi    a2, OsExcDefaultHandler                             // Address of default handler

    extui   a3, a3, SPREG_EXCCAUSE_CAUSE_SHIFT, SPREG_EXCCAUSE_CAUSE_BITS    // Extract exc cause

    addx4   a4, a3, a4
    l32i    a4, a4, 0                    // Load handler address

    moveqz  a4, a2, a4                 // If no handler then use default
    mov     a2, a1                        // Set handler parameter

    movi    a3, SPREG_PS_STACK_MASK
    movi    a5, SPREG_PS_STACK_CROSS
    xps     a5, a3

    movi    sp, __int_stack_end

    jx      a4                        // Return directly from handler

    .end    no-schedule
    .size    _OsExceptionDispatch, . - _OsExceptionDispatch


/***************************************************************************************
* the default exception handler entry. At here, the a2 is the pointer that pointer to the
* exception context save area. The rest of exception context has already save into
* stack frame, now save the a0-a7 registers and call the exception-trace
****************************************************************************************/
    .extern OsExcHandlerEntry
    .section .os.kernel.text, "ax"
    .align  4
    .global  OsExcDefaultHandler
OsExcDefaultHandler:
    // The ps.entrynr unset operation is necessary, or the pc can not go out of this code segment
    movi   a3, SPREG_PS_ENTRYNR_MASK
    movi   a4, 0
    xps    a4, a3

    // The lock interrupt operation was deleted, it is too late to disable interrupts at here.
    // When the dispatch entry is executed, the interrupt will be restored.
    // Disable interrupts should be executed before dispatch entry.

    ssai    0                             // trigger all window overflow to achieve the exception backtrace.
    spillw                                // a0-a7 saved below stack pointer

    addi   a2,   a2,  -STKFR_SA_SIZE      // The s32i can not visit the negative address, so move down the pointer
    addi   a1,   a1,  -STKFR_SA_SIZE

    l32i   a15,  a1,  STKFR_SA_OFF_A0     // save the a0-a7 to exception context save area
    s32i   a15,  a2,  STKFR_SA_OFF_A0
    l32i   a15,  a1,  STKFR_SA_OFF_A1
    s32i   a15,  a2,  STKFR_SA_OFF_A1
    l32i   a15,  a1,  STKFR_SA_OFF_A2
    s32i   a15,  a2,  STKFR_SA_OFF_A2
    l32i   a15,  a1,  STKFR_SA_OFF_A3
    s32i   a15,  a2,  STKFR_SA_OFF_A3
    l32i   a15,  a1,  STKFR_SA_OFF_A4
    s32i   a15,  a2,  STKFR_SA_OFF_A4
    l32i   a15,  a1,  STKFR_SA_OFF_A5
    s32i   a15,  a2,  STKFR_SA_OFF_A5
    l32i   a15,  a1,  STKFR_SA_OFF_A6
    s32i   a15,  a2,  STKFR_SA_OFF_A6
    l32i   a15,  a1,  STKFR_SA_OFF_A7
    s32i   a15,  a2,  STKFR_SA_OFF_A7
                                          // All exception context saved, prepare to call the exception_trace

    addi   a1,   a1,  STKFR_SA_SIZE       // restore the stack pointer
    mov    a10,  a2                       // pass the address of context to exception_trace

    movi   a4,  OsExcHandlerEntry
    callx8 a4

    retw                                  // the call does not return to here

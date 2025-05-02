#include <xtensa/config/core.h>
#include <xtensa/coreasm.h>
#include <xtensa/corebits.h>
#include <xtensa/core-macros.h>

#include "arch/regs.h"

#define OS_TASK_STATUS_RUNNING    0x0010

     .extern  g_newTask
     .extern  g_runTask
     .extern  g_taskSwitchHook

/*
 * The Interrupt Schedule handler, can only be execute
 * after the TailChain
 */
    .macro   OS_INT_SCHEDULE

    rsr.ms  a13 /* Temporarily Saved ms value into a13 */
    movi    a14, 0
    wsr.ms  a14 /* Set ms'value to 0 */
    rsync

    addi    a1,  a1, -(CONTEXT_SIZE + 4) /* Reserves stack space for contexts and a13 */
    s32i    a13, a1, 0 /* Because the c function is called, a13 may change, so save to the stack. */
    /* The value of ms needs to be set to 0 when the c function is called in the assembly.
       Otherwise, the sliding window does not work properly. */
    movi    a14, OsSchedProcSchedFlag
    callx8  a14
    l32i    a13, a1, 0 /* Restore the ms value */
    addi    a1,  a1, (CONTEXT_SIZE + 4)

    wsr.ms  a13
    rsync

    .endm

/*
 * The entry point vectors for windowed configurations.
 */
    .extern    _DoubleExceptionHandler
    .extern    _ResetHandler
    .extern    _OsExceptionDispatch

    .global    _DispatchVector
    .global    _DoubleExceptionVector
    .weak      _DoubleExceptionVector

    .section .DispatchVector.text, "ax"
    .align    64
_DispatchVector:
    .org    0
    j       JumpToResetHandler

    .org    3
    ill

    .org    6
_DoubleExceptionVector:
    j       JumpToResetHandler

    .org    9
    ill

/*
 * Start of dispatch code.
*/
    .org        12
    .global    _Dispatch_tailchain
    .global    _Dispatch_entry
    .global    _Dispatch_restore_epc
    .global    _Dispatch_restore_epc_live

_Dispatch_tailchain:
    /* Tailchain */
    s32si.x8   a10, a1         /* Select interrupt, a10 <- intnum * 8 */
    l32dis.it  a8,  a10        /* a8 <- handler_table[intnum].addr */
    l32dis.it4 a10, a10        /* a10 <- handler_table[intnum].arg */
    s32stk     a9,  a1, 96     /* Set new stack pointer */
    s32dis.h   a8,  a8         /* Jump to handler if interrupt else fall through
                                  Note this also clears local exclusive monitor */

/* Fallthrough: exit dispatch */
_Exit:
    j       _Exit_1            /* Start exit process */

    .org    36                 /* Fixed offset for Underflow segment */
    .global _Underflow

_Underflow:
    l32e     a8,  a1, -64      /* a8  <- [a1-32] */
    l32e     a9,  a1, -64      /* a9  <- [a1-28] */
    l32e    a10,  a1, -60      /* a10 <- [a1-24] */
    l32e    a11,  a1, -48      /* a11 <- [a1-20] */
    l32e    a12,  a1, -44      /* a12 <- [a1-16] */
    l32e    a13,  a1, -40      /* a13 <- [a1-12] */
    l32e    a14,  a1, -36      /* a14 <- [a1-08] */
    l32e    a15,  a1, -64      /* a15 <- [a1-04], branch to EPC */

    .org    60                 /* Fixed offset for Save/Overflow segment */
    .global _Save

_Save:
    s32e     a8,  a1, -52      /* [a1-32] <- a8,  (a8  <- ExcVaddr) */
    s32e     a9,  a1, -28      /* [a1-28] <- a9,  (a9  <-   PS/SAR) */
    s32e    a10,  a1, -48      /* [a1-24] <- a10, (a10 <-      EPC) */
    s32e    a11,  a1, -24      /* [a1-20] <- a11, (a11 <- ExcCause) */
    s32e    a12,  a1, -44      /* [a1-16] <- a12, (a12 <-     LBEG) */
    s32e    a13,  a1, -40      /* [a1-12] <- a13, (a13 <-     LEND) */
    s32e    a14,  a1, -36      /* [a1-08] <- a14, (a14 <-   LCOUNT) */
    s32e    a15,  a1, -32      /* [a1-04] <- a15, (a15 <-       a1) */

_Dispatch_entry:
    s32e    a12,  a1, -20      /* [a1-84] <- a12 (LBEG) */
    /* From here until out of entry dispatch, double exception handling
       may clobber a12. */
    s32e    a13,  a1, -24      /* [a1-88] <- a13 (LEND) */
    s32e    a14,  a1, -28      /* [a1-92] <- a14 (LCOUNT) */

    /* if need disable interrupt on exc, disable at here
       if extra save jobs exist, do it at here, corresponding restore jobs
       can only be done after _Exit1 label */
    extui   a14, a11, SPREG_EXCCAUSE_CAUSE_SHIFT, SPREG_EXCCAUSE_CAUSE_BITS
    _bnez   a14, _Disable_Int_On_Exc

_Dispatch_Select_Int_Or_Exc:
    s32si.x8      a10, a1      /* [a1-80] <- a10 (EPC), a10 <- intnum * 8 */
    l32dis.it     a8,  a10     /* a8 <- handler_table[intnum].addr */
    l32dis.it4    a10, a10     /* a10 <- handler_table[intnum].arg */
    s32stk        a9,  a1, 96  /* [a1-76] <- a9 (PS/SAR), a9 = a1 - 96 */
    s32dis.h      a8,  a8      /* If interrupt: a8 <- VecBase + 12, branch to handler, enable intrs
                                  If exception: a8 <- VecBase + 15, fall through, enable intrs
                                  If neither: branch to Tailchain
                                  If higher priority interrupt has appeared, branch to Tailchain */

    /* Fallthrough:  process exception
       At this point, we have:
       - DispSt = Normal
       - a3 holds ExcCause (.Cause != 0)
       - window rotated, a1 points to new stack frame
       EXCCAUSE and EXCVADDR may not have been saved into stack frame.
       Save them if needed. EXCVADDR needs to be read again. */
    rsr.excvaddr    a2
    s32i     a2,  a1, 28        /* [old_a1-68] <- a2 (ExcVaddr) */
    s32i     a3,  a1, 24        /* [old_a1-72] <- a3 (ExcCause) */

    j       _Exception          /* Jump around literals */

    .section .DispatchHandler.text, "ax"
    .align    4
    .literal_position

_Exception:
    j       _OsExceptionDispatch /* Exc handler returns to a0 (_Dispatch_tailchain+3) */

_Exit_1:
    /* if extra restore jobs exist, do it at here */
    OS_INT_SCHEDULE

    .global     _Restore11
_Restore11:
    l32e        a10,  a1, -12 /* a10 <- [a1-76] (PS/SAR) */
    l32e        a12,  a1, -20 /* a12 <- [a1-84] (LBEG) */
    l32e        a13,  a1, -24 /* a13 <- [a1-88] (LEND) */
    l32e        a14,  a1, -28 /* a14 <- [a1-92] (LCOUNT) */
    l32dis.epc  a11,  a1      /* a11 <- [a1-80] (EPC) If interrupt pending
                                 go to _Dispatch_tailchain else fall through */

    .global    _Restore
/*
 * NOTE: the following restrictions must be observed:
 * 1) The LCOUNT register must be restored after LBEG/LEND.
 * 2) There must be at least 3 instructions between the LCOUNT
 *    restore and the last L32E (the one that branches).
 */
_Restore:
    l32e    a12,  a1, -44  /* a12 <- [a1-16], (LBEG   <- a12) */
    l32e    a13,  a1, -40  /* a13 <- [a1-12], (LEND   <- a13) */
    l32e    a14,  a1, -36  /* a14 <- [a1-08], (LCOUNT <- a14) */
    l32e     a8,  a1, -64  /* a8  <- [a1-32] */
    l32e     a9,  a1, -64  /* a9  <- [a1-28] */
    l32e    a10,  a1, -60  /* a10 <- [a1-24], (PS/SAR <- a10) */

_Dispatch_restore_epc:
    l32e    a11,  a1, -48  /* a11 <- [a1-20], (EPC    <- a11) */

_Dispatch_restore_epc_live:
    l32e    a15,  a1, -64  /* a15 <- [a1-04], branch to EPC If interrupt, branch to Tailchain, don't update a15 */

_Disable_Int_On_Exc:
    movi    a14, SPREG_PS_DI
    xps     a14, a14
    j       _Dispatch_Select_Int_Or_Exc


    .align    4
JumpToResetHandler:
    j.l    _ResetHandler, a0

    .global _exit

_exit:
    j    JumpToResetHandler
    /* Symbols for the debugger to use in identifying interrupt / exception frames. */
    .global _Interrupt
    .set    _Interrupt, _Dispatch_tailchain - 1
    .size   _Interrupt, 1

    .global  _GeneralException
    .set     _GeneralException, _Dispatch_tailchain
    .size    _GeneralException, _Exit - _Dispatch_tailchain

    /* Size of interrupt frame */
    .global _InterruptFrameSize
    .set    _InterruptFrameSize, 0x60

    /* Register save offset in interrupt frame */
    .global _InterruptRegisterSaveOffset
    .set    _InterruptRegisterSaveOffset, 0x20

    /* EPC offset in interrupt frame */
    .global _InterruptEPCSaveOffset
    .set    _InterruptEPCSaveOffset, 0x10

    /* Size of exc frame */
    .global _GeneralExceptionFrameSize
    .set    _GeneralExceptionFrameSize, 0x60

    /* Register save offset in exc frame */
    .global _GeneralExceptionRegisterSaveOffset
    .set    _GeneralExceptionRegisterSaveOffset, 0x20

    /* EPC offset in exc frame */
    .global _GeneralExceptionEPCSaveOffset
    .set    _GeneralExceptionEPCSaveOffset, 0x10

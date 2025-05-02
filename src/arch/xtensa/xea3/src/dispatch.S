#include "arch/regs.h"

.global g_newTask
.global g_runTask
.global g_taskScheduled

.section .sram.text, "ax"
.align  4
.type OsStartToRun, @function
.global OsStartToRun
OsStartToRun:
    entry    a1,  48

    rsr      a15, sar /* trigger all overflow to save AR0-AR31 to system stack */
    ssai     0         /* avoid the context 's OS init and task confused */
    spillw
    wsr      a15, sar

    movi     a15, SPREG_PS_DI /* disable interrupt */
    xps      a15, a15

    movi     a10, g_newTask
    s32i     a2,  a10, 0 /* a2 : input parameter */
    l32i     a7,  a10, 0
    movi     a6,  g_runTask
    s32i     a7,  a6,  0

    movi     a4,  g_taskScheduled
    movi     a5,  1
    s32i     a5,  a4,  0

    movi     a4,  OsSwitchToNewTask
    jx       a4

.section .os.kernel.text, "ax"
.align  4
.type OsTaskSchedule, @function
.global OsTaskSchedule
OsTaskSchedule:
    entry    a1, 32

    ssai 0
    spillw

    movi     a3, SPREG_PS_DI_DEPC
    xps      a3, a3 /* disable interrupts, the ps.stack field not changed == first kernel */

    addi     a1, a1, -CONTEXT_SIZE
    s32i     a0, a1, CONTEXT_OFF_PC
    s32i     a3, a1, CONTEXT_OFF_PS

    movi     a4, OS_SCHED_FLAG_TASKPREEMT
    s32i     a4, a1, CONTEXT_OFF_SCHED_FLAG

    movi     a2, g_runTask
    l32i     a6, a2, 0  /* running task */
    movi     a2, g_newTask
    l32i     a7, a2, 0  /* highest task */

    s32i     a1, a6, 0 /* save current stack pointer */

OsSwitchToNewTask:
    movi     a4, SPREG_PS_STACK_CROSS
    movi     a5, SPREG_PS_STACK_MASK
    xps      a4, a5 /* set the ps.stack to cross stack, the isl will work */
    movi     sp, __int_stack_end

    movi     a2, g_runTask
    s32i     a7, a2, 0 /* g_runTask = g_newTask */
    l32i     a6, a2, 0

OsSwitchModeCheck:
    movi     a4, SPREG_PS_STACK_KERNEL
    movi     a5, SPREG_PS_STACK_MASK
    xps      a4, a5

    l32i     a3, a6, 12
    wsr.ksl  a3
    rsync

    l32i     a1, a6, 0

    l32i     a4, a1, CONTEXT_OFF_SCHED_FLAG /* get the context load flag from stack */

    beqi     a4, OS_SCHED_FLAG_TASKPREEMT, osFastContextLoad
    j        osThrdContextLoad

/*
 *  Load the new task's context, jump to the task entry
 *  At here, the new task's state is set to running state, and
 *  the g_runTask = g_newTask, a1 = g_newTask->stackPointer
 */
.global osThrdContextLoad
osThrdContextLoad:
    l32i     a4,  a1, CONTEXT_OFF_LCOUNT
    l32i     a5,  a1, CONTEXT_OFF_LEND
    l32i     a6,  a1, CONTEXT_OFF_LBEG
    l32i     a10, a1, CONTEXT_OFF_EXCCAUSE
    l32i     a11, a1, CONTEXT_OFF_EXCVADDR

    wsr.lcount    a4
    wsr.lend      a5
    wsr.lbeg      a6
    wsr.exccause  a10
    wsr.excvaddr  a11
    rsync

    l32i     a3,  a1, CONTEXT_OFF_PC  /* load the task entry */
    l32i     a4,  a1, CONTEXT_OFF_PS
    l32i     a8,  a1, CONTEXT_OFF_A8  /* load the task entry's return address */

    l32i     a9,  a1, CONTEXT_OFF_A9  /* load the task stack's stack bottom */
    l32i     a10, a1, CONTEXT_OFF_A10 /* load the task entry function's argument */
    mov      a1,  a9
    wsr.ps   a4
    rsync

    jx       a3

/*
 *  Load the task scheduled by calling the OsTaskSchedule. A few of the
 *  context need to be loaded. At here, the new task's state is set to
 *  running state, and the g_runTask = g_newTask,
 *  a1 = g_newTask->stackPointer
 */
    .global osFastContextLoad
osFastContextLoad:
    l32i    a0, a1, CONTEXT_OFF_PC
    l32i    a3, a1, CONTEXT_OFF_PS
    addi    a1, a1, CONTEXT_SIZE
    wsr.ps  a3
    retw

    .section .os.kernel.text, "ax"
    .align 4
    .global  OsGetCurrStkPt
    .type    OsGetCurrStkPt, @function
OsGetCurrStkPt:
    entry    a1,  STKFR_SA_SIZE
    addi     a2,  a1, STKFR_SA_SIZE
    retw

    /* need push stack firstly */
    .section .os.kernel.text, "ax"
    .align 4
    .global  OsGetCurrCaller
    .type    OsGetCurrCaller, @function
OsGetCurrCaller:
    entry    a1,  32

    rsr      a3,  sar
    ssai     0
    spillw
    wsr      a3,  sar

    addi     a2,   a1,  -STKFR_SA_SIZE
    l32i     a2,   a2,  0

    retw

    .section .os.kernel.text, "ax"
    .align 4
    .global  ArchIntLock
    .type    ArchIntLock, @function
ArchIntLock:
    entry    a1, 32

    movi     a2, SPREG_PS_DI
    xps      a2, a2

    retw

    .section .os.kernel.text, "ax"
    .align 4
    .global  ArchIntUnlock
    .type    ArchIntUnlock, @function
ArchIntUnlock:
    entry   a1, 32

    movi    a3, SPREG_PS_DI_MASK
    movi    a2, 0
    xps     a2, a3

    retw

    .section .os.kernel.text, "ax"
    .align 4
    .global  ArchIntRestore
    .type    ArchIntRestore, @function
ArchIntRestore:
    entry   a1, 32

    movi    a3, SPREG_PS_DI_MASK
    xps     a2, a3

    retw

    .section .os.kernel.text, "ax"
    .align 4
    .global  wfi
    .type    wfi, @function
wfi:
    entry    a1, 32
    waiti 0

    retw


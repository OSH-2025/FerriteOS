/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: LiteOS task schedule module implemention.
 * Author: Huawei LiteOS Team
 * Create: 2021-03-21
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------- */

#include <xtensa/coreasm.h>
#include "arch/regs.h"
#include "arch/task.h"

.macro OsSaveA4ToA7
    s32i    a4, a1, REG_OFF_AR4
    s32i    a5, a1, REG_OFF_AR5
    s32i    a6, a1, REG_OFF_AR6
    s32i    a7, a1, REG_OFF_AR7
.endm

/*
  firstly, read EPCn(n=2..6) or MEPC, and then write EPCn or MEPC into stack,
  secondly, read EPSn(n=2..6) or MEPS, and then write EPSn or MEPS into stack
*/
.macro OsSaveEpcnEpsn as, epcn, epsn
    rsr     \as, \epcn
    s32i    \as, a1, REG_OFF_PC
    rsr     \as, \epsn
    s32i    \as, a1, REG_OFF_PS
.endm


/*
 * Importing a symbol g_cpOwner that can be referenced by other objs. The benefit is that
 * make sure this obj must be linked. Otherwise, some interrupt vectors can't be linked to elf.
 */
    .section        .os.data, "a"
    .align    4
    .global g_cpOwner

g_cpOwner:
    .word   0


    .begin  literal_prefix  .UserExceptionVector
    .section        .UserExceptionVector.text, "ax"
    .align 4
    .global _UserExceptionVector
_UserExceptionVector:
    /* level1 interrupt or exception vector, user mode defined by PS.UM */
    addi    a1, a1, -REG_CONTEXT_SIZE
    s32i    a2, a1, REG_OFF_AR14
    wsr     a2, excsave1
    movi    a2, UserExcIntVecExt
    jx      a2

    .text
    .end    literal_prefix


    .begin  literal_prefix  .KernelExceptionVector
    .section        .KernelExceptionVector.text, "ax"
    .align 4
    .global _KernelExceptionVector
_KernelExceptionVector:
    /* level1 interrupt or exception vector, kernel mode defined by PS.UM */
    addi    a1, a1, -REG_CONTEXT_SIZE
    s32i    a2, a1, REG_OFF_AR14       /* save a2 temporary */
    wsr     a2, excsave1
    movi    a2, KernelExcIntVecExt
    jx      a2

    .text
    .end    literal_prefix


    .begin  literal_prefix  .Level2InterruptVector.text
    .section        .Level2InterruptVector.text, "ax"
    .align 4
    .global _Level2Vector
_Level2Vector:
    /* level2 interrupt vector */
    addi    a1, a1, -REG_CONTEXT_SIZE  /* allocate exception stack frame, etc. */
	wsr     a2, excsave2               /* save a2 to excsave2 temporarily */
    movi    a2, Level2VectorExt
    jx      a2

    .text
    .end    literal_prefix


    .begin  literal_prefix  .Level3InterruptVector.text
    .section        .Level3InterruptVector.text, "ax"
    .align 4
    .global _Level3Vector
_Level3Vector:
    /* level3 interrupt vector */
    addi    a1, a1, -REG_CONTEXT_SIZE  /* allocate exception stack frame, etc. */
	wsr     a2, excsave3               /* save a2 to excsave3 temporarily */
    movi    a2, Level3VectorExt
    jx      a2

    .text
    .end    literal_prefix


    .begin  literal_prefix  .DebugExceptionVector.text
    .section        .DebugExceptionVector.text, "ax"
    .align 4
    .global _DebugExceptionVector
_DebugExceptionVector:
1:
    j 1b

    .text
    .end    literal_prefix


    .begin  literal_prefix  .NMIExceptionVector.text
    .section        .NMIExceptionVector.text, "ax"
    .align 4
    .global _NMIExceptionVector
_NMIExceptionVector:
    movi    a0, PS_INTLEVEL(3) | PS_UM | PS_WOE
    wsr     a0, PS
    rsync
    rsr     a2, EXCCAUSE
    mov     a6,a2
    mov     a7,a1
    call4   OsNMIHandler
    rfi     XCHAL_NMILEVEL

    .text
    .end    literal_prefix


    .begin  literal_prefix  .DoubleExceptionVector.text
    .section        .DoubleExceptionVector.text, "ax"
    .align 4
    .global _DoubleExceptionVector
_DoubleExceptionVector:
1:
    j 1b

    .text
    .end    literal_prefix


    .section .os.kernel.text, "ax"
    .align  4
    .global UserExcIntVecExt
    .global UserIntVecExt
UserExcIntVecExt:
    rsr     a2, exccause
    beqi    a2, OS_EXCCAUSE_INT, UserIntVecExt
    s32i    a3, a1, REG_OFF_AR15       /* save a3 temporary */
    movi    a3, ComExcVecExt           /* common exception vector */
    jx      a3

    .align 4
UserIntVecExt:
    rsr     a2, excsave1
    s32i    a2, a1, REG_OFF_AR14       /* save a2 temporary */

    rsr     a2, epc1                   /* get epc1 before possibly overflow */
    s32i    a2, a1, REG_OFF_PC         /* save epc1 */

    movi    a2, OS_PS_VECTOR_VALUE
    xsr     a2, ps                     /* PS.INTLEVEL=EXCMLEVEL(disable int), xsr exchange special register */
    rsync                              /* wait for WSR to PS to complete */
    s32i    a2, a1, REG_OFF_PS         /* save PS */
    l32i    a2, a1, REG_OFF_AR14       /* restore original a2 */

    OsSaveA4ToA7

    movi    a7, 1                      /* level1 */
    addi    a1, a1, REG_CONTEXT_SIZE
    call4   OsUserHwiHandler


.section .os.kernel.text, "ax"
    .align  4
    .global KernelExcIntVecExt
KernelExcIntVecExt:
    rsr     a2, exccause
    beqi    a2, OS_EXCCAUSE_INT, KernelIntVecExt
    s32i    a3, a1, REG_OFF_AR15       /* save a3 temporary */
    movi    a3, ComExcVecExt           /* common exception vector */
    jx      a3

    .align  4
KernelIntVecExt:
    rsr     a2, epc1                   /* get epc1 before possibly overflow */
    s32i    a2, a1, REG_OFF_PC         /* save epc1 */

    movi    a2, OS_PS_VECTOR_VALUE
    xsr     a2, ps                     /* PS.INTLEVEL=EXCMLEVEL(disable int), xsr exchange special register */
    rsync                              /* wait for WSR to PS to complete */
    s32i    a2, a1, REG_OFF_PS         /* save PS */
    l32i    a2, a1, REG_OFF_AR14       /* restore a2 immediately */

    OsSaveA4ToA7

    movi    a7, 1                      /* level1 */
    addi    a1, a1, REG_CONTEXT_SIZE
    call4   OsKernelHwiHandler


    .section .os.kernel.text, "ax"
    .align  4
    .global Level2VectorExt
Level2VectorExt:
    movi    a2, OS_PS_VECTOR_VALUE
    wsr     a2, ps                     /* PS.INTLEVEL=EXCMLEVEL */
    rsync                              /* wait for WSR to PS to complete */
    rsr     a2, excsave2               /* restore a2 immediately */

    OsSaveA4ToA7                       /* save a4~a7 */
    OsSaveEpcnEpsn a4, epc2, eps2      /* save epc2, eps2 */

    movi    a7, 2                      /* level2 */
    addi    a1, a1, REG_CONTEXT_SIZE

    bbci.l  a4, 5, 1f
    call4   OsUserHwiHandler
1:
    call4   OsKernelHwiHandler


    .section .os.kernel.text, "ax"
    .align  4
    .global Level3VectorExt
Level3VectorExt:
    movi    a2, OS_PS_VECTOR_VALUE
    wsr     a2, ps                     /* PS.INTLEVEL=EXCMLEVEL */
    rsync                              /* wait for WSR to PS to complete */
    rsr     a2, excsave3               /* restore a2 immediately */

    OsSaveA4ToA7                       /* save a4~a7 */
    OsSaveEpcnEpsn a4, epc3, eps3      /* save epc3, eps3 */

    movi    a7, 3                      /* level3 */
    addi    a1, a1, REG_CONTEXT_SIZE

    bbci.l  a4, 5, 1f
    call4   OsUserHwiHandler
1:
    call4   OsKernelHwiHandler

    .text

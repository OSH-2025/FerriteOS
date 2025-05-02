/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Memory Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

#ifndef _IT_LOS_MEM_H
#define _IT_LOS_MEM_H

#include "ostest.h"
#include "los_slab_pri.h"
#include "los_multipledlinkhead_pri.h"
#include "los_bitmap.h"

#include "los_memory_pri.h"
#include "los_memstat_pri.h"
#include "los_memory_internal.h"
#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
#include "los_slab_pri.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef LOSCFG_MEM_MUL_MODULE
#define MODULE0  0
#define MODULE1  1
#endif

#ifndef OS_MEM_WATERLINE
#define OS_MEM_WATERLINE NO
#endif

#ifdef LOSCFG_MEM_HEAD_BACKUP
#define LOS_MEM_NODE_MAGIC 0x55aadead
#endif

/**
 * @ingroup los_memory
 * Memory pool information structure
 */
typedef struct {
    VOID *pPoolAddr;                        /* <Starting address of a memory pool  */
    UINT32 uwPoolSize;                      /* <Memory pool size    */
#ifdef LOSCFG_MEM_TASK_STAT
    Memstat stat;
#endif
#ifdef LOSCFG_MEM_MUL_POOL
    VOID *pNextPool;
#endif
#ifdef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
    struct LosSlabControlHeader slabCtrlHdr;
#endif
} LOS_MEM_POOL_INFO;

/**
 * @ingroup los_memory
 * Memory linked list control node structure
 */
typedef struct TagLosMemCtlNode {
    LOS_DL_LIST stFreeNodeInfo; /* <Free memory node  */
    struct TagLosMemDynNode *pstPreNode; /* <Pointer to the previous memory node */

#ifdef LOSCFG_MEM_HEAD_BACKUP
    UINT32 uwGapSize;
    UINTPTR uwChecksum;  /* <magic = xor checksum> */
#endif

#ifdef LOSCFG_MEM_LEAKCHECK
    UINTPTR uwLR[LOS_RECORD_LR_CNT];
#endif

#ifdef LOSCFG_AARCH64
    UINT32 uwReserve;
#endif
    /* <Size and flag of the current node (the high two bits represent a flag, and the rest bits specify the size) */
    UINT32 uwSizeAndFlag;
} LOS_MEM_CTL_NODE;

/**
 * @ingroup los_memory
 * Memory linked list node structure
 */
typedef struct TagLosMemDynNode {
#ifdef LOSCFG_MEM_HEAD_BACKUP
    LOS_MEM_CTL_NODE stBackupNode;
#endif
    LOS_MEM_CTL_NODE stSelfNode;
} LOS_MEM_DYN_NODE;

#define SLAB_BUCKET_NUM    ((TEST_MEM_SIZE / SLAB_MEM_ALLOCATOR_SIZE) >> SLAB_MEM_BUCKET_SIZE_TYPE)
#define TEST_MEM_SIZE LOSCFG_TEST_PLATFORM_MEMPOOL_SIZE
#define TEST_MEM_SIZE_SMALL 0x2100
#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
#define SLAB_BUCKET_NUM    ((TEST_MEM_SIZE_SMALL / SLAB_MEM_ALLOCATOR_SIZE) >> SLAB_MEM_BUCKET_SIZE_TYPE)
#endif
extern VOID* g_poolSlab;

#define LOS_DLNK_HEAD_SIZE sizeof(LosMultipleDlinkHead)
#define LOS_DLNK_NODE_HEAD_SIZE sizeof(LOS_MEM_DYN_NODE)  // 16 bytes
#define MIN_DLNK_POOL_SIZE (LOS_DLNK_HEAD_SIZE + 2 * LOS_DLNK_NODE_HEAD_SIZE + sizeof(LOS_MEM_POOL_INFO))
#define LOS_DLNK_NODE_GET_USED_FLAG(sizeAndFlag) ((sizeAndFlag) & 0x80000000)

#define HEAD_SIZE  (LOS_DLNK_NODE_HEAD_SIZE)

#ifdef OS_MEM_WATERLINE
#define TEST_MEM_MINIUSE (LOS_DLNK_HEAD_SIZE + LOS_DLNK_NODE_HEAD_SIZE + sizeof(LOS_MEM_POOL_INFO))
#endif

extern VOID* g_pool;
extern VOID MEM_START(VOID);
extern VOID MEM_END(VOID);
extern VOID MEM_INIT(VOID);
extern VOID MEM_FREE(VOID);
extern VOID MEM_INIT_SLAB(VOID);
extern VOID MEM_FREE_SLAB(VOID);
extern VOID MEM_START_GetMemInfo(VOID *pool);
extern VOID MEM_END_GetMemInfo(VOID *pool);
extern VOID MEM_INFO_CHECK(VOID);
extern UINT32 MemGetFreeSize(VOID *pool);
extern UINT32 CalPow(UINT32 exp);

#ifdef LOSCFG_TEST_KERNEL_BASE_MEM
extern UINT32 OsMemBackupDoRestore(VOID *pPool, LOS_MEM_DYN_NODE *pstNodePre, LOS_MEM_DYN_NODE *pstNode);
extern LOS_MEM_DYN_NODE *OsMemNodeNextGet(VOID *pPool, LOS_MEM_DYN_NODE *pstNode);
extern LOS_MEM_DYN_NODE *OsMemNodePrevGet(VOID *pPool, LOS_MEM_DYN_NODE *pstNode);
extern VOID LOS_MemBadNodeShow(VOID *pPool);
#endif

VOID ItLosMem001(VOID);
VOID ItLosMem002(VOID);
#ifdef LOSCFG_KERNEL_MEM_BESTFIT
VOID ItLosMem003(VOID);
VOID ItLosMem004(VOID);
#ifndef LOSCFG_KERNEL_MEM_SLAB_EXTENTION
VOID ItLosMem005(VOID);
VOID ItLosMem006(VOID);
#endif
VOID ItLosMem007(VOID);
#endif
VOID ItLosMem008(VOID);
#ifdef LOSCFG_BASE_MEM_NODE_SIZE_CHECK
VOID ItLosMem009(VOID);
VOID ItLosMem010(VOID);
#endif

#ifdef LOSCFG_KERNEL_MEM_BESTFIT
VOID ItLosMem011(VOID);
VOID ItLosMem012(VOID);
VOID ItLosMem013(VOID);
#endif
VOID ItLosMem014(VOID);
#ifdef LOS_MEM_SLAB
VOID ItLosMemSlab001(VOID);
VOID ItLosMemSlab002(VOID);
VOID ItLosMemSlab003(VOID);
VOID ItLosMemSlab004(VOID);
VOID ItLosMemSlab005(VOID);
#endif
#ifdef LOSCFG_MEM_MUL_MODULE
VOID ItLosMemSlink001(VOID);
#endif
VOID ItLosMemSlink002(VOID);

VOID IT_LOS_MEM_BMP(VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _IT_LOS_MEM_H */

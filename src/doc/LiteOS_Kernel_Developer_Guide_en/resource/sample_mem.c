#include "los_config.h"
#include "los_memory.h"
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define TEST_POOL_SIZE (2*1024*1024)
UINT8 g_testPool[TEST_POOL_SIZE];

VOID Example_DynMem(VOID)
{
    UINT32 *mem = NULL;
    UINT32 ret;

    ret = LOS_MemInit(g_testPool, TEST_POOL_SIZE);
    if (LOS_OK  == ret) {
        dprintf("The memory pool is successfully initialized.\n");
    } else {
        dprintf("Failed to initialize the memory pool.\n");
        return;
    }

    /*Allocate memory.*/
    mem = (UINT32 *)LOS_MemAlloc(g_testPool, 4);
    if (NULL == mem) {
        dprintf("Failed to allocate memory.\n");
        return;
    }
    dprintf("Memory is successfully allocated.\n");

    /*Assign a value.*/
    *mem = 828;
    dprintf("*mem = %d\n", *mem);

    /*Free up memory.*/
    ret = LOS_MemFree(g_testPool, mem);
    if (LOS_OK == ret) {
        dprintf("Memory is successfully freed up.\n");
    } else {
        dprintf("Failed to free up memory.\n");
    }

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


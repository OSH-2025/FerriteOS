#include "los_config.h"
#include "los_membox.h"
#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID Example_StaticMem(VOID)
{
    UINT32 *mem = NULL;
    UINT32 blkSize = 10;
    UINT32 boxSize = 100;
    UINT32 boxMem[1000];
    UINT32 ret;

    ret = LOS_MemboxInit(&boxMem[0], boxSize, blkSize);
    if(ret != LOS_OK) {
        printf("Failed to initialize the memory pool.\n");
        return;
    } else {
        printf("The memory pool is successfully initialized.\n");
    }

    /*Apply for a memory block.*/
    mem = (UINT32 *)LOS_MemboxAlloc(boxMem);
    if (NULL == mem) {
        printf("Failed to allocate memory.\n");
        return;
    }
    printf("Memory is successfully allocated.\n");

    /*Assign a value.*/
    *mem = 828;
    printf("*mem = %d\n", *mem);

     /*Clear content in the memory.*/
     LOS_MemboxClr(boxMem, mem);
     printf("Content is successfully cleared from memory.\n *mem = %d\n", *mem);

    /*Free up memory.*/
    ret = LOS_MemboxFree(boxMem, mem);
    if (LOS_OK == ret) {
        printf("Memory is successfully freed up.\n");
    } else {
        printf("Failed to free up memory.\n");
    }

    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

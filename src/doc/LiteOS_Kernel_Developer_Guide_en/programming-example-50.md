# Programming Example<a name="EN-US_TOPIC_0311018444"></a>

## Example Description<a name="en-us_topic_0175230506_section57243849151319"></a>

In the programming example, the following steps will be performed:

1.  Initialize a doubly linked list.
2.  Insert a node into the list.
3.  Delete a node from the list.
4.  Check whether the insertion and deletion was successful.

## Example Code<a name="en-us_topic_0175230506_section31107981151351"></a>

The code is as follows:

```c
#include "stdio.h"
#include "los_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

static UINT32 DLlist_sample(VOID)
{
    LOS_DL_LIST DLlist = {NULL,NULL};
    LOS_DL_LIST DLlistNode01 = {NULL,NULL};
    LOS_DL_LIST DLlistNode02 = {NULL,NULL};
    LOS_DL_LIST DLlistNode03 = {NULL,NULL};

    dprintf("Initial head\n");
    LOS_ListInit(&DLlist);

    LOS_ListAdd(&DLlist, &DLlistNode01);
    if (DLlistNode01.pstNext == &DLlist && DLlistNode01.pstPrev == &DLlist) {
        dprintf("Add DLlistNode01 success \n");
    }

    LOS_ListTailInsert(&DLlist, &DLlistNode02);
    if (DLlistNode02.pstNext == &DLlist && DLlistNode02.pstPrev == &DLlistNode01) {
        dprintf("Tail insert DLlistNode02 success \n");
    }

    LOS_ListHeadInsert(&DLlistNode02, &DLlistNode03);
    if (DLlistNode03.pstNext == &DLlist && DLlistNode03.pstPrev == &DLlistNode02) {
        dprintf("Head insert DLlistNode03 success \n");
    }

    LOS_ListDelInit(&DLlistNode03);
    LOS_ListDelete(&DLlistNode01);
    LOS_ListDelete(&DLlistNode02);

    if (LOS_ListEmpty(&DLlist)) {
        dprintf("Delete success \n");
    }

    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
```

## Verification<a name="en-us_topic_0175230506_section9844937144850"></a>

The verification result is as follows:

```
Initial head 
Add DLlistNode01 success 
Tail insert DLlistNode02 success 
Head insert DLlistNode03 success 
Delete success 
```


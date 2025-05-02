# Programming Example<a name="EN-US_TOPIC_0311018390"></a>

## Example Description<a name="en-us_topic_0175230465_section5049564511595"></a>

Prerequisite: Dynamic memory has been configured in the  **menuconfig**  menu.

In the programming example, the following steps will be performed:

1.  Initialize a dynamic memory pool.
2.  Apply for a memory block from the dynamic memory pool.
3.  Store data in the memory block.
4.  Print the data in the memory block.
5.  Free up the memory block.

## Programming Example<a name="en-us_topic_0175230465_section60752912413"></a>

```c
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
```

## Verification<a name="en-us_topic_0175230465_section3983848912449"></a>

The verification result is as follows:

```
The memory pool is successfully initialized.
Memory is successfully allocated.
*mem = 828
Memory is successfully freed up.
```

## Complete Code<a name="en-us_topic_0175230465_section5427588511595"></a>

[sample\_mem.c](resource/sample_mem.c)


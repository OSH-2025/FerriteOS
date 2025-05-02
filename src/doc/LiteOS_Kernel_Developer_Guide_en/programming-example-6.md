# Programming Example<a name="EN-US_TOPIC_0311018394"></a>

## Example Description<a name="en-us_topic_0175230458_section21541389142037"></a>

Prerequisite: Static memory has been configured in the  **menuconfig**  menu.

In the programming example, the following steps will be performed:

1.  Initialize a static memory pool.
2.  Apply for a memory block from the static memory pool.
3.  Store data in the memory block.
4.  Print the data in the memory block.
5.  Clear the data in the memory block.
6.  Free up the memory block.

## Programming Example<a name="en-us_topic_0175230458_section47408530142115"></a>

```c
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
```

## Verification<a name="en-us_topic_0175230458_section45784923142519"></a>

The verification result is as follows:

```
The memory pool is successfully initialized.
Memory is successfully allocated.
*mem = 828
Content is successfully cleared from memory.
*mem = 0
Memory is successfully freed up.
```

## Complete Code<a name="en-us_topic_0175230458_section18066546142537"></a>

[sample\_membox.c](resource/sample_membox.c)


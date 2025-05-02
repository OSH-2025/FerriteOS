# Memory Validity Check<a name="EN-US_TOPIC_0312244438"></a>

## Usage Scenarios<a name="en-us_topic_0175230365_section112041842161114"></a>

Overwriting of the service memory will cause overwriting of the control header of the memory node. However, service exceptions will not occur immediately, but after a long period of time. Therefore, the service logic is so complex that the location where memory is overwritten cannot be located easily.

## Function Description<a name="en-us_topic_0175230365_section46400139155028"></a>

When this function is enabled, memory validity check is added to the dynamic memory application interface to check the validity of all node control headers in the memory pool. If the dynamic memory node is overwritten, an exception is promptly triggered to display an error message and narrow down the fault location scope.

## How to Use<a name="en-us_topic_0175230365_section300741316138"></a>

1.  Run the  **make menuconfig**  command to enable memory validity check.

    This function depends on  **LOSCFG\_BASE\_MEM\_NODE\_INTEGRITY\_CHECK**. Before using this function, enable  **Enable integrity check or not**.

    ```
    Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable integrity check or not
    ```

2.  After memory overwriting occurs, the next memory application operation triggers an exception and the information about the overwritten node and the previous node will be provided. A preliminary analysis can be performed to determine whether the memory overwriting is caused by the overflow of the previous node. The memory overwriting occurs between the previous and the current memory application. You can run the shell command  [memcheck](/shell/doc_en/memcheck.md)  to view exception information.

## Precautions<a name="en-us_topic_0175230365_section4175643316212"></a>

Enabling this function greatly degrades the system performance when you apply for memory. Therefore, you are advised to enable this function only for fault locating. By default, this function is disabled.

## Example for Locating Memory Overwriting<a name="en-us_topic_0175230365_section18827202214589"></a>

Construct a  **memset**  operation that exceeds the memory length to construct memory overwriting. As a result, the memory node is damaged. The code is constructed as follows:

```c
VOID SampleFunc(VOID *p)
{
    memset(p, 0, 0x110); // memset that exceeds the length, which is used to construct memory overwriting
}

UINT32 Test(UINT32 argc, CHAR **args)
{
    void *p1, *p2;

    p1 = LOS_MemAlloc((void*)OS_SYS_MEM_ADDR, 0x100);
    p2 = LOS_MemAlloc((void*)OS_SYS_MEM_ADDR, 0x100);
    dprintf("p1 = %p, p2 = %p \n", p1, p2);
    SampleFunc(p1); // As p1 and p2 are adjacent, when the length of memset exceeds the memory size of p1, memory overwriting occurs on p2.

    LOS_MemFree(OS_SYS_MEM_ADDR, (void *)p1);
    LOS_MemFree(OS_SYS_MEM_ADDR, (void *)p2);
    return 0;
}
```

After the preceding code is executed, run the shell command  **memcheck**. The command output is shown in the following figure.

![](figures/run_memcheck.png)

The preceding figure shows that the printed information is incorrect.

-   As shown in mark 2, "cur node: 0x8034fbfc" indicates the node where the memory is illegally overwritten, and "pre node: 0x8034faec" indicates the node before the illegally overwritten node. As shown in mark 3, "pre node was allocated by task:SerialShellTask" indicates that memory overwriting occurs in the SerialShellTask task.
-   Mark 1 displays the start addresses of the P1 and P2 memories. "p2 = 0x8034fc0c" minus the control header size 0x10 equals the address printed by the cur node \(p2 – 0x10 = 0x8034fbfc\), and the p2 memory is illegally overwritten. According to the code, p1 and p2 are two adjacent nodes \(which can also be seen from the printed p1 and p2 addresses, that is, p1 + p1 size + control header size = p2, 0x8034fafc + 0x100 + 0x10 = 0x8034fc0c\). Therefore, "pre node: 0x8034faec" should be the address of p1. The p1 address shown in mark 1 is "p1 = 0x8034fafc", that is, the pre node plus the control header size 0x10 \(0x8034faec + 0x10 = 0x8034fafc\).


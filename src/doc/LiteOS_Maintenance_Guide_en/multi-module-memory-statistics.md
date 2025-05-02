# Multi-Module Memory Statistics<a name="EN-US_TOPIC_0312244435"></a>

## Usage Scenarios<a name="en-us_topic_0175230547_section1106945162938"></a>

The system has clear service modules and users need to collect statistics on the memory usage of each module.

## Function Description<a name="en-us_topic_0175230547_section46400139155028"></a>

Huawei LiteOS provides a set of encapsulation interfaces based on the kernel memory interface, adding the module ID input parameter. When performing memory operations in different modules and calling related encapsulation interfaces, users can collect statistics about memory usage in all modules and memory usage of a specified module by module ID.

<a name="en-us_topic_0175230547_table62913171618"></a>
<table><thead align="left"><tr id="en-us_topic_0175230547_row29861117131613"><th class="cellrowborder" valign="top" width="22.24%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230547_p1642661951613"><a name="en-us_topic_0175230547_p1642661951613"></a><a name="en-us_topic_0175230547_p1642661951613"></a>Function</p>
</th>
<th class="cellrowborder" valign="top" width="23.21%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230547_p1442671919166"><a name="en-us_topic_0175230547_p1442671919166"></a><a name="en-us_topic_0175230547_p1442671919166"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="54.55%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230547_p174261519191610"><a name="en-us_topic_0175230547_p174261519191610"></a><a name="en-us_topic_0175230547_p174261519191610"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230547_row8291819162"><td class="cellrowborder" rowspan="4" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230547_p9291113162"><a name="en-us_topic_0175230547_p9291113162"></a><a name="en-us_topic_0175230547_p9291113162"></a>Allocate and free up dynamic memory to a specified module, and record the amount of the memory</p>
</td>
<td class="cellrowborder" valign="top" width="23.21%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230547_p17292017165"><a name="en-us_topic_0175230547_p17292017165"></a><a name="en-us_topic_0175230547_p17292017165"></a>LOS_MemMalloc</p>
</td>
<td class="cellrowborder" valign="top" width="54.55%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230547_p329181131610"><a name="en-us_topic_0175230547_p329181131610"></a><a name="en-us_topic_0175230547_p329181131610"></a>Allocates memory of the required size from a specified dynamic memory pool to a specified module.</p>
</td>
</tr>
<tr id="en-us_topic_0175230547_row62916101620"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230547_p5305117164"><a name="en-us_topic_0175230547_p5305117164"></a><a name="en-us_topic_0175230547_p5305117164"></a>LOS_MemMfree</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230547_p113011131618"><a name="en-us_topic_0175230547_p113011131618"></a><a name="en-us_topic_0175230547_p113011131618"></a>Free ups the memory block of a specified module.</p>
</td>
</tr>
<tr id="en-us_topic_0175230547_row1330412161"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230547_p4305171615"><a name="en-us_topic_0175230547_p4305171615"></a><a name="en-us_topic_0175230547_p4305171615"></a>LOS_MemMallocAlign</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230547_p1930131121614"><a name="en-us_topic_0175230547_p1930131121614"></a><a name="en-us_topic_0175230547_p1930131121614"></a>Applies for a memory block of the required size whose address is aligned based on <strong id="en-us_topic_0175230547_b18497133131214"><a name="en-us_topic_0175230547_b18497133131214"></a><a name="en-us_topic_0175230547_b18497133131214"></a>boundary</strong>, from a dynamic memory pool and allocates the memory block to the specified module.</p>
</td>
</tr>
<tr id="en-us_topic_0175230547_row83013111166"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230547_p193041181619"><a name="en-us_topic_0175230547_p193041181619"></a><a name="en-us_topic_0175230547_p193041181619"></a>LOS_MemMrealloc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230547_p713816242294"><a name="en-us_topic_0175230547_p713816242294"></a><a name="en-us_topic_0175230547_p713816242294"></a>Re-allocates a memory block of the required size to the specified module and copies content from the original block. If the new memory block is successfully applied for, the original one will be freed up.</p>
</td>
</tr>
<tr id="en-us_topic_0175230547_row19305121617"><td class="cellrowborder" valign="top" width="22.24%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230547_p73016117166"><a name="en-us_topic_0175230547_p73016117166"></a><a name="en-us_topic_0175230547_p73016117166"></a>Obtain the memory usage of a specified module</p>
</td>
<td class="cellrowborder" valign="top" width="23.21%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230547_p8301819168"><a name="en-us_topic_0175230547_p8301819168"></a><a name="en-us_topic_0175230547_p8301819168"></a>LOS_MemMusedGet</p>
</td>
<td class="cellrowborder" valign="top" width="54.55%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230547_p1305121613"><a name="en-us_topic_0175230547_p1305121613"></a><a name="en-us_topic_0175230547_p1305121613"></a>Obtains the memory usage of a specified module. The unit is byte.</p>
</td>
</tr>
</tbody>
</table>

## How to Use<a name="en-us_topic_0175230547_section2552676516411"></a>

1.  Run the  **make menuconfig**  command to enable the multi-module memory statistics function.
    -   Currently, only the  **bestfit**  memory management algorithm supports this function. You need to enable  **LOSCFG\_KERNEL\_MEM\_BESTFIT**.

        ```
        Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
        ```

    -   This function depends on  **LOSCFG\_MEM\_MUL\_MODULE**. Before using this function, enable  **Enable Memory module statistics**.

        ```
        Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable Memory module statistics
        ```

2.  Each service module is configured with a unique module ID. In the module code, the memory operation calls the related interface and transfers the related module ID.
3.  The LOS\_MemMusedGet interface can obtain the memory usage of a specified module, which can be used for optimization analysis of module memory usage.

## Precautions<a name="en-us_topic_0175230547_section59220665164038"></a>

-   The value of module ID is specified by the macro  **MEM\_MODULE\_MAX**. When the number of system modules exceeds the value specified, the  **MEM\_MODULE\_MAX**  needs to be modified accordingly.
-   All memory operations in the module need to call the encapsulation interface. Otherwise, the statistics may be inaccurate.
-   Currently, only the  **bestfit**  memory management algorithm supports this function. You need to enable  **LOSCFG\_KERNEL\_MEM\_BESTFIT**.

## Programming Example<a name="en-us_topic_0175230547_section620944813143"></a>

```c
void test(void)
{
    void *ptr = NULL;
    void *ptrTmp = NULL;
    UINT32 size = 0x10;
    UINT32 module = 0;
    UINT32 memUsed = 0;

    ptr = LOS_MemMalloc(OS_SYS_MEM_ADDR, size, module);
    if (ptr == NULL) {
        PRINTK("module %d malloc failed\n", module);
    } else {
        PRINTK("module %d malloc successed\n", module);
    }

    memUsed = LOS_MemMusedGet(module);
    PRINTK("module %d mem used size %d\n", module, memUsed);

    module = 1;
    ptrTmp = LOS_MemMalloc(OS_SYS_MEM_ADDR, size, module);
    if (ptrTmp == NULL) {
        PRINTK("module %d malloc failed\n", module);
    } else {
        PRINTK("module %d malloc successed\n", module);
    }

    memUsed = LOS_MemMusedGet(module);
    PRINTK("module %d mem used size %d\n", module, memUsed);

    module = 0;
    LOS_MemMfree(OS_SYS_MEM_ADDR, ptr, module);
    module = 1;
    LOS_MemMfree(OS_SYS_MEM_ADDR, ptrTmp, module);
}

```

Logs:

```
module 0 malloc successed
module 0 mem used size 32
module 1 malloc successed
module 1 mem used size 32
```


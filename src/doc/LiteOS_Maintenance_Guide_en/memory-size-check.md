# Memory Size Check<a name="EN-US_TOPIC_0312244439"></a>

## Usage Scenarios<a name="en-us_topic_0175230376_section38699997192621"></a>

When the  **memset**  and  **memcpy**  functions are called for operations on the dynamic memory, memory overwriting caused by overflow occurs.

## Function Description<a name="en-us_topic_0175230376_section46400139155028"></a>

For the  **memset**  and  **memcpy**  operations, when the input parameter is a dynamic memory node, the system checks the actual size of the memory node and the size specified by the input parameter. If the specified size of the node is greater than the actual size, the system displays an error message and cancels the  **memset**  or  **memcpy**  operation to prevent operation violation. When dynamic memory overwriting occurs, you can enable this function to locate the fault.

<a name="en-us_topic_0175230376_table6587685163114"></a>
<table><thead align="left"><tr id="en-us_topic_0175230376_row8355205704514"><th class="cellrowborder" valign="top" width="34.86%" id="mcps1.1.3.1.1"><p id="en-us_topic_0175230376_a2dffaf83b2894662b99e38092a81f4c0"><a name="en-us_topic_0175230376_a2dffaf83b2894662b99e38092a81f4c0"></a><a name="en-us_topic_0175230376_a2dffaf83b2894662b99e38092a81f4c0"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="65.14%" id="mcps1.1.3.1.2"><p id="en-us_topic_0175230376_a78e2e39f5b894c1d9e717315aceb9aeb"><a name="en-us_topic_0175230376_a78e2e39f5b894c1d9e717315aceb9aeb"></a><a name="en-us_topic_0175230376_a78e2e39f5b894c1d9e717315aceb9aeb"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230376_row10513314151950"><td class="cellrowborder" valign="top" width="34.86%" headers="mcps1.1.3.1.1 "><p id="en-us_topic_0175230376_p663627871946"><a name="en-us_topic_0175230376_p663627871946"></a><a name="en-us_topic_0175230376_p663627871946"></a>LOS_MemCheckLevelSet</p>
</td>
<td class="cellrowborder" valign="top" width="65.14%" headers="mcps1.1.3.1.2 "><p id="en-us_topic_0175230376_p66766791946"><a name="en-us_topic_0175230376_p66766791946"></a><a name="en-us_topic_0175230376_p66766791946"></a>Checks the memory check level.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row57717782151954"><td class="cellrowborder" valign="top" width="34.86%" headers="mcps1.1.3.1.1 "><p id="en-us_topic_0175230376_p591081821946"><a name="en-us_topic_0175230376_p591081821946"></a><a name="en-us_topic_0175230376_p591081821946"></a>LOS_MemCheckLevelGet</p>
</td>
<td class="cellrowborder" valign="top" width="65.14%" headers="mcps1.1.3.1.2 "><p id="en-us_topic_0175230376_p230334311946"><a name="en-us_topic_0175230376_p230334311946"></a><a name="en-us_topic_0175230376_p230334311946"></a>Obtains the memory check level.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row193316438483"><td class="cellrowborder" valign="top" width="34.86%" headers="mcps1.1.3.1.1 "><p id="en-us_topic_0175230376_p846713011116"><a name="en-us_topic_0175230376_p846713011116"></a><a name="en-us_topic_0175230376_p846713011116"></a>LOS_MemNodeSizeCheck</p>
</td>
<td class="cellrowborder" valign="top" width="65.14%" headers="mcps1.1.3.1.2 "><p id="en-us_topic_0175230376_p104671930316"><a name="en-us_topic_0175230376_p104671930316"></a><a name="en-us_topic_0175230376_p104671930316"></a>Obtains the total size and available size of a specified memory block.</p>
</td>
</tr>
</tbody>
</table>

## Error Codes<a name="en-us_topic_0175230376_section1881784174416"></a>

<a name="en-us_topic_0175230376_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230376_row2267197395642"><th class="cellrowborder" valign="top" width="7.5200000000000005%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230376_p1908783195642"><a name="en-us_topic_0175230376_p1908783195642"></a><a name="en-us_topic_0175230376_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="29.03%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230376_p261046995642"><a name="en-us_topic_0175230376_p261046995642"></a><a name="en-us_topic_0175230376_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="11.58%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230376_p1012144095642"><a name="en-us_topic_0175230376_p1012144095642"></a><a name="en-us_topic_0175230376_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="26.790000000000003%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230376_p1453028795642"><a name="en-us_topic_0175230376_p1453028795642"></a><a name="en-us_topic_0175230376_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="25.080000000000002%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230376_p2753561710026"><a name="en-us_topic_0175230376_p2753561710026"></a><a name="en-us_topic_0175230376_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230376_row6366372295642"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230376_p5648782795642"><a name="en-us_topic_0175230376_p5648782795642"></a><a name="en-us_topic_0175230376_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="29.03%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230376_p261116438345"><a name="en-us_topic_0175230376_p261116438345"></a><a name="en-us_topic_0175230376_p261116438345"></a>LOS_ERRNO_MEMCHECK_PARA_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="11.58%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230376_p660114510360"><a name="en-us_topic_0175230376_p660114510360"></a><a name="en-us_topic_0175230376_p660114510360"></a>0x02000101</p>
</td>
<td class="cellrowborder" valign="top" width="26.790000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230376_p165631814114010"><a name="en-us_topic_0175230376_p165631814114010"></a><a name="en-us_topic_0175230376_p165631814114010"></a>The LOS_MemNodeSizeCheck input parameter contains a null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="25.080000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230376_p54251328201810"><a name="en-us_topic_0175230376_p54251328201810"></a><a name="en-us_topic_0175230376_p54251328201810"></a>Input a valid pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row18396131112373"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230376_p163971911103714"><a name="en-us_topic_0175230376_p163971911103714"></a><a name="en-us_topic_0175230376_p163971911103714"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="29.03%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230376_p15397191113710"><a name="en-us_topic_0175230376_p15397191113710"></a><a name="en-us_topic_0175230376_p15397191113710"></a>LOS_ERRNO_MEMCHECK_OUTSIDE</p>
</td>
<td class="cellrowborder" valign="top" width="11.58%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230376_p739751114372"><a name="en-us_topic_0175230376_p739751114372"></a><a name="en-us_topic_0175230376_p739751114372"></a>0x02000102</p>
</td>
<td class="cellrowborder" valign="top" width="26.790000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230376_p193971711193717"><a name="en-us_topic_0175230376_p193971711193717"></a><a name="en-us_topic_0175230376_p193971711193717"></a>The memory address is not in the valid range.</p>
</td>
<td class="cellrowborder" valign="top" width="25.080000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230376_p4397811113714"><a name="en-us_topic_0175230376_p4397811113714"></a><a name="en-us_topic_0175230376_p4397811113714"></a>The input memory address is not within the memory management range. Ignore this error code.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row1077061733719"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230376_p10770111716371"><a name="en-us_topic_0175230376_p10770111716371"></a><a name="en-us_topic_0175230376_p10770111716371"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="29.03%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230376_p177703178376"><a name="en-us_topic_0175230376_p177703178376"></a><a name="en-us_topic_0175230376_p177703178376"></a>LOS_ERRNO_MEMCHECK_NO_HEAD</p>
</td>
<td class="cellrowborder" valign="top" width="11.58%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230376_p7770191743713"><a name="en-us_topic_0175230376_p7770191743713"></a><a name="en-us_topic_0175230376_p7770191743713"></a>0x02000103</p>
</td>
<td class="cellrowborder" valign="top" width="26.790000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230376_p5770101743720"><a name="en-us_topic_0175230376_p5770101743720"></a><a name="en-us_topic_0175230376_p5770101743720"></a>The memory address has been released or is a wild pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="25.080000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230376_p187701217163712"><a name="en-us_topic_0175230376_p187701217163712"></a><a name="en-us_topic_0175230376_p187701217163712"></a>Verify that the input memory address is valid.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row1577222415378"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230376_p1477282433715"><a name="en-us_topic_0175230376_p1477282433715"></a><a name="en-us_topic_0175230376_p1477282433715"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="29.03%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230376_p1677242453718"><a name="en-us_topic_0175230376_p1677242453718"></a><a name="en-us_topic_0175230376_p1677242453718"></a>LOS_ERRNO_MEMCHECK_WRONG_LEVEL</p>
</td>
<td class="cellrowborder" valign="top" width="11.58%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230376_p18772162443714"><a name="en-us_topic_0175230376_p18772162443714"></a><a name="en-us_topic_0175230376_p18772162443714"></a>0x02000104</p>
</td>
<td class="cellrowborder" valign="top" width="26.790000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230376_p18772142493717"><a name="en-us_topic_0175230376_p18772142493717"></a><a name="en-us_topic_0175230376_p18772142493717"></a>The memory detection level is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="25.080000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230376_p1177210244376"><a name="en-us_topic_0175230376_p1177210244376"></a><a name="en-us_topic_0175230376_p1177210244376"></a>Use LOS_MemCheckLevelGet to check the memory check level and use LOS_MemCheckLevelSet to configure a valid memory check level.</p>
</td>
</tr>
<tr id="en-us_topic_0175230376_row126753302374"><td class="cellrowborder" valign="top" width="7.5200000000000005%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230376_p18675193014379"><a name="en-us_topic_0175230376_p18675193014379"></a><a name="en-us_topic_0175230376_p18675193014379"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="29.03%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230376_p7675163011372"><a name="en-us_topic_0175230376_p7675163011372"></a><a name="en-us_topic_0175230376_p7675163011372"></a>LOS_ERRNO_MEMCHECK_DISABLED</p>
</td>
<td class="cellrowborder" valign="top" width="11.58%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230376_p967513014372"><a name="en-us_topic_0175230376_p967513014372"></a><a name="en-us_topic_0175230376_p967513014372"></a>0x02000105</p>
</td>
<td class="cellrowborder" valign="top" width="26.790000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230376_p19675530163715"><a name="en-us_topic_0175230376_p19675530163715"></a><a name="en-us_topic_0175230376_p19675530163715"></a>The memory check is disabled.</p>
</td>
<td class="cellrowborder" valign="top" width="25.080000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230376_p186751830193715"><a name="en-us_topic_0175230376_p186751830193715"></a><a name="en-us_topic_0175230376_p186751830193715"></a>Use LOS_MemCheckLevelSet to enable memory check.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](../LiteOS_Kernel_Developer_Guide_en/development-guidelines-14.md). Bits 8 to 15 belong to the dynamic memory module, and the value is  **0x01**.

## How to Use<a name="en-us_topic_0175230376_section300741316138"></a>

Run the  **make menuconfig**  command to check the memory size.

-   Currently, only the  **bestfit**  memory management algorithm supports this function. Therefore, you need to enable  **LOSCFG\_KERNEL\_MEM\_BESTFIT**.

    ```
    Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
    ```

-   In addition, this function depends on  **LOSCFG\_BASE\_MEM\_NODE\_SIZE\_CHECK**. You can enable the macro by enabling  **Enable size check or not**.

    ```
    Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable size check or not
    ```


## Precautions<a name="en-us_topic_0175230376_section4175643316212"></a>

After this function is enabled, the performance of  **memset**  and  **memcpy**  degrades. You are advised to enable this function only for locating memory overwriting issues. By default, this function is disabled.

## Example for Locating Memory Overwriting<a name="en-us_topic_0175230376_section93997248518"></a>

Construct the  **memset**  and  **memcpy**  operations that exceed the memory length to construct memory overwriting. The code is constructed as follows:

```c
VOID test(VOID)
{
    UINT32 size = 0x100;

    VOID *poolAddr1 = LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, size);
    if (poolAddr1 == NULL) {
        PRINTK("malloc poolAddr1 failed\n");
        return;
    } else {
        PRINTK("malloc poolAddr1 %p successed\n", poolAddr1);
    }

    VOID *poolAddr2 = LOS_MemAlloc((VOID *)OS_SYS_MEM_ADDR, size);
    if (poolAddr2 == NULL) {
        PRINTK("malloc poolAddr2 failed\n");
        return;
    } else {
        PRINTK("malloc poolAddr2 %p successed\n", poolAddr2);
    }

    LOS_MemCheckLevelSet(LOS_MEM_CHECK_LEVEL_LOW);      // Used to enable the check of the length of memset and memcpy.

    PRINTK("memset poolAddr1 overflow\n"); 
    memset(poolAddr1, 0x10, size * 2);                  // memset that exceeds the length
    PRINTK("memset poolAddr1\n"); 
    memset(poolAddr1, 0x10, size);                      // memset with a proper length

    PRINTK("memcpy poolAddr2 overflow\n"); 
    memcpy(poolAddr2, poolAddr1, size * 2);            // memcpy that exceeds the length

    PRINTK("memcpy poolAddr2\n"); 
    memcpy(poolAddr2, poolAddr1, size);                // memcpy with a proper length

    LOS_MemCheckLevelSet(LOS_MEM_CHECK_LEVEL_DISABLE); // Used to disable the check of the length of memset and memcpy.

    LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, (VOID *)poolAddr1);
    LOS_MemFree((VOID *)OS_SYS_MEM_ADDR, (VOID *)poolAddr2);

    return 0;
}
```

Logs:

```
malloc poolAddr1 0x80349514 successed
malloc poolAddr2 0x80349624 successed
LOS_MemCheckLevelSet: LOS_MEM_CHECK_LEVEL_LOW
memset poolAddr1 overflow
[ERR] ---------------------------------------------
memset: dst inode availSize is not enough availSize = 0x100, memcpy length = 0x200
runTask->taskName = osMain
runTask->taskId = 64
*******backtrace begin*******
traceback 0 -- lr = 0x80209798    fp = 0x802c6930
traceback 1 -- lr = 0x80210fc4    fp = 0x802c6954
traceback 2 -- lr = 0x8020194c    fp = 0x802c6994
traceback 3 -- lr = 0x80201448    fp = 0x802c699c
traceback 4 -- lr = 0x802012fc    fp = 0x0
[ERR] ---------------------------------------------
memset poolAddr1
memcpy poolAddr2 overflow
[ERR] ---------------------------------------------
memcpy: dst inode availSize is not enough availSize = 0x100, memcpy length = 0x200
runTask->taskName = osMain
runTask->taskId = 64
*******backtrace begin*******
traceback 0 -- lr = 0x80209798    fp = 0x802c6930
traceback 1 -- lr = 0x8020dbc4    fp = 0x802c6954
traceback 2 -- lr = 0x8020194c    fp = 0x802c6994
traceback 3 -- lr = 0x80201448    fp = 0x802c699c
traceback 4 -- lr = 0x802012fc    fp = 0x0
[ERR] ---------------------------------------------
memcpy poolAddr2
```

As the size check is enabled, invalid  **memset**  and  **memcpy**  operations are canceled, and error information is provided. "runTask-\>taskName = osMain" indicates that the illegal operation occurs in the osMain function, and the values of registers  **lr**  and  **fp**  are printed. You can open the .asm disassembly file generated after compilation. By default, the file is generated in the  **Huawei\_LiteOS/out/**_Platform name_  directory. Compare the value of the  **lr**  register to check the nested function calling.


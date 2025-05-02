# Multi-Memory Pool Mechanism<a name="EN-US_TOPIC_0312244436"></a>

## Usage Scenarios<a name="en-us_topic_0175230456_section43996196164637"></a>

If multiple dynamic memory pools are used in the system, each memory pool shall be managed and the usage statistics of each memory pool shall be collected.

## Function Description<a name="en-us_topic_0175230456_section46400139155028"></a>

In the system memory mechanism, multiple memory pools are managed using a linked list. When the memory pool needs to be recycled, call the corresponding interface to deinitialize the memory pool.

The multi-memory pool mechanism can be used to obtain the information and usage of each memory pool in the system and check the space allocation of memory pools. When the space of two memory pools overlaps, the initialization of the second memory pool fails and a message indicating space overlapping is displayed.

<a name="en-us_topic_0175230456_table17171164165"></a>
<table><thead align="left"><tr id="en-us_topic_0175230456_row31169589163114"><th class="cellrowborder" valign="top" width="21.97%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230456_p41708771163114"><a name="en-us_topic_0175230456_p41708771163114"></a><a name="en-us_topic_0175230456_p41708771163114"></a>Function</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230456_p22967294163114"><a name="en-us_topic_0175230456_p22967294163114"></a><a name="en-us_topic_0175230456_p22967294163114"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="58.28%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230456_p48411501163114"><a name="en-us_topic_0175230456_p48411501163114"></a><a name="en-us_topic_0175230456_p48411501163114"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230456_row29017497163114"><td class="cellrowborder" valign="top" width="21.97%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230456_p1607017163114"><a name="en-us_topic_0175230456_p1607017163114"></a><a name="en-us_topic_0175230456_p1607017163114"></a>Initialize memory</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230456_p63059525163114"><a name="en-us_topic_0175230456_p63059525163114"></a><a name="en-us_topic_0175230456_p63059525163114"></a>LOS_MemInit</p>
</td>
<td class="cellrowborder" valign="top" width="58.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230456_p7547926163114"><a name="en-us_topic_0175230456_p7547926163114"></a><a name="en-us_topic_0175230456_p7547926163114"></a>Initializes a dynamic memory pool of the required size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230456_row566665491512"><td class="cellrowborder" valign="top" width="21.97%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230456_p5634586991512"><a name="en-us_topic_0175230456_p5634586991512"></a><a name="en-us_topic_0175230456_p5634586991512"></a>Delete memory pools</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230456_p61270991512"><a name="en-us_topic_0175230456_p61270991512"></a><a name="en-us_topic_0175230456_p61270991512"></a>LOS_MemDeInit</p>
</td>
<td class="cellrowborder" valign="top" width="58.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230456_p4962945391512"><a name="en-us_topic_0175230456_p4962945391512"></a><a name="en-us_topic_0175230456_p4962945391512"></a>Deletes a memory pool. This function takes effect only when <strong id="en-us_topic_0175230456_b263663612225"><a name="en-us_topic_0175230456_b263663612225"></a><a name="en-us_topic_0175230456_b263663612225"></a>LOSCFG_MEM_MUL_POOL</strong> is enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230456_row131382579613"><td class="cellrowborder" valign="top" width="21.97%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230456_p2077919553615"><a name="en-us_topic_0175230456_p2077919553615"></a><a name="en-us_topic_0175230456_p2077919553615"></a>Display system memory pools</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230456_p9779255064"><a name="en-us_topic_0175230456_p9779255064"></a><a name="en-us_topic_0175230456_p9779255064"></a>LOS_MemPoolList</p>
</td>
<td class="cellrowborder" valign="top" width="58.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230456_p2077919551963"><a name="en-us_topic_0175230456_p2077919551963"></a><a name="en-us_topic_0175230456_p2077919551963"></a>Prints information about all initialized memory pools in the system, including the start addresses of the memory pools, size of the memory pools, total size of idle memory, total size of used memory, maximum size of idle memory blocks, number of idle memory blocks, and number of used memory blocks. This function takes effect only when <strong id="en-us_topic_0175230456_b0961025182315"><a name="en-us_topic_0175230456_b0961025182315"></a><a name="en-us_topic_0175230456_b0961025182315"></a>LOSCFG_MEM_MUL_POOL</strong> is enabled.</p>
</td>
</tr>
</tbody>
</table>

## How to Use<a name="en-us_topic_0175230456_section300741316138"></a>

1.  Run the  **make menuconfig**  command to enable the multi-memory pool mechanism.

    This function depends on  **LOSCFG\_MEM\_MUL\_POOL**. Before using this function, enable  **Enable Memory multi-pool control**.

    ```
    Debug  ---> Enable a Debug Version---> Enable MEM Debug---> Enable Memory multi-pool control
    ```

2.  Call the LOS\_MemInit API to initialize the memory pool and call the LOS\_MemDeInit API to deinitialize the memory pool when the memory pool needs to be recycled.
3.  Call the LOS\_MemInfoGet API to obtain the information and usage of the specified memory pool.
4.  Call the LOS\_MemPoolList API to obtain the information and usage of all memory pools in the system.

## Precautions<a name="en-us_topic_0175230456_section4175643316212"></a>

-   Before initializing a memory pool, ensure that the memory pool spaces are not overlapped. If the memory pool spaces are overlapped, the memory pool fails to be initialized.
-   By default, the malloc/free series APIs apply for and release memory from the OS memory pool. For other memory pool operations, the Huawei LiteOS memory APIs \(such as LOS\_MemAlloc\) must be called, and the malloc/free series APIs and related encapsulation interfaces cannot be called.
-   The LOS\_MemDeInit API must be called to deinitialize the memory pool for recycling \(ensure that all memory blocks in the memory pool have been released before the recycling\). Otherwise, the memory pool space fails to be initialized again, and the memory pool cannot be used again.
-   The memory pool size must be properly allocated based on site requirements.

## Programming Example<a name="en-us_topic_0175230456_section74785714338"></a>

```c
void test(void)
{
    UINT32 ret = 0;
    UINT32 size = 0x100000;

    VOID *poolAddr1 = LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
    ret = LOS_MemInit(poolAddr1, size);
    if (ret != 0) {
        PRINTK("LOS_MemInit failed\n");
        return;
    }

    VOID *poolAddr2 = LOS_MemAlloc(OS_SYS_MEM_ADDR, size);
    ret = LOS_MemInit(poolAddr2, size);
    if (ret != 0) {
        PRINTK("LOS_MemInit failed\n");
        return;
    }

    PRINTK("\n********step1 list the mem poll\n");
    LOS_MemPoolList();

    LOS_MemDeInit(poolAddr1);
    if (ret != 0) {
        PRINTK("LOS_MemDeInit failed\n");
        return;
    }

    PRINTK("\n********step2 list the mem poll\n");
    LOS_MemPoolList();

    LOS_MemDeInit(poolAddr2);
    if (ret != 0) {
        PRINTK("LOS_MemDeInit failed\n");
        return;
    } 

    PRINTK("\n********step3 list the mem poll\n");
    LOS_MemPoolList(); 
}
```

Logs:

```
********step1 list the mem poll
pool0 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8017b2c0         0x100000     0x2e1fc       0xd1d20      0xd1d20              0x2b               0x1            
pool1 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8027b2c0         0x7d84d40    0x7070c8      0x767db94    0x767db94            0x1026             0x1            
pool2 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8078244c         0x100000     0x10          0xfff0c      0xfff0c              0x1                0x1            
pool3 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8088245c         0x100000     0x10          0xfff0c      0xfff0c              0x1                0x1            

********step2 list the mem poll
pool0 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8017b2c0         0x100000     0x2e1fc       0xd1d20      0xd1d20              0x2b               0x1            
pool1 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8027b2c0         0x7d84d40    0x7070c8      0x767db94    0x767db94            0x1026             0x1            
pool2 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8088245c         0x100000     0x10          0xfff0c      0xfff0c              0x1                0x1            

********step3 list the mem poll
pool0 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8017b2c0         0x100000     0x2e1fc       0xd1d20      0xd1d20              0x2b               0x1            
pool1 :
pool addr          pool size    used size     free size    max free node size   used node num     free node num
---------------    --------     -------       --------     --------------       -------------      ------------
0x8027b2c0         0x7d84d40    0x7070c8      0x767db94    0x767db94            0x1026             0x1   
```


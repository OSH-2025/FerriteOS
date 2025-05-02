# Usage of Dying Gasp<a name="EN-US_TOPIC_0312244445"></a>

## Function Description<a name="en-us_topic_0175230516_section13598163943816"></a>

The dying gasp log storage hook function is used to register the read and write functions for log storage. For details about the API, see the API reference.

<a name="en-us_topic_0175230516_table65300298141415"></a>
<table><thead align="left"><tr id="en-us_topic_0175230516_row54518477141415"><th class="cellrowborder" valign="top" width="29.080000000000002%" id="mcps1.1.3.1.1"><p id="en-us_topic_0175230516_p5483720141415"><a name="en-us_topic_0175230516_p5483720141415"></a><a name="en-us_topic_0175230516_p5483720141415"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="70.92%" id="mcps1.1.3.1.2"><p id="en-us_topic_0175230516_p41528136141415"><a name="en-us_topic_0175230516_p41528136141415"></a><a name="en-us_topic_0175230516_p41528136141415"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230516_row8335828141415"><td class="cellrowborder" valign="top" width="29.080000000000002%" headers="mcps1.1.3.1.1 "><p id="en-us_topic_0175230516_p64757555141415"><a name="en-us_topic_0175230516_p64757555141415"></a><a name="en-us_topic_0175230516_p64757555141415"></a>LOS_ExcInfoRegHook</p>
</td>
<td class="cellrowborder" valign="top" width="70.92%" headers="mcps1.1.3.1.2 "><p id="en-us_topic_0175230516_p10870613141415"><a name="en-us_topic_0175230516_p10870613141415"></a><a name="en-us_topic_0175230516_p10870613141415"></a>Registers the hook function for reading and writing dying gasp logs, and sets the start address, size, and buffer for recording dying gasp logs.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>LOS\_ExcInfoRegHook has four parameters:
>-   **startAddr**: indicates the start address for storing dying gasp logs.
>-   **space**: indicates the size of the storage space for dying gasp logs.
>-   **buf**: indicates the buffer address of the dying gasp logs in the memory. The size of  **buffer**  is the same as that of  **space**.
>-   **LogReadWriteFunc**: indicates the hook function for reading and writing exception information. The input parameters of this function include  **startAddr**,  **space**,  **rwFlag**, and  **buf**.  **rwFlag**  is the Read/Write flag. The value  **0**  indicates write, and the value  **1**  indicates read. The other three input parameters are the same as those of LOS\_ExcInfoRegHook.

## How to Use<a name="en-us_topic_0175230516_section104865185441"></a>

1.  Run  **make menuconfig**  and choose  **Debug**  \>  **Enable a Debug Version**  \>  **Enable Shell**  \>  **Enable Shell excInfo**  to enable the dying gasp recording function. That is, configure  **LOSCFG\_SHELL\_EXCINFO\_DUMP=y**. This function is disabled by default.
2.  Register a read/write hook function.
    1.  Compile the hook function for reading and writing dying gasp logs. The following codes are as an example:

        ```c
        #include "los_base.h"
        #if defined(LOSCFG_SHELL_EXCINFO) && defined(LOSCFG_DRIVERS_MTD_SPI_NOR)
        #include "linux/mtd/mtd.h"
        #include "linux/module.h"
        #include "linux/mtd/mtd_list.h"
        #include "spinor.h"
        #endif
        #include "los_hwi.h"
        #ifdef LOSCFG_FS_VFS
        #include "fs/fs.h"
        #endif
        
        #ifdef LOSCFG_SHELL_EXCINFO_DUMP
        STATIC struct mtd_info *g_mtdSpinor = NULL;
        STATIC VOID OsSpiflashErase(UINT32 start, size_t size)
        {
            struct erase_info eraseInfo;
            (VOID)memset_s(&eraseInfo, sizeof(struct erase_info), 0, sizeof(struct erase_info));
            eraseInfo.mtd = g_mtdSpinor;
            eraseInfo.callback = NULL;
            eraseInfo.fail_addr = (UINT64)MTD_FAIL_ADDR_UNKNOWN;
            eraseInfo.addr = start;
            eraseInfo.len = size;
            eraseInfo.time = 1;
            eraseInfo.retries = 1;
            eraseInfo.dev = 0;
            eraseInfo.cell = 0;
            eraseInfo.priv = 0;
            eraseInfo.state = 0;
            eraseInfo.next = NULL;
            eraseInfo.scrub = 0;
            (VOID)g_mtdSpinor->erase(g_mtdSpinor, &eraseInfo);
        }
        STATIC INT32 OsWriteExcInfoToSpiFlash(UINT32 startAddr, UINT32 space, const CHAR *buf)
        {
            UINT32 outLen;
            OsSpiflashErase(startAddr, LOS_Align(space, g_mtdSpinor->erasesize));
            return g_mtdSpinor->write(g_mtdSpinor, (loff_t)startAddr, space, &outLen, buf);
        }
        STATIC INT32 OsReadExcInfoForSpiFlash(UINT32 startAddr, UINT32 space, CHAR *buf)
        {
            UINT32 outLen;
            return g_mtdSpinor->read(g_mtdSpinor, (loff_t)startAddr, space, &outLen, buf);
        }
        
        /* Be called in the exception. */
        VOID OsReadWriteExceptionInfo(UINT32 startAddr, UINT32 space, UINT32 flag, CHAR *buf)
        {
            if ((buf == NULL) || (space == 0)) {
                PRINT_ERR("buffer is null or space is zero\n");
                return;
            }
        
            g_mtdSpinor = get_mtd("spinor");
            if (g_mtdSpinor == NULL) {
                (VOID)spinor_init();
                g_mtdSpinor = get_mtd("spinor");
                if (g_mtdSpinor == NULL) {
                    PRINT_ERR("Init spinor is failed\n");
                    return;
                }
            }
        
            if (flag == 0) {
                if (OsWriteExcInfoToSpiFlash(startAddr, space, buf) != LOS_OK) {
                    PRINT_ERR("Exception information written to 0x%x flash failed\n", startAddr);
                }
        #ifndef LOSCFG_EXC_INTERACTION
                /*
                 * When system is in the exception interaction, this buf was free,
                 * but this feature is still running. This buffer may be used again
                 * without malloc.
                 * So, consider whether or not the "buf" is released according to actual use.
                 */
                free(buf);
        #endif
            } elseif (flag == 1) {
                if (OsReadExcInfoForSpiFlash(startAddr, space, buf) != LOS_OK) {
                    PRINT_ERR("Exception information read from 0x%x flash failed\n", startAddr);
                }
            } else {
                PRINT_ERR("flag is error\n");
            }
        }
        #endif
        #endif
        ```

    2.  Register the hook function in the initialization function such as app\_init. The following codes are as an example:

        ```c
        #ifdef LOSCFG_SHELL_EXCINFO_DUMP
        #define EXCINFO_RECORD_BUF_SIZE     (16 * 1024)   /* user-defined */
        #define EXCINFO_RECORD_ADDR         (0xffffffff)  /* The value is invalid and needs to be defined by users. */
            CHAR *buf = (CHAR *)malloc(EXCINFO_RECORD_BUF_SIZE);
            if (buf != NULL) {
                (VOID)memset_s(buf, EXCINFO_RECORD_BUF_SIZE, 0, EXCINFO_RECORD_BUF_SIZE);
                LOS_ExcInfoRegHook(EXCINFO_RECORD_ADDR, EXCINFO_RECORD_BUF_SIZE, buf, OsReadWriteExceptionInfo);
            } else {
                PRINTK("shell excinfo malloc failed!\n");
            }
        #ifdef LOSCFG_FS_VFS
            los_vfs_init();
        #endif
        #endif
        ```

        >![](public_sys-resources/icon-notice.gif) **NOTICE:** 
        >-   The OsReadWriteExceptionInfo function is a NOR flash read/write function. The EXCINFO\_RECORD\_BUF\_SIZE and EXCINFO\_RECORD\_ADDR macros need to be configured by users based on site requirements. It is recommended that the code be placed in front of the user service code.
        >-   Before data is written to the NOR flash, the block needs to be erased. The block size is 64 KB. Therefore, the 64 KB area behind the start address for the dying gasp storage is erased when the dying gasp information is written. You are advised not to save data in this area.


3.  View error information.

    After the system is reset and restarted, run the  **excInfo**  command in the shell window to print the recorded dying gasp logs.


## Precautions<a name="en-us_topic_0175230516_section7672188125113"></a>

You need to maintain the buffer for storing dying gasp information. When calling a registration function repeatedly, you need to release the buffer generated during the previous registration.


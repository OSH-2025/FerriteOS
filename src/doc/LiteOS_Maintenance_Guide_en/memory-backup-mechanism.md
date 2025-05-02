# Memory Backup Mechanism<a name="EN-US_TOPIC_0312244437"></a>

## Usage Scenarios<a name="en-us_topic_0175230432_section25700555173750"></a>

If an issue such as memory overwriting or releasing of wild pointer occurs, it is difficult to locate the illegitimate operation position of the memory by using the abnormal dump information.

## Function Description<a name="en-us_topic_0175230432_section46400139155028"></a>

Back up the control header information of the dynamic memory node: Back up the control header information of the current node in the control header of the previous memory node. When applying for and releasing memory, detect the control header information and backup information of the current node. When the node control header information is overwritten but the backup information is not overwritten, output the node control header backup information and the information of the previous memory node of the overwritten node, to further analyze whether the memory overwriting is caused by overflow. When the wild pointer is released, promptly output the prompt information and call stack information to quickly locate the position where the wild pointer is released.

## How to Use<a name="en-us_topic_0175230432_section300741316138"></a>

Run the  **make menuconfig**  command to enable the memory backup mechanism.

-   Currently, only the  **bestfit**  memory management algorithm supports this function. You need to enable  **LOSCFG\_KERNEL\_MEM\_BESTFIT**.

    ```
    Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
    ```

-   In addition, this function depends on  **LOSCFG\_MEM\_HEAD\_BACKUP**. You can enable the macro by enabling  **Enable Node Head Backup**.

    ```
    Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable Node Head Backup
    ```


## Precautions<a name="en-us_topic_0175230432_section4175643316212"></a>

After this function is enabled, the system memory usage increases \(Size of occupied memory = Number of nodes x Size of the node control header\) and the memory operation performance is affected. It is recommended that this function be enabled only for detecting issues. By default, this function is disabled.


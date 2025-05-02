# Memory Leakage Check<a name="EN-US_TOPIC_0312244440"></a>

## Usage Scenarios<a name="en-us_topic_0175230527_section20752180194244"></a>

Memory leakage occurs during service running if the service logic is complex or runs for a long time.

## Function Description<a name="en-us_topic_0175230527_section46400139155028"></a>

During memory application and release, the function call stack is recorded in the control header of the memory node. When a memory leakage occurs, the suspected memory leakage location can be located by analyzing the used node information.

## How to Use<a name="en-us_topic_0175230527_section300741316138"></a>

1.  Run the  **make menuconfig**  command to enable memory leakage check.
    -   Currently, only the  **bestfit**  memory management algorithm supports this function. You need to enable  **LOSCFG\_KERNEL\_MEM\_BESTFIT**.

        ```
        Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
        ```

    -   In addition, this function depends on  **LOSCFG\_MEM\_LEAKCHECK**. You can enable the macro by enabling  **Enable Function call stack of Mem operation recorded**.

        ```
        Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable Function call stack of Mem operation recorded
        ```

2.  Configure call stack backtracking information.

    -   **LOS\_OMIT\_LR\_CNT**: Layer without call stack backtracking. The default value is  **2**.
    -   **LOS\_RECORD\_LR\_CNT**: Number of call stack backtracking records. The default value is  **3**.

    By default, the system obtains the LR information at layers 0 to 4. The system ignores layers 0 and 1 \(the LR information at layers 0 and 1 of the node that calls the encapsulation interface is the same\), and records LR information at layers 2, 3, and 4.

    >![](public_sys-resources/icon-note.gif) **NOTE:** 
    >
    >The configurations of macro  **LOS\_OMIT\_LR\_CNT**  and  **LOS\_RECORD\_LR\_CNT**  are contained in the  **los\_memory.h**  file.

3.  Run the shell command  **memused**  to obtain used node data.

    After the system runs stably, if the number of used nodes keeps increasing, there is a very large probability that memory leaks. Compare and analyze the data, and check whether nodes with repeated LR information have memory leakage. The memory leakage can be backtracked based on the LR information.

    The log information is displayed as follows \(for details about the **memused** command, see  [memused](/shell/doc_en/memused.md)\):

    ```
    Huawei LiteOS # memused
    node         LR[0]       LR[1]       LR[2]
    0x802d7b34:  0x8006d86c  0x8011c604  0x8011c758
    0x802dab6c:  0x8006d16c  0x8006d8a0  0x8011c604
    ```


## Precautions<a name="en-us_topic_0175230527_section4175643316212"></a>

Enabling this function increases the system memory usage \(Memory occupied = Number of nodes x LOS\_RECORD\_LR\_CNT x Size of the pointer\) and has an impact on the system performance during memory operations. Therefore, you are advised to enable this function only for fault locating. By default, this function is disabled.


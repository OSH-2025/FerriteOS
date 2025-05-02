# Development Guidelines<a name="EN-US_TOPIC_0311018321"></a>

## Usage Scenarios<a name="en-us_topic_0175230600_section5224652316715"></a>

After a task is created, Huawei LiteOS Kernel can perform operations such as locking/unlocking task scheduling, suspending/resuming/delaying the task, and setting/obtaining the task priority.

## Functions<a name="en-us_topic_0175230600_section21906860141415"></a>

The task management module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230600_table65300298141415"></a>
<table><thead align="left"><tr id="en-us_topic_0175230600_row54518477141415"><th class="cellrowborder" valign="top" width="23.142314231423143%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230600_p53920492141415"><a name="en-us_topic_0175230600_p53920492141415"></a><a name="en-us_topic_0175230600_p53920492141415"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="25.452545254525454%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230600_p5483720141415"><a name="en-us_topic_0175230600_p5483720141415"></a><a name="en-us_topic_0175230600_p5483720141415"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="51.4051405140514%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230600_p41528136141415"><a name="en-us_topic_0175230600_p41528136141415"></a><a name="en-us_topic_0175230600_p41528136141415"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230600_row8335828141415"><td class="cellrowborder" rowspan="5" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p4113493141415"><a name="en-us_topic_0175230600_p4113493141415"></a><a name="en-us_topic_0175230600_p4113493141415"></a>Task creation and deletion</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p64757555141415"><a name="en-us_topic_0175230600_p64757555141415"></a><a name="en-us_topic_0175230600_p64757555141415"></a>LOS_TaskCreateOnly</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p10870613141415"><a name="en-us_topic_0175230600_p10870613141415"></a><a name="en-us_topic_0175230600_p10870613141415"></a>Creates a task and suspends the task without scheduling it. If scheduling is required, call LOS_TaskResume to set the task as Ready.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row15367626144438"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p4090907144438"><a name="en-us_topic_0175230600_p4090907144438"></a><a name="en-us_topic_0175230600_p4090907144438"></a>LOS_TaskCreate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p62928011144438"><a name="en-us_topic_0175230600_p62928011144438"></a><a name="en-us_topic_0175230600_p62928011144438"></a>Creates a task and sets it as Ready. If the Ready list contains no higher-priority tasks, this task will be executed.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row178341441114418"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p12834104194415"><a name="en-us_topic_0175230600_p12834104194415"></a><a name="en-us_topic_0175230600_p12834104194415"></a>LOS_TaskCreateOnlyStatic</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p9834164110446"><a name="en-us_topic_0175230600_p9834164110446"></a><a name="en-us_topic_0175230600_p9834164110446"></a>Creates a task, sets the task stack, and suspends the task without scheduling it. If scheduling is required, call LOS_TaskResume to set the task as Ready.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row7575193584216"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p1157543520427"><a name="en-us_topic_0175230600_p1157543520427"></a><a name="en-us_topic_0175230600_p1157543520427"></a>LOS_TaskCreateStatic</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p125751635184210"><a name="en-us_topic_0175230600_p125751635184210"></a><a name="en-us_topic_0175230600_p125751635184210"></a>Creates a task, sets the task stack, and sets the task as Ready. If the Ready list contains no higher-priority tasks, this task will be executed.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row193541212163511"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p5831495141415"><a name="en-us_topic_0175230600_p5831495141415"></a><a name="en-us_topic_0175230600_p5831495141415"></a>LOS_TaskDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p2589124141415"><a name="en-us_topic_0175230600_p2589124141415"></a><a name="en-us_topic_0175230600_p2589124141415"></a>Deletes a task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row53535882141415"><td class="cellrowborder" rowspan="4" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p1134097141415"><a name="en-us_topic_0175230600_p1134097141415"></a><a name="en-us_topic_0175230600_p1134097141415"></a>Task state control</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p24753017141415"><a name="en-us_topic_0175230600_p24753017141415"></a><a name="en-us_topic_0175230600_p24753017141415"></a>LOS_TaskResume</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p58837337141415"><a name="en-us_topic_0175230600_p58837337141415"></a><a name="en-us_topic_0175230600_p58837337141415"></a>Restores a task from suspended to ready.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row59773990141415"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p9855022141415"><a name="en-us_topic_0175230600_p9855022141415"></a><a name="en-us_topic_0175230600_p9855022141415"></a>LOS_TaskSuspend</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p60059331141415"><a name="en-us_topic_0175230600_p60059331141415"></a><a name="en-us_topic_0175230600_p60059331141415"></a>Suspends and switches a task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row3663069141415"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p28273200141415"><a name="en-us_topic_0175230600_p28273200141415"></a><a name="en-us_topic_0175230600_p28273200141415"></a>LOS_TaskDelay</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p8427843141415"><a name="en-us_topic_0175230600_p8427843141415"></a><a name="en-us_topic_0175230600_p8427843141415"></a>Delays a task and releases CPU resources. After the delay duration elapses, the task will become Ready again.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row8741724141415"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p36991026141415"><a name="en-us_topic_0175230600_p36991026141415"></a><a name="en-us_topic_0175230600_p36991026141415"></a>LOS_TaskYield</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p43483166141415"><a name="en-us_topic_0175230600_p43483166141415"></a><a name="en-us_topic_0175230600_p43483166141415"></a>Releases CPU resources and appends a task to a Ready list with the same priority as that task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row55804174141415"><td class="cellrowborder" rowspan="2" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p23844275141415"><a name="en-us_topic_0175230600_p23844275141415"></a><a name="en-us_topic_0175230600_p23844275141415"></a>Task scheduling control</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p1281258141415"><a name="en-us_topic_0175230600_p1281258141415"></a><a name="en-us_topic_0175230600_p1281258141415"></a>LOS_TaskLock</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p36673079141415"><a name="en-us_topic_0175230600_p36673079141415"></a><a name="en-us_topic_0175230600_p36673079141415"></a>Locks task scheduling, after which tasks can still be interrupted.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row61622261141415"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p25347244141415"><a name="en-us_topic_0175230600_p25347244141415"></a><a name="en-us_topic_0175230600_p25347244141415"></a>LOS_TaskUnlock</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p39860875141415"><a name="en-us_topic_0175230600_p39860875141415"></a><a name="en-us_topic_0175230600_p39860875141415"></a>Unlocks task scheduling.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row23203562141415"><td class="cellrowborder" rowspan="3" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p440345141415"><a name="en-us_topic_0175230600_p440345141415"></a><a name="en-us_topic_0175230600_p440345141415"></a>Task priority control</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p35667973141415"><a name="en-us_topic_0175230600_p35667973141415"></a><a name="en-us_topic_0175230600_p35667973141415"></a>LOS_CurTaskPriSet</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p3424661141415"><a name="en-us_topic_0175230600_p3424661141415"></a><a name="en-us_topic_0175230600_p3424661141415"></a>Sets the priority of the current task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row31647912144835"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p16395752144835"><a name="en-us_topic_0175230600_p16395752144835"></a><a name="en-us_topic_0175230600_p16395752144835"></a>LOS_TaskPriSet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p52987508144835"><a name="en-us_topic_0175230600_p52987508144835"></a><a name="en-us_topic_0175230600_p52987508144835"></a>Sets the priority of a particular task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row30821950141415"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p13550016141415"><a name="en-us_topic_0175230600_p13550016141415"></a><a name="en-us_topic_0175230600_p13550016141415"></a>LOS_TaskPriGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p23809498141415"><a name="en-us_topic_0175230600_p23809498141415"></a><a name="en-us_topic_0175230600_p23809498141415"></a>Obtains the priority of a particular task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row1421833119412"><td class="cellrowborder" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p12998122195310"><a name="en-us_topic_0175230600_p12998122195310"></a><a name="en-us_topic_0175230600_p12998122195310"></a>Task affinity setting</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p7999102185313"><a name="en-us_topic_0175230600_p7999102185313"></a><a name="en-us_topic_0175230600_p7999102185313"></a>LOS_TaskCpuAffiSet</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p1699910217539"><a name="en-us_topic_0175230600_p1699910217539"></a><a name="en-us_topic_0175230600_p1699910217539"></a>Sets the CPU collection of a particular task. (This function is available only in SMP mode.)</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row269683424112"><td class="cellrowborder" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p38501673543"><a name="en-us_topic_0175230600_p38501673543"></a><a name="en-us_topic_0175230600_p38501673543"></a>Task stack resource reclamation</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p885017115415"><a name="en-us_topic_0175230600_p885017115415"></a><a name="en-us_topic_0175230600_p885017115415"></a>LOS_TaskResRecycle</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p88505710544"><a name="en-us_topic_0175230600_p88505710544"></a><a name="en-us_topic_0175230600_p88505710544"></a>Reclaims all possible task stack resources.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row19937126145221"><td class="cellrowborder" rowspan="3" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p45216412145221"><a name="en-us_topic_0175230600_p45216412145221"></a><a name="en-us_topic_0175230600_p45216412145221"></a>Task information obtainment</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p38650789145221"><a name="en-us_topic_0175230600_p38650789145221"></a><a name="en-us_topic_0175230600_p38650789145221"></a>LOS_CurTaskIDGet</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p43706210145221"><a name="en-us_topic_0175230600_p43706210145221"></a><a name="en-us_topic_0175230600_p43706210145221"></a>Obtains the ID of the current task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row18034640145252"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p61024423145252"><a name="en-us_topic_0175230600_p61024423145252"></a><a name="en-us_topic_0175230600_p61024423145252"></a>LOS_TaskInfoGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p44031202145252"><a name="en-us_topic_0175230600_p44031202145252"></a><a name="en-us_topic_0175230600_p44031202145252"></a>Obtains information about a particular task, including its state, priority, total task stack size, SP, entrypoint function, and used task stack size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row164182610524"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p541836195218"><a name="en-us_topic_0175230600_p541836195218"></a><a name="en-us_topic_0175230600_p541836195218"></a>LOS_TaskCpuAffiGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p1841815685215"><a name="en-us_topic_0175230600_p1841815685215"></a><a name="en-us_topic_0175230600_p1841815685215"></a>Obtains the CPU collection of a particular task. (This function is available only in SMP mode.)</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row1897617132814"><td class="cellrowborder" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p16751115280"><a name="en-us_topic_0175230600_p16751115280"></a><a name="en-us_topic_0175230600_p16751115280"></a>Task information maintenance and test</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p1751519285"><a name="en-us_topic_0175230600_p1751519285"></a><a name="en-us_topic_0175230600_p1751519285"></a>LOS_TaskSwitchHookReg</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p77518118284"><a name="en-us_topic_0175230600_p77518118284"></a><a name="en-us_topic_0175230600_p77518118284"></a>Registers a hook function for switching task context. This hook function is called upon task context switching only when the <strong id="en-us_topic_0175230600_b62073161512"><a name="en-us_topic_0175230600_b62073161512"></a><a name="en-us_topic_0175230600_b62073161512"></a>LOSCFG_BASE_CORE_TSK_MONITOR</strong> macro has been enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row172231051637"><td class="cellrowborder" valign="top" width="23.142314231423143%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p22234512037"><a name="en-us_topic_0175230600_p22234512037"></a><a name="en-us_topic_0175230600_p22234512037"></a>Idle task callback</p>
</td>
<td class="cellrowborder" valign="top" width="25.452545254525454%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p12231051935"><a name="en-us_topic_0175230600_p12231051935"></a><a name="en-us_topic_0175230600_p12231051935"></a>LOS_IdleHandlerHookReg</p>
</td>
<td class="cellrowborder" valign="top" width="51.4051405140514%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p1722319518314"><a name="en-us_topic_0175230600_p1722319518314"></a><a name="en-us_topic_0175230600_p1722319518314"></a>Registers a hook function for an idle task. This function is called when the system is idle.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>-   You can run  **make menuconfig**  and choose  **Kernel**  \>  **Enable Kernel SMP**  to configure  **LOSCFG\_KERNEL\_SMP**  to enable multi-core mode. In the SMP submenu, you can set the number of cores, enable inter-core synchronization for tasks, and enable cross-core function calling.
>-   In SMP mode,  **usCpuAffiMask**  can be set during task creation to specify the CPU affinity of the task. This flag bit uses the "1 bit, 1 core" paradigm. For details, see the TSK\_INIT\_PARAM\_S structure.
>-   The task stack size of each task can be set during task creation. If it is set to 0, the default size \(**LOSCFG\_BASE\_CORE\_TSK\_DEFAULT\_STACK\_SIZE**\) will be used.

## Task State<a name="en-us_topic_0175230600_section31234536144156"></a>

In Huawei LiteOS, most task states are defined by the kernel. Only Detached state can be defined by users. Users need to define Detached state during task creation.

<a name="en-us_topic_0175230600_table40176671144552"></a>
<table><thead align="left"><tr id="en-us_topic_0175230600_row45035287144552"><th class="cellrowborder" valign="top" width="31.410000000000004%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230600_p63302382144552"><a name="en-us_topic_0175230600_p63302382144552"></a><a name="en-us_topic_0175230600_p63302382144552"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="27.540000000000003%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230600_p27219308144552"><a name="en-us_topic_0175230600_p27219308144552"></a><a name="en-us_topic_0175230600_p27219308144552"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="41.050000000000004%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230600_p57280343144552"><a name="en-us_topic_0175230600_p57280343144552"></a><a name="en-us_topic_0175230600_p57280343144552"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230600_row6693266144552"><td class="cellrowborder" valign="top" width="31.410000000000004%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230600_p25321182144552"><a name="en-us_topic_0175230600_p25321182144552"></a><a name="en-us_topic_0175230600_p25321182144552"></a>LOS_TASK_STATUS_DETACHED</p>
</td>
<td class="cellrowborder" valign="top" width="27.540000000000003%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230600_p37749887144552"><a name="en-us_topic_0175230600_p37749887144552"></a><a name="en-us_topic_0175230600_p37749887144552"></a>0x0100</p>
</td>
<td class="cellrowborder" valign="top" width="41.050000000000004%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230600_p37842045144552"><a name="en-us_topic_0175230600_p37842045144552"></a><a name="en-us_topic_0175230600_p37842045144552"></a>The task is detached.</p>
</td>
</tr>
</tbody>
</table>

When calling the task creation API, you can set the  **uwResved**  field of  **TSK\_INIT\_PARAM\_S**  to  **LOS\_TASK\_STATUS\_DETACHED**  \(indicating the Detached state\). Detached tasks will be automatically deleted after being executed.

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>The Detached status is affected by the  **LOSCFG\_COMPAT\_POSIX**  switch.
>-   If  **LOSCFG\_COMPAT\_POSIX**  is enabled, the task can be automatically deleted only when the task status is set to  **LOS\_TASK\_STATUS\_DETACHED**. Otherwise, the task is not automatically deleted when it is complete.
>-   If  **LOSCFG\_COMPAT\_POSIX**  is disabled, the task is automatically deleted when the task is complete, regardless of whether the  **uwResved**  field of  **TSK\_INIT\_PARAM\_S**  is set to  **LOS\_TASK\_STATUS\_DETACHED**.

## Task Error Codes<a name="en-us_topic_0175230600_section868101994847"></a>

An error code is returned when a task fails to be created, deleted, suspended, resumed, or delayed. The error code usually suggests the possible cause of the failure.

<a name="en-us_topic_0175230600_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230600_row2267197395642"><th class="cellrowborder" valign="top" width="5.76%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230600_p1908783195642"><a name="en-us_topic_0175230600_p1908783195642"></a><a name="en-us_topic_0175230600_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="21.990000000000002%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230600_p261046995642"><a name="en-us_topic_0175230600_p261046995642"></a><a name="en-us_topic_0175230600_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="11.62%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230600_p1012144095642"><a name="en-us_topic_0175230600_p1012144095642"></a><a name="en-us_topic_0175230600_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="29.01%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230600_p1453028795642"><a name="en-us_topic_0175230600_p1453028795642"></a><a name="en-us_topic_0175230600_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="31.619999999999997%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230600_p2753561710026"><a name="en-us_topic_0175230600_p2753561710026"></a><a name="en-us_topic_0175230600_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230600_row6366372295642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p5648782795642"><a name="en-us_topic_0175230600_p5648782795642"></a><a name="en-us_topic_0175230600_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p1211123695642"><a name="en-us_topic_0175230600_p1211123695642"></a><a name="en-us_topic_0175230600_p1211123695642"></a>LOS_ERRNO_TSK_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p4148605495642"><a name="en-us_topic_0175230600_p4148605495642"></a><a name="en-us_topic_0175230600_p4148605495642"></a>0x03000200</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p492720095642"><a name="en-us_topic_0175230600_p492720095642"></a><a name="en-us_topic_0175230600_p492720095642"></a>Memory space is insufficient.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p63511053020"><a name="en-us_topic_0175230600_p63511053020"></a><a name="en-us_topic_0175230600_p63511053020"></a>Expand the dynamic memory space in either of the following ways:</p>
<a name="en-us_topic_0175230600_ul8816111853016"></a><a name="en-us_topic_0175230600_ul8816111853016"></a><ul id="en-us_topic_0175230600_ul8816111853016"><li>Set a larger system dynamic memory pool. The configuration item is <strong id="en-us_topic_0175230600_b139298439268"><a name="en-us_topic_0175230600_b139298439268"></a><a name="en-us_topic_0175230600_b139298439268"></a>OS_SYS_MEM_SIZE</strong>.</li><li>Release a part of the dynamic memory.</li></ul>
<p id="en-us_topic_0175230600_p54251328201810"><a name="en-us_topic_0175230600_p54251328201810"></a><a name="en-us_topic_0175230600_p54251328201810"></a>If the error occurs in task initialization during LiteOS startup, reduce the maximum number of tasks supported by the system to solve the problem. If the error occurs during task creation, reduce the size of the task stack to solve the problem.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row4434480695642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3515954495642"><a name="en-us_topic_0175230600_p3515954495642"></a><a name="en-us_topic_0175230600_p3515954495642"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p2935083995642"><a name="en-us_topic_0175230600_p2935083995642"></a><a name="en-us_topic_0175230600_p2935083995642"></a>LOS_ERRNO_TSK_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p2860775495642"><a name="en-us_topic_0175230600_p2860775495642"></a><a name="en-us_topic_0175230600_p2860775495642"></a>0x02000201</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p4450954181910"><a name="en-us_topic_0175230600_p4450954181910"></a><a name="en-us_topic_0175230600_p4450954181910"></a>The <strong id="en-us_topic_0175230600_b8276641163117"><a name="en-us_topic_0175230600_b8276641163117"></a><a name="en-us_topic_0175230600_b8276641163117"></a>initParam</strong> parameter transferred to the task creation API or a parameter transferred to the task information obtainment API is a null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p412423410026"><a name="en-us_topic_0175230600_p412423410026"></a><a name="en-us_topic_0175230600_p412423410026"></a>Ensure that the input parameter is not a null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row5130526095642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p6208537795642"><a name="en-us_topic_0175230600_p6208537795642"></a><a name="en-us_topic_0175230600_p6208537795642"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p6285965595642"><a name="en-us_topic_0175230600_p6285965595642"></a><a name="en-us_topic_0175230600_p6285965595642"></a>LOS_ERRNO_TSK_STKSZ_NOT_ALIGN</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p5846730195642"><a name="en-us_topic_0175230600_p5846730195642"></a><a name="en-us_topic_0175230600_p5846730195642"></a>0x02000202</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p3823098095642"><a name="en-us_topic_0175230600_p3823098095642"></a><a name="en-us_topic_0175230600_p3823098095642"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p6562756010026"><a name="en-us_topic_0175230600_p6562756010026"></a><a name="en-us_topic_0175230600_p6562756010026"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row853450895642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p2020651095642"><a name="en-us_topic_0175230600_p2020651095642"></a><a name="en-us_topic_0175230600_p2020651095642"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p2611462995642"><a name="en-us_topic_0175230600_p2611462995642"></a><a name="en-us_topic_0175230600_p2611462995642"></a>LOS_ERRNO_TSK_PRIOR_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p3491023395642"><a name="en-us_topic_0175230600_p3491023395642"></a><a name="en-us_topic_0175230600_p3491023395642"></a>0x02000203</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p915662595642"><a name="en-us_topic_0175230600_p915662595642"></a><a name="en-us_topic_0175230600_p915662595642"></a>An incorrect priority is used during task creation or priority setting.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1423211810026"><a name="en-us_topic_0175230600_p1423211810026"></a><a name="en-us_topic_0175230600_p1423211810026"></a>Check that the task priority is within the range of 0 to 31.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row1530076695642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3140256895642"><a name="en-us_topic_0175230600_p3140256895642"></a><a name="en-us_topic_0175230600_p3140256895642"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p6058010795642"><a name="en-us_topic_0175230600_p6058010795642"></a><a name="en-us_topic_0175230600_p6058010795642"></a>LOS_ERRNO_TSK_ENTRY_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p804161495642"><a name="en-us_topic_0175230600_p804161495642"></a><a name="en-us_topic_0175230600_p804161495642"></a>0x02000204</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_a1114d5c28c884640abc27a8fdc3eb618"><a name="en-us_topic_0175230600_a1114d5c28c884640abc27a8fdc3eb618"></a><a name="en-us_topic_0175230600_a1114d5c28c884640abc27a8fdc3eb618"></a>The task entrypoint function set during task creation is a null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1195087610026"><a name="en-us_topic_0175230600_p1195087610026"></a><a name="en-us_topic_0175230600_p1195087610026"></a>Specify the task entrypoint function.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row2386554195642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p5406070595642"><a name="en-us_topic_0175230600_p5406070595642"></a><a name="en-us_topic_0175230600_p5406070595642"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p1684095295642"><a name="en-us_topic_0175230600_p1684095295642"></a><a name="en-us_topic_0175230600_p1684095295642"></a>LOS_ERRNO_TSK_NAME_EMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p2193989295642"><a name="en-us_topic_0175230600_p2193989295642"></a><a name="en-us_topic_0175230600_p2193989295642"></a>0x02000205</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p3230086195642"><a name="en-us_topic_0175230600_p3230086195642"></a><a name="en-us_topic_0175230600_p3230086195642"></a>The task name set during task creation is a null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p2849689910026"><a name="en-us_topic_0175230600_p2849689910026"></a><a name="en-us_topic_0175230600_p2849689910026"></a>Specify the task name.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row2227229895642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p5922568095642"><a name="en-us_topic_0175230600_p5922568095642"></a><a name="en-us_topic_0175230600_p5922568095642"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p3255077595642"><a name="en-us_topic_0175230600_p3255077595642"></a><a name="en-us_topic_0175230600_p3255077595642"></a>LOS_ERRNO_TSK_STKSZ_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1936711095642"><a name="en-us_topic_0175230600_p1936711095642"></a><a name="en-us_topic_0175230600_p1936711095642"></a>0x02000206</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p2523210295642"><a name="en-us_topic_0175230600_p2523210295642"></a><a name="en-us_topic_0175230600_p2523210295642"></a>A small stack size is set during task creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p62917066101048"><a name="en-us_topic_0175230600_p62917066101048"></a><a name="en-us_topic_0175230600_p62917066101048"></a>Increase the size of the task stack to the minimum limit (<strong id="en-us_topic_0175230600_b8946132943615"><a name="en-us_topic_0175230600_b8946132943615"></a><a name="en-us_topic_0175230600_b8946132943615"></a>LOS_TASK_MIN_STACK_SIZE</strong>) or above.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row43458669101114"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p30491270101114"><a name="en-us_topic_0175230600_p30491270101114"></a><a name="en-us_topic_0175230600_p30491270101114"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p53873767101114"><a name="en-us_topic_0175230600_p53873767101114"></a><a name="en-us_topic_0175230600_p53873767101114"></a>LOS_ERRNO_TSK_ID_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1699001101114"><a name="en-us_topic_0175230600_p1699001101114"></a><a name="en-us_topic_0175230600_p1699001101114"></a>0x02000207</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p3401379101114"><a name="en-us_topic_0175230600_p3401379101114"></a><a name="en-us_topic_0175230600_p3401379101114"></a>The task ID is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p7076302101114"><a name="en-us_topic_0175230600_p7076302101114"></a><a name="en-us_topic_0175230600_p7076302101114"></a>Check the task ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row12143904101125"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p44132204101125"><a name="en-us_topic_0175230600_p44132204101125"></a><a name="en-us_topic_0175230600_p44132204101125"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p17938739101125"><a name="en-us_topic_0175230600_p17938739101125"></a><a name="en-us_topic_0175230600_p17938739101125"></a>LOS_ERRNO_TSK_ALREADY_SUSPENDED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p43751775101125"><a name="en-us_topic_0175230600_p43751775101125"></a><a name="en-us_topic_0175230600_p43751775101125"></a>0x02000208</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p54232920101125"><a name="en-us_topic_0175230600_p54232920101125"></a><a name="en-us_topic_0175230600_p54232920101125"></a>The task to be suspended is already suspended.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p30790405101125"><a name="en-us_topic_0175230600_p30790405101125"></a><a name="en-us_topic_0175230600_p30790405101125"></a>Suspend the task after it is resumed.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row44185939101121"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p22291345101121"><a name="en-us_topic_0175230600_p22291345101121"></a><a name="en-us_topic_0175230600_p22291345101121"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p60768531101121"><a name="en-us_topic_0175230600_p60768531101121"></a><a name="en-us_topic_0175230600_p60768531101121"></a>LOS_ERRNO_TSK_NOT_SUSPENDED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p23304002101121"><a name="en-us_topic_0175230600_p23304002101121"></a><a name="en-us_topic_0175230600_p23304002101121"></a>0x02000209</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p8576005101121"><a name="en-us_topic_0175230600_p8576005101121"></a><a name="en-us_topic_0175230600_p8576005101121"></a>The task to be resumed is not suspended.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p23567813101121"><a name="en-us_topic_0175230600_p23567813101121"></a><a name="en-us_topic_0175230600_p23567813101121"></a>Suspend the task and then resume it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row25611405101131"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p61257961101131"><a name="en-us_topic_0175230600_p61257961101131"></a><a name="en-us_topic_0175230600_p61257961101131"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p62947776101131"><a name="en-us_topic_0175230600_p62947776101131"></a><a name="en-us_topic_0175230600_p62947776101131"></a>LOS_ERRNO_TSK_NOT_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p65605116101131"><a name="en-us_topic_0175230600_p65605116101131"></a><a name="en-us_topic_0175230600_p65605116101131"></a>0x0200020a</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p12414213101131"><a name="en-us_topic_0175230600_p12414213101131"></a><a name="en-us_topic_0175230600_p12414213101131"></a>The task has not been created.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p66027202101131"><a name="en-us_topic_0175230600_p66027202101131"></a><a name="en-us_topic_0175230600_p66027202101131"></a>Create the task. This error may occur during the following operations:</p>
<a name="en-us_topic_0175230600_ul17581554133315"></a><a name="en-us_topic_0175230600_ul17581554133315"></a><ul id="en-us_topic_0175230600_ul17581554133315"><li>Deleting a task</li><li>Resuming/suspending a task</li><li>Setting the priority of a particular task</li><li>Obtaining information about a particular task</li><li>Setting the CPU collection of a particular task</li></ul>
</td>
</tr>
<tr id="en-us_topic_0175230600_row61184987101143"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p57036951101143"><a name="en-us_topic_0175230600_p57036951101143"></a><a name="en-us_topic_0175230600_p57036951101143"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p56590286101143"><a name="en-us_topic_0175230600_p56590286101143"></a><a name="en-us_topic_0175230600_p56590286101143"></a>LOS_ERRNO_TSK_DELETE_LOCKED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p20410474101143"><a name="en-us_topic_0175230600_p20410474101143"></a><a name="en-us_topic_0175230600_p20410474101143"></a>0x0300020b</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p42635731101143"><a name="en-us_topic_0175230600_p42635731101143"></a><a name="en-us_topic_0175230600_p42635731101143"></a>The task to be deleted is locked.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p19608837101953"><a name="en-us_topic_0175230600_p19608837101953"></a><a name="en-us_topic_0175230600_p19608837101953"></a>Unlock the task and then delete it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row55331775101146"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p52688791101146"><a name="en-us_topic_0175230600_p52688791101146"></a><a name="en-us_topic_0175230600_p52688791101146"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p39933692101146"><a name="en-us_topic_0175230600_p39933692101146"></a><a name="en-us_topic_0175230600_p39933692101146"></a>LOS_ERRNO_TSK_MSG_NONZERO</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p13403605101146"><a name="en-us_topic_0175230600_p13403605101146"></a><a name="en-us_topic_0175230600_p13403605101146"></a>0x0200020c</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p11950219101146"><a name="en-us_topic_0175230600_p11950219101146"></a><a name="en-us_topic_0175230600_p11950219101146"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p28443653101146"><a name="en-us_topic_0175230600_p28443653101146"></a><a name="en-us_topic_0175230600_p28443653101146"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row2857798210120"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3311521410120"><a name="en-us_topic_0175230600_p3311521410120"></a><a name="en-us_topic_0175230600_p3311521410120"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p6508665810120"><a name="en-us_topic_0175230600_p6508665810120"></a><a name="en-us_topic_0175230600_p6508665810120"></a>LOS_ERRNO_TSK_DELAY_IN_INT</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p3752793910120"><a name="en-us_topic_0175230600_p3752793910120"></a><a name="en-us_topic_0175230600_p3752793910120"></a>0x0300020d</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1986418410120"><a name="en-us_topic_0175230600_p1986418410120"></a><a name="en-us_topic_0175230600_p1986418410120"></a>The task to be delayed is under an interrupt.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p6549504910120"><a name="en-us_topic_0175230600_p6549504910120"></a><a name="en-us_topic_0175230600_p6549504910120"></a>Delay the task after it exits the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row4447897810129"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p4602745410129"><a name="en-us_topic_0175230600_p4602745410129"></a><a name="en-us_topic_0175230600_p4602745410129"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p3723625410129"><a name="en-us_topic_0175230600_p3723625410129"></a><a name="en-us_topic_0175230600_p3723625410129"></a>LOS_ERRNO_TSK_DELAY_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p6334657010129"><a name="en-us_topic_0175230600_p6334657010129"></a><a name="en-us_topic_0175230600_p6334657010129"></a>0x0200020e</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p3079857110129"><a name="en-us_topic_0175230600_p3079857110129"></a><a name="en-us_topic_0175230600_p3079857110129"></a>The task to be delayed is locked.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1165636110129"><a name="en-us_topic_0175230600_p1165636110129"></a><a name="en-us_topic_0175230600_p1165636110129"></a>Unlock the task and then delay it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row6331348110124"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p2811835210124"><a name="en-us_topic_0175230600_p2811835210124"></a><a name="en-us_topic_0175230600_p2811835210124"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p1648117254216"><a name="en-us_topic_0175230600_p1648117254216"></a><a name="en-us_topic_0175230600_p1648117254216"></a>LOS_ERRNO_TSK_YIELD_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p224588410124"><a name="en-us_topic_0175230600_p224588410124"></a><a name="en-us_topic_0175230600_p224588410124"></a>0x0200020f</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p568818398214"><a name="en-us_topic_0175230600_p568818398214"></a><a name="en-us_topic_0175230600_p568818398214"></a>The task to be yielded is locked.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p3840802610124"><a name="en-us_topic_0175230600_p3840802610124"></a><a name="en-us_topic_0175230600_p3840802610124"></a>Unlock the task and then yield it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row4160002610127"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p1415891810127"><a name="en-us_topic_0175230600_p1415891810127"></a><a name="en-us_topic_0175230600_p1415891810127"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p602173510127"><a name="en-us_topic_0175230600_p602173510127"></a><a name="en-us_topic_0175230600_p602173510127"></a>LOS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1799856510127"><a name="en-us_topic_0175230600_p1799856510127"></a><a name="en-us_topic_0175230600_p1799856510127"></a>0x02000210</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p4859767110127"><a name="en-us_topic_0175230600_p4859767110127"></a><a name="en-us_topic_0175230600_p4859767110127"></a>No task is found in the Ready list with the same priority as the task to be yielded.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p4409730210127"><a name="en-us_topic_0175230600_p4409730210127"></a><a name="en-us_topic_0175230600_p4409730210127"></a>Add tasks with the same priority as the current task.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row44067988101158"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p12737284101158"><a name="en-us_topic_0175230600_p12737284101158"></a><a name="en-us_topic_0175230600_p12737284101158"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p25087064101158"><a name="en-us_topic_0175230600_p25087064101158"></a><a name="en-us_topic_0175230600_p25087064101158"></a>LOS_ERRNO_TSK_TCB_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p18786297101158"><a name="en-us_topic_0175230600_p18786297101158"></a><a name="en-us_topic_0175230600_p18786297101158"></a>0x02000211</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p45295054101158"><a name="en-us_topic_0175230600_p45295054101158"></a><a name="en-us_topic_0175230600_p45295054101158"></a>No idle TCB is available during task creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p45020735101158"><a name="en-us_topic_0175230600_p45020735101158"></a><a name="en-us_topic_0175230600_p45020735101158"></a>Call the LOS_TaskResRecyle API to reclaim idle TCBs. If the failure persists, add TCBs in the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row13287662101156"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p2558809101156"><a name="en-us_topic_0175230600_p2558809101156"></a><a name="en-us_topic_0175230600_p2558809101156"></a>19</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p5936981101156"><a name="en-us_topic_0175230600_p5936981101156"></a><a name="en-us_topic_0175230600_p5936981101156"></a>LOS_ERRNO_TSK_HOOK_NOT_MATCH</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p11133449101156"><a name="en-us_topic_0175230600_p11133449101156"></a><a name="en-us_topic_0175230600_p11133449101156"></a>0x02000212</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p29394206101156"><a name="en-us_topic_0175230600_p29394206101156"></a><a name="en-us_topic_0175230600_p29394206101156"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p32120508101156"><a name="en-us_topic_0175230600_p32120508101156"></a><a name="en-us_topic_0175230600_p32120508101156"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row62576359101154"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p35520303101154"><a name="en-us_topic_0175230600_p35520303101154"></a><a name="en-us_topic_0175230600_p35520303101154"></a>20</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p58572271101154"><a name="en-us_topic_0175230600_p58572271101154"></a><a name="en-us_topic_0175230600_p58572271101154"></a>LOS_ERRNO_TSK_HOOK_IS_FULL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p46733520101154"><a name="en-us_topic_0175230600_p46733520101154"></a><a name="en-us_topic_0175230600_p46733520101154"></a>0x02000213</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p27318777101154"><a name="en-us_topic_0175230600_p27318777101154"></a><a name="en-us_topic_0175230600_p27318777101154"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p65337363101154"><a name="en-us_topic_0175230600_p65337363101154"></a><a name="en-us_topic_0175230600_p65337363101154"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row38567820101152"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p36985730101152"><a name="en-us_topic_0175230600_p36985730101152"></a><a name="en-us_topic_0175230600_p36985730101152"></a>21</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p43054160101152"><a name="en-us_topic_0175230600_p43054160101152"></a><a name="en-us_topic_0175230600_p43054160101152"></a>LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p64834975101152"><a name="en-us_topic_0175230600_p64834975101152"></a><a name="en-us_topic_0175230600_p64834975101152"></a>0x02000214</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p17141655101152"><a name="en-us_topic_0175230600_p17141655101152"></a><a name="en-us_topic_0175230600_p17141655101152"></a>It is forbidden to delete, suspend, or delay system-level tasks (such as idle tasks and software timer tasks), or modify the priorities of system-level tasks.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p46296799101152"><a name="en-us_topic_0175230600_p46296799101152"></a><a name="en-us_topic_0175230600_p46296799101152"></a>Check the task ID and do not operate system-level tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row7233065101149"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p49007378101149"><a name="en-us_topic_0175230600_p49007378101149"></a><a name="en-us_topic_0175230600_p49007378101149"></a>22</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p10174680101149"><a name="en-us_topic_0175230600_p10174680101149"></a><a name="en-us_topic_0175230600_p10174680101149"></a>LOS_ERRNO_TSK_SUSPEND_LOCKED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p18842722101149"><a name="en-us_topic_0175230600_p18842722101149"></a><a name="en-us_topic_0175230600_p18842722101149"></a>0x03000215</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p49865496101149"><a name="en-us_topic_0175230600_p49865496101149"></a><a name="en-us_topic_0175230600_p49865496101149"></a>The task to be suspended is locked.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p12573412101149"><a name="en-us_topic_0175230600_p12573412101149"></a><a name="en-us_topic_0175230600_p12573412101149"></a>Unlock the task and then suspend it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row33009169101238"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p56496996101238"><a name="en-us_topic_0175230600_p56496996101238"></a><a name="en-us_topic_0175230600_p56496996101238"></a>23</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p12854001101238"><a name="en-us_topic_0175230600_p12854001101238"></a><a name="en-us_topic_0175230600_p12854001101238"></a>LOS_ERRNO_TSK_FREE_STACK_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p34541150101238"><a name="en-us_topic_0175230600_p34541150101238"></a><a name="en-us_topic_0175230600_p34541150101238"></a>0x02000217</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p64962041101238"><a name="en-us_topic_0175230600_p64962041101238"></a><a name="en-us_topic_0175230600_p64962041101238"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1956611586403"><a name="en-us_topic_0175230600_p1956611586403"></a><a name="en-us_topic_0175230600_p1956611586403"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row49491606101241"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p49397169101241"><a name="en-us_topic_0175230600_p49397169101241"></a><a name="en-us_topic_0175230600_p49397169101241"></a>24</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p41747765101241"><a name="en-us_topic_0175230600_p41747765101241"></a><a name="en-us_topic_0175230600_p41747765101241"></a>LOS_ERRNO_TSK_STKAREA_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p26125768101241"><a name="en-us_topic_0175230600_p26125768101241"></a><a name="en-us_topic_0175230600_p26125768101241"></a>0x02000218</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p15131458101241"><a name="en-us_topic_0175230600_p15131458101241"></a><a name="en-us_topic_0175230600_p15131458101241"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p456611584402"><a name="en-us_topic_0175230600_p456611584402"></a><a name="en-us_topic_0175230600_p456611584402"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row22231064101243"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p55885775101243"><a name="en-us_topic_0175230600_p55885775101243"></a><a name="en-us_topic_0175230600_p55885775101243"></a>25</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p30453955101243"><a name="en-us_topic_0175230600_p30453955101243"></a><a name="en-us_topic_0175230600_p30453955101243"></a>LOS_ERRNO_TSK_ACTIVE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p50851291101243"><a name="en-us_topic_0175230600_p50851291101243"></a><a name="en-us_topic_0175230600_p50851291101243"></a>0x03000219</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p37161625101243"><a name="en-us_topic_0175230600_p37161625101243"></a><a name="en-us_topic_0175230600_p37161625101243"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p8565165884010"><a name="en-us_topic_0175230600_p8565165884010"></a><a name="en-us_topic_0175230600_p8565165884010"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row17237398101245"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p54051959101245"><a name="en-us_topic_0175230600_p54051959101245"></a><a name="en-us_topic_0175230600_p54051959101245"></a>26</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p16132553101245"><a name="en-us_topic_0175230600_p16132553101245"></a><a name="en-us_topic_0175230600_p16132553101245"></a>LOS_ERRNO_TSK_CONFIG_TOO_MANY</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p31668403101245"><a name="en-us_topic_0175230600_p31668403101245"></a><a name="en-us_topic_0175230600_p31668403101245"></a>0x0200021a</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p7353970101245"><a name="en-us_topic_0175230600_p7353970101245"></a><a name="en-us_topic_0175230600_p7353970101245"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p756455813405"><a name="en-us_topic_0175230600_p756455813405"></a><a name="en-us_topic_0175230600_p756455813405"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row57562865101249"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p32080501101249"><a name="en-us_topic_0175230600_p32080501101249"></a><a name="en-us_topic_0175230600_p32080501101249"></a>27</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p48383752101249"><a name="en-us_topic_0175230600_p48383752101249"></a><a name="en-us_topic_0175230600_p48383752101249"></a>LOS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p26769877101249"><a name="en-us_topic_0175230600_p26769877101249"></a><a name="en-us_topic_0175230600_p26769877101249"></a>0x0200021b</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p13268279101249"><a name="en-us_topic_0175230600_p13268279101249"></a><a name="en-us_topic_0175230600_p13268279101249"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1056365884019"><a name="en-us_topic_0175230600_p1056365884019"></a><a name="en-us_topic_0175230600_p1056365884019"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row3107909810130"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3437898210130"><a name="en-us_topic_0175230600_p3437898210130"></a><a name="en-us_topic_0175230600_p3437898210130"></a>28</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p3323418310130"><a name="en-us_topic_0175230600_p3323418310130"></a><a name="en-us_topic_0175230600_p3323418310130"></a>LOS_ERRNO_TSK_MSG_Q_TOO_MANY</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p761434110130"><a name="en-us_topic_0175230600_p761434110130"></a><a name="en-us_topic_0175230600_p761434110130"></a>0x0200021d</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p2870059810130"><a name="en-us_topic_0175230600_p2870059810130"></a><a name="en-us_topic_0175230600_p2870059810130"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p16542125812401"><a name="en-us_topic_0175230600_p16542125812401"></a><a name="en-us_topic_0175230600_p16542125812401"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row6008174710133"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3478334510133"><a name="en-us_topic_0175230600_p3478334510133"></a><a name="en-us_topic_0175230600_p3478334510133"></a>29</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p6598760010133"><a name="en-us_topic_0175230600_p6598760010133"></a><a name="en-us_topic_0175230600_p6598760010133"></a>LOS_ERRNO_TSK_CP_SAVE_AREA_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p4339541610133"><a name="en-us_topic_0175230600_p4339541610133"></a><a name="en-us_topic_0175230600_p4339541610133"></a>0x0200021e</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p4152524810133"><a name="en-us_topic_0175230600_p4152524810133"></a><a name="en-us_topic_0175230600_p4152524810133"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p17228144204117"><a name="en-us_topic_0175230600_p17228144204117"></a><a name="en-us_topic_0175230600_p17228144204117"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row5260030510136"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3276627310136"><a name="en-us_topic_0175230600_p3276627310136"></a><a name="en-us_topic_0175230600_p3276627310136"></a>30</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p3682242910136"><a name="en-us_topic_0175230600_p3682242910136"></a><a name="en-us_topic_0175230600_p3682242910136"></a>LOS_ERRNO_TSK_SELF_DELETE_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p2982679810136"><a name="en-us_topic_0175230600_p2982679810136"></a><a name="en-us_topic_0175230600_p2982679810136"></a>0x0200021f</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p418062210136"><a name="en-us_topic_0175230600_p418062210136"></a><a name="en-us_topic_0175230600_p418062210136"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p52091640411"><a name="en-us_topic_0175230600_p52091640411"></a><a name="en-us_topic_0175230600_p52091640411"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row5615305101320"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p52186542101320"><a name="en-us_topic_0175230600_p52186542101320"></a><a name="en-us_topic_0175230600_p52186542101320"></a>31</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p66360388101320"><a name="en-us_topic_0175230600_p66360388101320"></a><a name="en-us_topic_0175230600_p66360388101320"></a>LOS_ERRNO_TSK_STKSZ_TOO_LARGE</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p6482354101320"><a name="en-us_topic_0175230600_p6482354101320"></a><a name="en-us_topic_0175230600_p6482354101320"></a>0x02000220</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p55308705101320"><a name="en-us_topic_0175230600_p55308705101320"></a><a name="en-us_topic_0175230600_p55308705101320"></a>A large stack size is set during task creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p50820153101320"><a name="en-us_topic_0175230600_p50820153101320"></a><a name="en-us_topic_0175230600_p50820153101320"></a>Reduce the size of the task stack.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row52184175143246"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p66168651143246"><a name="en-us_topic_0175230600_p66168651143246"></a><a name="en-us_topic_0175230600_p66168651143246"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p58060511143246"><a name="en-us_topic_0175230600_p58060511143246"></a><a name="en-us_topic_0175230600_p58060511143246"></a>LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p5280980143246"><a name="en-us_topic_0175230600_p5280980143246"></a><a name="en-us_topic_0175230600_p5280980143246"></a>0x02000221</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p25106258143246"><a name="en-us_topic_0175230600_p25106258143246"></a><a name="en-us_topic_0175230600_p25106258143246"></a>This error code is not used currently.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1523825016557"><a name="en-us_topic_0175230600_p1523825016557"></a><a name="en-us_topic_0175230600_p1523825016557"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row155365520513"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p1955415512516"><a name="en-us_topic_0175230600_p1955415512516"></a><a name="en-us_topic_0175230600_p1955415512516"></a>34</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p05541955125115"><a name="en-us_topic_0175230600_p05541955125115"></a><a name="en-us_topic_0175230600_p05541955125115"></a>LOS_ERRNO_TSK_CPU_AFFINITY_MASK_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p135541755105116"><a name="en-us_topic_0175230600_p135541755105116"></a><a name="en-us_topic_0175230600_p135541755105116"></a>0x03000223</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1455411559518"><a name="en-us_topic_0175230600_p1455411559518"></a><a name="en-us_topic_0175230600_p1455411559518"></a>An incorrect CPU collection is set for the task.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p18554655165110"><a name="en-us_topic_0175230600_p18554655165110"></a><a name="en-us_topic_0175230600_p18554655165110"></a>Check the CPU collection.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row625620114525"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p125610105210"><a name="en-us_topic_0175230600_p125610105210"></a><a name="en-us_topic_0175230600_p125610105210"></a>35</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p625651125220"><a name="en-us_topic_0175230600_p625651125220"></a><a name="en-us_topic_0175230600_p625651125220"></a>LOS_ERRNO_TSK_YIELD_IN_INT</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1225611165210"><a name="en-us_topic_0175230600_p1225611165210"></a><a name="en-us_topic_0175230600_p1225611165210"></a>0x02000224</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1125611165217"><a name="en-us_topic_0175230600_p1125611165217"></a><a name="en-us_topic_0175230600_p1125611165217"></a>The task to be yielded is under an interrupt.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p1425616113522"><a name="en-us_topic_0175230600_p1425616113522"></a><a name="en-us_topic_0175230600_p1425616113522"></a>Yield the task after it exits the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row10426134810919"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p10427134817915"><a name="en-us_topic_0175230600_p10427134817915"></a><a name="en-us_topic_0175230600_p10427134817915"></a>36</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p742712485915"><a name="en-us_topic_0175230600_p742712485915"></a><a name="en-us_topic_0175230600_p742712485915"></a>LOS_ERRNO_TSK_MP_SYNC_RESOURCE</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p13427184812917"><a name="en-us_topic_0175230600_p13427184812917"></a><a name="en-us_topic_0175230600_p13427184812917"></a>0x02000225</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p081164616317"><a name="en-us_topic_0175230600_p081164616317"></a><a name="en-us_topic_0175230600_p081164616317"></a>Resource application fails during cross-core task deletion.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p3427348797"><a name="en-us_topic_0175230600_p3427348797"></a><a name="en-us_topic_0175230600_p3427348797"></a>Set <strong id="en-us_topic_0175230600_b189093445919"><a name="en-us_topic_0175230600_b189093445919"></a><a name="en-us_topic_0175230600_b189093445919"></a>LOSCFG_BASE_IPC_SEM_LIMIT</strong> to a larger value to add semaphores.</p>
</td>
</tr>
<tr id="en-us_topic_0175230600_row1111865316913"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p1111810535912"><a name="en-us_topic_0175230600_p1111810535912"></a><a name="en-us_topic_0175230600_p1111810535912"></a>37</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p01181453495"><a name="en-us_topic_0175230600_p01181453495"></a><a name="en-us_topic_0175230600_p01181453495"></a>LOS_ERRNO_TSK_MP_SYNC_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="11.62%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1311816538913"><a name="en-us_topic_0175230600_p1311816538913"></a><a name="en-us_topic_0175230600_p1311816538913"></a>0x02000226</p>
</td>
<td class="cellrowborder" valign="top" width="29.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p14118205314910"><a name="en-us_topic_0175230600_p14118205314910"></a><a name="en-us_topic_0175230600_p14118205314910"></a>A task is not promptly deleted during cross-core task deletion.</p>
</td>
<td class="cellrowborder" valign="top" width="31.619999999999997%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p121184531916"><a name="en-us_topic_0175230600_p121184531916"></a><a name="en-us_topic_0175230600_p121184531916"></a>Check whether the target task frequently switches among different states. Such tasks cannot be deleted within the specified time.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>-   For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the task module, and the value is  **0x02**.
>-   The error codes 0x16 and 0x1c in the task module are not defined and cannot be used.

## Development Process<a name="en-us_topic_0175230600_section159273414243"></a>

Task creation is used as an example to explain the development process.

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Basic Config**  \>  **Task**  to configure the task module.

    <a name="en-us_topic_0175230600_table12907575155221"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230600_row23629430155221"><th class="cellrowborder" valign="top" width="26.750000000000007%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230600_p61687296155221"><a name="en-us_topic_0175230600_p61687296155221"></a><a name="en-us_topic_0175230600_p61687296155221"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="39.410000000000004%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230600_p25007692155221"><a name="en-us_topic_0175230600_p25007692155221"></a><a name="en-us_topic_0175230600_p25007692155221"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="15.830000000000002%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230600_p47081329204415"><a name="en-us_topic_0175230600_p47081329204415"></a><a name="en-us_topic_0175230600_p47081329204415"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="10.430000000000001%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230600_p7383544155221"><a name="en-us_topic_0175230600_p7383544155221"></a><a name="en-us_topic_0175230600_p7383544155221"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="7.580000000000002%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230600_p34917797155221"><a name="en-us_topic_0175230600_p34917797155221"></a><a name="en-us_topic_0175230600_p34917797155221"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230600_row64280956155221"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p250114575396"><a name="en-us_topic_0175230600_p250114575396"></a><a name="en-us_topic_0175230600_p250114575396"></a>LOSCFG_BASE_CORE_TSK_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p49931484155221"><a name="en-us_topic_0175230600_p49931484155221"></a><a name="en-us_topic_0175230600_p49931484155221"></a>Maximum number of tasks supported by the system.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p147081729164416"><a name="en-us_topic_0175230600_p147081729164416"></a><a name="en-us_topic_0175230600_p147081729164416"></a>[0, OS_SYS_MEM_SIZE)</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p5304493155221"><a name="en-us_topic_0175230600_p5304493155221"></a><a name="en-us_topic_0175230600_p5304493155221"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p57580333155221"><a name="en-us_topic_0175230600_p57580333155221"></a><a name="en-us_topic_0175230600_p57580333155221"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row73250333916"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p1719350892834"><a name="en-us_topic_0175230600_p1719350892834"></a><a name="en-us_topic_0175230600_p1719350892834"></a>LOSCFG_BASE_CORE_TSK_MIN_STACK_SIZE</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p5049690492834"><a name="en-us_topic_0175230600_p5049690492834"></a><a name="en-us_topic_0175230600_p5049690492834"></a>Minimum size of the task stack. In most cases, retain the default value.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p870810296444"><a name="en-us_topic_0175230600_p870810296444"></a><a name="en-us_topic_0175230600_p870810296444"></a>[0, OS_SYS_MEM_SIZE)</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1936214173244"><a name="en-us_topic_0175230600_p1936214173244"></a><a name="en-us_topic_0175230600_p1936214173244"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p2910031392834"><a name="en-us_topic_0175230600_p2910031392834"></a><a name="en-us_topic_0175230600_p2910031392834"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row32051702155221"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p24460360155644"><a name="en-us_topic_0175230600_p24460360155644"></a><a name="en-us_topic_0175230600_p24460360155644"></a>LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p40300143155221"><a name="en-us_topic_0175230600_p40300143155221"></a><a name="en-us_topic_0175230600_p40300143155221"></a>Default size of the task stack.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1708429134410"><a name="en-us_topic_0175230600_p1708429134410"></a><a name="en-us_topic_0175230600_p1708429134410"></a>[0, OS_SYS_MEM_SIZE)</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p35008432155221"><a name="en-us_topic_0175230600_p35008432155221"></a><a name="en-us_topic_0175230600_p35008432155221"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p3680129155221"><a name="en-us_topic_0175230600_p3680129155221"></a><a name="en-us_topic_0175230600_p3680129155221"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row1635511631015"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p10343440155221"><a name="en-us_topic_0175230600_p10343440155221"></a><a name="en-us_topic_0175230600_p10343440155221"></a>LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p65163552155221"><a name="en-us_topic_0175230600_p65163552155221"></a><a name="en-us_topic_0175230600_p65163552155221"></a>Size of the idle task stack. In most cases, retain the default value.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p11708129174419"><a name="en-us_topic_0175230600_p11708129174419"></a><a name="en-us_topic_0175230600_p11708129174419"></a>[0, OS_SYS_MEM_SIZE)</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p184744132215"><a name="en-us_topic_0175230600_p184744132215"></a><a name="en-us_topic_0175230600_p184744132215"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p32619484155221"><a name="en-us_topic_0175230600_p32619484155221"></a><a name="en-us_topic_0175230600_p32619484155221"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row1093015614112"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p3453970192846"><a name="en-us_topic_0175230600_p3453970192846"></a><a name="en-us_topic_0175230600_p3453970192846"></a>LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p4625240192846"><a name="en-us_topic_0175230600_p4625240192846"></a><a name="en-us_topic_0175230600_p4625240192846"></a>Default task priority. In most cases, retain the default value.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1070882954416"><a name="en-us_topic_0175230600_p1070882954416"></a><a name="en-us_topic_0175230600_p1070882954416"></a>[0,31]</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p6283180292846"><a name="en-us_topic_0175230600_p6283180292846"></a><a name="en-us_topic_0175230600_p6283180292846"></a>10</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p5621122492846"><a name="en-us_topic_0175230600_p5621122492846"></a><a name="en-us_topic_0175230600_p5621122492846"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row57489744155221"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p52935610155221"><a name="en-us_topic_0175230600_p52935610155221"></a><a name="en-us_topic_0175230600_p52935610155221"></a>LOSCFG_BASE_CORE_TIMESLICE</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p57828477155221"><a name="en-us_topic_0175230600_p57828477155221"></a><a name="en-us_topic_0175230600_p57828477155221"></a>Whether to schedule the task based on time slices.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p37081298441"><a name="en-us_topic_0175230600_p37081298441"></a><a name="en-us_topic_0175230600_p37081298441"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p34141688155221"><a name="en-us_topic_0175230600_p34141688155221"></a><a name="en-us_topic_0175230600_p34141688155221"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p24099051155221"><a name="en-us_topic_0175230600_p24099051155221"></a><a name="en-us_topic_0175230600_p24099051155221"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row44343148155221"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p26649168155221"><a name="en-us_topic_0175230600_p26649168155221"></a><a name="en-us_topic_0175230600_p26649168155221"></a>LOSCFG_BASE_CORE_TIMESLICE_TIMEOUT</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p8231977155221"><a name="en-us_topic_0175230600_p8231977155221"></a><a name="en-us_topic_0175230600_p8231977155221"></a>Maximum execution time of tasks with the same priority (unit: tick).</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p1270811297446"><a name="en-us_topic_0175230600_p1270811297446"></a><a name="en-us_topic_0175230600_p1270811297446"></a>[0, 65535]</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p14238792155221"><a name="en-us_topic_0175230600_p14238792155221"></a><a name="en-us_topic_0175230600_p14238792155221"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p38182968155221"><a name="en-us_topic_0175230600_p38182968155221"></a><a name="en-us_topic_0175230600_p38182968155221"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row1278217571126"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p147827579217"><a name="en-us_topic_0175230600_p147827579217"></a><a name="en-us_topic_0175230600_p147827579217"></a>LOSCFG_OBSOLETE_API</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p5782357327"><a name="en-us_topic_0175230600_p5782357327"></a><a name="en-us_topic_0175230600_p5782357327"></a>Whether to use the old task parameter <strong id="en-us_topic_0175230600_b4127135631713"><a name="en-us_topic_0175230600_b4127135631713"></a><a name="en-us_topic_0175230600_b4127135631713"></a>UINTPTR auwArgs[4]</strong> instead of the new <strong id="en-us_topic_0175230600_b1630515131813"><a name="en-us_topic_0175230600_b1630515131813"></a><a name="en-us_topic_0175230600_b1630515131813"></a>VOID *pArgs</strong>. You are advised to disable this function and use the new task parameter.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p18280510814"><a name="en-us_topic_0175230600_p18280510814"></a><a name="en-us_topic_0175230600_p18280510814"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p112801811985"><a name="en-us_topic_0175230600_p112801811985"></a><a name="en-us_topic_0175230600_p112801811985"></a>The default value varies depending on the platform.</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p16782175717218"><a name="en-us_topic_0175230600_p16782175717218"></a><a name="en-us_topic_0175230600_p16782175717218"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row15335440215"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p83359408120"><a name="en-us_topic_0175230600_p83359408120"></a><a name="en-us_topic_0175230600_p83359408120"></a>LOSCFG_LAZY_STACK</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p2033514402011"><a name="en-us_topic_0175230600_p2033514402011"></a><a name="en-us_topic_0175230600_p2033514402011"></a>Whether to enable the inert stack.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p8125122914217"><a name="en-us_topic_0175230600_p8125122914217"></a><a name="en-us_topic_0175230600_p8125122914217"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1312510299211"><a name="en-us_topic_0175230600_p1312510299211"></a><a name="en-us_topic_0175230600_p1312510299211"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p183352040418"><a name="en-us_topic_0175230600_p183352040418"></a><a name="en-us_topic_0175230600_p183352040418"></a>M-core</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row49861795155714"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p44453260155714"><a name="en-us_topic_0175230600_p44453260155714"></a><a name="en-us_topic_0175230600_p44453260155714"></a>LOSCFG_BASE_CORE_TSK_MONITOR</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p8099496155714"><a name="en-us_topic_0175230600_p8099496155714"></a><a name="en-us_topic_0175230600_p8099496155714"></a>Whether to monitor task stack overflows and tracks.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p6708112912448"><a name="en-us_topic_0175230600_p6708112912448"></a><a name="en-us_topic_0175230600_p6708112912448"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p66977673114339"><a name="en-us_topic_0175230600_p66977673114339"></a><a name="en-us_topic_0175230600_p66977673114339"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p12893852155714"><a name="en-us_topic_0175230600_p12893852155714"></a><a name="en-us_topic_0175230600_p12893852155714"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230600_row83121140981"><td class="cellrowborder" valign="top" width="26.750000000000007%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230600_p42295187592"><a name="en-us_topic_0175230600_p42295187592"></a><a name="en-us_topic_0175230600_p42295187592"></a>LOSCFG_TASK_STATIC_ALLOCATION</p>
    </td>
    <td class="cellrowborder" valign="top" width="39.410000000000004%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230600_p822914184592"><a name="en-us_topic_0175230600_p822914184592"></a><a name="en-us_topic_0175230600_p822914184592"></a>Whether to allow users to input the task stack during task creation.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.830000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230600_p922919187592"><a name="en-us_topic_0175230600_p922919187592"></a><a name="en-us_topic_0175230600_p922919187592"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.430000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230600_p1222921812598"><a name="en-us_topic_0175230600_p1222921812598"></a><a name="en-us_topic_0175230600_p1222921812598"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="7.580000000000002%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230600_p202291118185920"><a name="en-us_topic_0175230600_p202291118185920"></a><a name="en-us_topic_0175230600_p202291118185920"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Call the LOS\_TaskLock API to lock task scheduling so that high-priority tasks will not be scheduled first.
3.  Call the LOS\_TaskCreate or LOS\_TaskCreateStatic API to create a task. \(The  **LOSCFG\_TASK\_STATIC\_ALLOCATION**  macro must be enabled first.\)
4.  Call the LOS\_TaskUnlock API to unlock task scheduling so that tasks will be scheduled by priority.
5.  Call the LOS\_TaskDelay API to delay the task.
6.  Call the LOS\_TaskSuspend API to suspend the task.
7.  Call the LOS\_TaskResume API to resume the suspended task.

## Platform Differences<a name="en-us_topic_0175230600_section25619610165222"></a>

None


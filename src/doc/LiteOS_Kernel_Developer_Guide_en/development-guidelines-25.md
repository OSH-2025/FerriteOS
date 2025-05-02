# Development Guidelines<a name="EN-US_TOPIC_0311018416"></a>

## Usage Scenarios<a name="en-us_topic_0175230638_section61127176164034"></a>

In the multi-task system, semaphore is a flexible synchronization mode. It can be used in multiple scenarios to implement functions such as lock, synchronization, and resource counting. It can also be used in synchronization between tasks and between an interrupt and a task. Typically, a semaphore is used to coordinate a group of competing tasks to access to shared resources.

## Functions<a name="en-us_topic_0175230638_section49492823145823"></a>

The semaphore module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230638_table42808011145823"></a>
<table><thead align="left"><tr id="en-us_topic_0175230638_row27710684145823"><th class="cellrowborder" valign="top" width="20.102010201020104%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230638_p29972955145823"><a name="en-us_topic_0175230638_p29972955145823"></a><a name="en-us_topic_0175230638_p29972955145823"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="20.52205220522052%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230638_p11890286145823"><a name="en-us_topic_0175230638_p11890286145823"></a><a name="en-us_topic_0175230638_p11890286145823"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="59.375937593759375%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230638_p23589118145823"><a name="en-us_topic_0175230638_p23589118145823"></a><a name="en-us_topic_0175230638_p23589118145823"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230638_row31670386145823"><td class="cellrowborder" rowspan="3" valign="top" width="20.102010201020104%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230638_p15164467145823"><a name="en-us_topic_0175230638_p15164467145823"></a><a name="en-us_topic_0175230638_p15164467145823"></a>Semaphore creation or deletion</p>
</td>
<td class="cellrowborder" valign="top" width="20.52205220522052%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230638_p20362280145823"><a name="en-us_topic_0175230638_p20362280145823"></a><a name="en-us_topic_0175230638_p20362280145823"></a>LOS_SemCreate</p>
</td>
<td class="cellrowborder" valign="top" width="59.375937593759375%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230638_p38731961145823"><a name="en-us_topic_0175230638_p38731961145823"></a><a name="en-us_topic_0175230638_p38731961145823"></a>Creates a semaphore and returns a semaphore ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row11854496016"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230638_p1851149602"><a name="en-us_topic_0175230638_p1851149602"></a><a name="en-us_topic_0175230638_p1851149602"></a>LOS_BinarySemCreate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230638_p108544917020"><a name="en-us_topic_0175230638_p108544917020"></a><a name="en-us_topic_0175230638_p108544917020"></a>Creates a binary semaphore. The maximum counter value is 1.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row13043336145823"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230638_p161317214406"><a name="en-us_topic_0175230638_p161317214406"></a><a name="en-us_topic_0175230638_p161317214406"></a>LOS_SemDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230638_p13531752145823"><a name="en-us_topic_0175230638_p13531752145823"></a><a name="en-us_topic_0175230638_p13531752145823"></a>Deletes a semaphore.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row54676905145823"><td class="cellrowborder" rowspan="2" valign="top" width="20.102010201020104%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230638_p66753213145823"><a name="en-us_topic_0175230638_p66753213145823"></a><a name="en-us_topic_0175230638_p66753213145823"></a>Semaphore request or release</p>
</td>
<td class="cellrowborder" valign="top" width="20.52205220522052%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230638_p29675167144037"><a name="en-us_topic_0175230638_p29675167144037"></a><a name="en-us_topic_0175230638_p29675167144037"></a>LOS_SemPend</p>
</td>
<td class="cellrowborder" valign="top" width="59.375937593759375%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230638_p15387322145823"><a name="en-us_topic_0175230638_p15387322145823"></a><a name="en-us_topic_0175230638_p15387322145823"></a>Requests a specified semaphore and sets the timeout interval.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row4268172145823"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230638_p33315591144148"><a name="en-us_topic_0175230638_p33315591144148"></a><a name="en-us_topic_0175230638_p33315591144148"></a>LOS_SemPost</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230638_p19085650145823"><a name="en-us_topic_0175230638_p19085650145823"></a><a name="en-us_topic_0175230638_p19085650145823"></a>Posts (releases) a semaphore.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>There are three semaphore request modes: non-block mode, permanent block mode, and scheduled block mode.
>-   Non-block mode: When a task requests a semaphore, the input parameter timeout is 0. If the current semaphore counter value is not 0, the request is successful. Otherwise, a request failure message is returned immediately.
>-   Permanent mode: When a task requests a semaphore, the input parameter timeout is 0xFFFFFFFF. If the current semaphore counter value is not 0, the request is successful. Otherwise, the task enters the blocked state, and the system switches to the ready task with the highest priority and continues task execution. After a task enters the blocked state, the blocked task can be executed again only after another task releases the semaphore.
>-   Scheduled block mode: When a task requests a semaphore, the input parameter timeout is greater than 0 and smaller than 0xFFFFFFFF. If the current semaphore counter value is not 0, the request is successful. Otherwise, the task enters the blocked state, and the system switches to the ready task with the highest priority and continues task execution. After a task enters the blocked state, if another task releases the semaphore before the timeout occurs, the task can successfully obtain the semaphore and continue to be executed. If the semaphore is not obtained before the timeout occurs, the API returns the timeout error code.

## Semaphore Error Codes<a name="en-us_topic_0175230638_section868101994847"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230638_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230638_row2267197395642"><th class="cellrowborder" valign="top" width="5.65%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230638_p1908783195642"><a name="en-us_topic_0175230638_p1908783195642"></a><a name="en-us_topic_0175230638_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="21.15%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230638_p261046995642"><a name="en-us_topic_0175230638_p261046995642"></a><a name="en-us_topic_0175230638_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="13.089999999999998%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230638_p1012144095642"><a name="en-us_topic_0175230638_p1012144095642"></a><a name="en-us_topic_0175230638_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="26.91%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230638_p1453028795642"><a name="en-us_topic_0175230638_p1453028795642"></a><a name="en-us_topic_0175230638_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="33.2%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230638_p2753561710026"><a name="en-us_topic_0175230638_p2753561710026"></a><a name="en-us_topic_0175230638_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230638_row6366372295642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p5648782795642"><a name="en-us_topic_0175230638_p5648782795642"></a><a name="en-us_topic_0175230638_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p1211123695642"><a name="en-us_topic_0175230638_p1211123695642"></a><a name="en-us_topic_0175230638_p1211123695642"></a>LOS_ERRNO_SEM_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p4148605495642"><a name="en-us_topic_0175230638_p4148605495642"></a><a name="en-us_topic_0175230638_p4148605495642"></a>0x02000700</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p492720095642"><a name="en-us_topic_0175230638_p492720095642"></a><a name="en-us_topic_0175230638_p492720095642"></a>The memory space is insufficient when the semaphore is initialized.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p145551278485"><a name="en-us_topic_0175230638_p145551278485"></a><a name="en-us_topic_0175230638_p145551278485"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for semaphores, or reduce the maximum number (LOSCFG_BASE_IPC_SEM_LIMIT) of semaphores supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row4434480695642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p3515954495642"><a name="en-us_topic_0175230638_p3515954495642"></a><a name="en-us_topic_0175230638_p3515954495642"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p2935083995642"><a name="en-us_topic_0175230638_p2935083995642"></a><a name="en-us_topic_0175230638_p2935083995642"></a>LOS_ERRNO_SEM_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p2860775495642"><a name="en-us_topic_0175230638_p2860775495642"></a><a name="en-us_topic_0175230638_p2860775495642"></a>0x02000701</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p3552674695642"><a name="en-us_topic_0175230638_p3552674695642"></a><a name="en-us_topic_0175230638_p3552674695642"></a>The semaphore ID is incorrect or the semaphore is not created.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p412423410026"><a name="en-us_topic_0175230638_p412423410026"></a><a name="en-us_topic_0175230638_p412423410026"></a>Pass in a correct semaphore ID or create a semaphore before using it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row5130526095642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p6208537795642"><a name="en-us_topic_0175230638_p6208537795642"></a><a name="en-us_topic_0175230638_p6208537795642"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p6285965595642"><a name="en-us_topic_0175230638_p6285965595642"></a><a name="en-us_topic_0175230638_p6285965595642"></a>LOS_ERRNO_SEM_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p5846730195642"><a name="en-us_topic_0175230638_p5846730195642"></a><a name="en-us_topic_0175230638_p5846730195642"></a>0x02000702</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p3823098095642"><a name="en-us_topic_0175230638_p3823098095642"></a><a name="en-us_topic_0175230638_p3823098095642"></a>The passed-in pointer is null.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p6562756010026"><a name="en-us_topic_0175230638_p6562756010026"></a><a name="en-us_topic_0175230638_p6562756010026"></a>Pass in a non-null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row853450895642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p2020651095642"><a name="en-us_topic_0175230638_p2020651095642"></a><a name="en-us_topic_0175230638_p2020651095642"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p2611462995642"><a name="en-us_topic_0175230638_p2611462995642"></a><a name="en-us_topic_0175230638_p2611462995642"></a>LOS_ERRNO_SEM_ALL_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p3491023395642"><a name="en-us_topic_0175230638_p3491023395642"></a><a name="en-us_topic_0175230638_p3491023395642"></a>0x02000703</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p915662595642"><a name="en-us_topic_0175230638_p915662595642"></a><a name="en-us_topic_0175230638_p915662595642"></a>When a semaphore is created, no unused semaphore exists in the system.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p1423211810026"><a name="en-us_topic_0175230638_p1423211810026"></a><a name="en-us_topic_0175230638_p1423211810026"></a>Delete useless semaphores or increase the maximum number of semaphores (LOSCFG_BASE_IPC_SEM_LIMIT) supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row1530076695642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p3140256895642"><a name="en-us_topic_0175230638_p3140256895642"></a><a name="en-us_topic_0175230638_p3140256895642"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p6058010795642"><a name="en-us_topic_0175230638_p6058010795642"></a><a name="en-us_topic_0175230638_p6058010795642"></a>LOS_ERRNO_SEM_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p804161495642"><a name="en-us_topic_0175230638_p804161495642"></a><a name="en-us_topic_0175230638_p804161495642"></a>0x02000704</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p10819081152943"><a name="en-us_topic_0175230638_p10819081152943"></a><a name="en-us_topic_0175230638_p10819081152943"></a>Failed to obtain the semaphore in non-block mode.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p42669253153017"><a name="en-us_topic_0175230638_p42669253153017"></a><a name="en-us_topic_0175230638_p42669253153017"></a>Block the task and wait, or handle the problem based on the error code.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row2386554195642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p5406070595642"><a name="en-us_topic_0175230638_p5406070595642"></a><a name="en-us_topic_0175230638_p5406070595642"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p1684095295642"><a name="en-us_topic_0175230638_p1684095295642"></a><a name="en-us_topic_0175230638_p1684095295642"></a>LOS_ERRNO_SEM_PEND_INTERR</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p2193989295642"><a name="en-us_topic_0175230638_p2193989295642"></a><a name="en-us_topic_0175230638_p2193989295642"></a>0x02000705</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p63490793152953"><a name="en-us_topic_0175230638_p63490793152953"></a><a name="en-us_topic_0175230638_p63490793152953"></a>During the interrupt, the LOS_SemPend API is called to request a semaphore.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p43375472153017"><a name="en-us_topic_0175230638_p43375472153017"></a><a name="en-us_topic_0175230638_p43375472153017"></a>Do not call the LOS_SemPend API during the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row2227229895642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p5922568095642"><a name="en-us_topic_0175230638_p5922568095642"></a><a name="en-us_topic_0175230638_p5922568095642"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p3255077595642"><a name="en-us_topic_0175230638_p3255077595642"></a><a name="en-us_topic_0175230638_p3255077595642"></a>LOS_ERRNO_SEM_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p1936711095642"><a name="en-us_topic_0175230638_p1936711095642"></a><a name="en-us_topic_0175230638_p1936711095642"></a>0x02000706</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p52085876152953"><a name="en-us_topic_0175230638_p52085876152953"></a><a name="en-us_topic_0175230638_p52085876152953"></a>The task is locked and fails to obtain the semaphore.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p36140674153017"><a name="en-us_topic_0175230638_p36140674153017"></a><a name="en-us_topic_0175230638_p36140674153017"></a>Do not call the LOS_SemPend API when the task is locked.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row43458669101114"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p30491270101114"><a name="en-us_topic_0175230638_p30491270101114"></a><a name="en-us_topic_0175230638_p30491270101114"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p53873767101114"><a name="en-us_topic_0175230638_p53873767101114"></a><a name="en-us_topic_0175230638_p53873767101114"></a>LOS_ERRNO_SEM_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p1699001101114"><a name="en-us_topic_0175230638_p1699001101114"></a><a name="en-us_topic_0175230638_p1699001101114"></a>0x02000707</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p28760934152953"><a name="en-us_topic_0175230638_p28760934152953"></a><a name="en-us_topic_0175230638_p28760934152953"></a>Obtaining the semaphore timed out.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p48623836153017"><a name="en-us_topic_0175230638_p48623836153017"></a><a name="en-us_topic_0175230638_p48623836153017"></a>Set the time to a proper range.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row12143904101125"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p44132204101125"><a name="en-us_topic_0175230638_p44132204101125"></a><a name="en-us_topic_0175230638_p44132204101125"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p17938739101125"><a name="en-us_topic_0175230638_p17938739101125"></a><a name="en-us_topic_0175230638_p17938739101125"></a>LOS_ERRNO_SEM_OVERFLOW</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p43751775101125"><a name="en-us_topic_0175230638_p43751775101125"></a><a name="en-us_topic_0175230638_p43751775101125"></a>0x02000708</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p35771040152953"><a name="en-us_topic_0175230638_p35771040152953"></a><a name="en-us_topic_0175230638_p35771040152953"></a>The semaphore counter value has reached the maximum, and the semaphore cannot be released.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p7712100153017"><a name="en-us_topic_0175230638_p7712100153017"></a><a name="en-us_topic_0175230638_p7712100153017"></a>Handle the problem based on the error code.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row44185939101121"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p22291345101121"><a name="en-us_topic_0175230638_p22291345101121"></a><a name="en-us_topic_0175230638_p22291345101121"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p60768531101121"><a name="en-us_topic_0175230638_p60768531101121"></a><a name="en-us_topic_0175230638_p60768531101121"></a>LOS_ERRNO_SEM_PENDED</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p23304002101121"><a name="en-us_topic_0175230638_p23304002101121"></a><a name="en-us_topic_0175230638_p23304002101121"></a>0x02000709</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p52495624152953"><a name="en-us_topic_0175230638_p52495624152953"></a><a name="en-us_topic_0175230638_p52495624152953"></a>The task queue waiting for the semaphore is not empty.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p9449161153017"><a name="en-us_topic_0175230638_p9449161153017"></a><a name="en-us_topic_0175230638_p9449161153017"></a>Wake up all tasks that are waiting for the semaphore, and then delete the semaphore.</p>
</td>
</tr>
<tr id="en-us_topic_0175230638_row14232120451"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p8232012154514"><a name="en-us_topic_0175230638_p8232012154514"></a><a name="en-us_topic_0175230638_p8232012154514"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="21.15%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p1623121284516"><a name="en-us_topic_0175230638_p1623121284516"></a><a name="en-us_topic_0175230638_p1623121284516"></a>LOS_ERRNO_SEM_PEND_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="13.089999999999998%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p323131215459"><a name="en-us_topic_0175230638_p323131215459"></a><a name="en-us_topic_0175230638_p323131215459"></a>0x0200070a</p>
</td>
<td class="cellrowborder" valign="top" width="26.91%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p1123512134512"><a name="en-us_topic_0175230638_p1123512134512"></a><a name="en-us_topic_0175230638_p1123512134512"></a>Semaphores are obtained from system tasks, including idle and software timer tasks.</p>
</td>
<td class="cellrowborder" valign="top" width="33.2%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p162331214458"><a name="en-us_topic_0175230638_p162331214458"></a><a name="en-us_topic_0175230638_p162331214458"></a>Do not obtain semaphores from system tasks.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the semaphore module, and the value is  **0x07**.

## Development Process<a name="en-us_topic_0175230638_section486548315050"></a>

The typical semaphore development process is as follows:

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Sem**  to configure the semaphore module.

    <a name="en-us_topic_0175230638_table34659900162615"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230638_row3414390162615"><th class="cellrowborder" valign="top" width="25.757424257574236%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230638_p8130151162615"><a name="en-us_topic_0175230638_p8130151162615"></a><a name="en-us_topic_0175230638_p8130151162615"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="30.456954304569532%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230638_p54562526162615"><a name="en-us_topic_0175230638_p54562526162615"></a><a name="en-us_topic_0175230638_p54562526162615"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="16.6983301669833%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230638_p57488448162615"><a name="en-us_topic_0175230638_p57488448162615"></a><a name="en-us_topic_0175230638_p57488448162615"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.85861413858614%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230638_p26052727162615"><a name="en-us_topic_0175230638_p26052727162615"></a><a name="en-us_topic_0175230638_p26052727162615"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.228677132286768%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230638_p29896171162615"><a name="en-us_topic_0175230638_p29896171162615"></a><a name="en-us_topic_0175230638_p29896171162615"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230638_row5670787162615"><td class="cellrowborder" valign="top" width="25.757424257574236%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p56680616162615"><a name="en-us_topic_0175230638_p56680616162615"></a><a name="en-us_topic_0175230638_p56680616162615"></a>LOSCFG_BASE_IPC_SEM</p>
    </td>
    <td class="cellrowborder" valign="top" width="30.456954304569532%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p27727169162615"><a name="en-us_topic_0175230638_p27727169162615"></a><a name="en-us_topic_0175230638_p27727169162615"></a>Whether to enable the tailoring function of the semaphore module.</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.6983301669833%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p31308215162615"><a name="en-us_topic_0175230638_p31308215162615"></a><a name="en-us_topic_0175230638_p31308215162615"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.85861413858614%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p52937457162615"><a name="en-us_topic_0175230638_p52937457162615"></a><a name="en-us_topic_0175230638_p52937457162615"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.228677132286768%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p60075607162615"><a name="en-us_topic_0175230638_p60075607162615"></a><a name="en-us_topic_0175230638_p60075607162615"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230638_row3809559162615"><td class="cellrowborder" valign="top" width="25.757424257574236%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230638_p40138859162615"><a name="en-us_topic_0175230638_p40138859162615"></a><a name="en-us_topic_0175230638_p40138859162615"></a>LOSCFG_BASE_IPC_SEM_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="30.456954304569532%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230638_p30022176162615"><a name="en-us_topic_0175230638_p30022176162615"></a><a name="en-us_topic_0175230638_p30022176162615"></a>Maximum number of semaphores supported by the system</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.6983301669833%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230638_p15877214162615"><a name="en-us_topic_0175230638_p15877214162615"></a><a name="en-us_topic_0175230638_p15877214162615"></a>[0, 65535]</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.85861413858614%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230638_p10985932162615"><a name="en-us_topic_0175230638_p10985932162615"></a><a name="en-us_topic_0175230638_p10985932162615"></a>1024</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.228677132286768%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230638_p17445266162615"><a name="en-us_topic_0175230638_p17445266162615"></a><a name="en-us_topic_0175230638_p17445266162615"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Call the LOS\_SemCreate API to create a semaphore. To create a binary semaphore, call the LOS\_BinarySemCreate API.
3.  Call the LOS\_SemPend API to request a semaphore.
4.  Call the LOS\_SemPost API to release a semaphore.
5.  Call the LOS\_SemDelete API to delete a semaphore.

## Platform Differences<a name="en-us_topic_0175230638_section28221686151840"></a>

None


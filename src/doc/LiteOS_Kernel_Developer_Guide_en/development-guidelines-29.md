# Development Guidelines<a name="EN-US_TOPIC_0311018421"></a>

## Usage Scenarios<a name="en-us_topic_0175230221_section10746131142833"></a>

In a multi-task environment, multiple tasks often compete for the same critical resource. The mutex provides a mutex mechanism between tasks to prevent two tasks from accessing the same critical resource at the same time. In this way, exclusive access is implemented.

## Functions<a name="en-us_topic_0175230221_section34540545162410"></a>

The mutex module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230221_table14234001162410"></a>
<table><thead align="left"><tr id="en-us_topic_0175230221_row20709259162410"><th class="cellrowborder" valign="top" width="21.152115211521153%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230221_p66837260162410"><a name="en-us_topic_0175230221_p66837260162410"></a><a name="en-us_topic_0175230221_p66837260162410"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="21.992199219921993%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230221_p45109011162410"><a name="en-us_topic_0175230221_p45109011162410"></a><a name="en-us_topic_0175230221_p45109011162410"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="56.85568556855686%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230221_p29951237162410"><a name="en-us_topic_0175230221_p29951237162410"></a><a name="en-us_topic_0175230221_p29951237162410"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230221_row10131148162410"><td class="cellrowborder" rowspan="2" valign="top" width="21.152115211521153%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230221_p15316688162410"><a name="en-us_topic_0175230221_p15316688162410"></a><a name="en-us_topic_0175230221_p15316688162410"></a>Mutex creation and deletion</p>
</td>
<td class="cellrowborder" valign="top" width="21.992199219921993%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230221_p32692216162410"><a name="en-us_topic_0175230221_p32692216162410"></a><a name="en-us_topic_0175230221_p32692216162410"></a>LOS_MuxCreate</p>
</td>
<td class="cellrowborder" valign="top" width="56.85568556855686%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230221_p30823853162410"><a name="en-us_topic_0175230221_p30823853162410"></a><a name="en-us_topic_0175230221_p30823853162410"></a>Creates a mutex.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row8979223162410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230221_p56228490162410"><a name="en-us_topic_0175230221_p56228490162410"></a><a name="en-us_topic_0175230221_p56228490162410"></a>LOS_MuxDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230221_p58213821162410"><a name="en-us_topic_0175230221_p58213821162410"></a><a name="en-us_topic_0175230221_p58213821162410"></a>Deletes a mutex.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row54162342162410"><td class="cellrowborder" rowspan="2" valign="top" width="21.152115211521153%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230221_p25073564162410"><a name="en-us_topic_0175230221_p25073564162410"></a><a name="en-us_topic_0175230221_p25073564162410"></a>Mutex request and release</p>
</td>
<td class="cellrowborder" valign="top" width="21.992199219921993%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230221_p17692840162410"><a name="en-us_topic_0175230221_p17692840162410"></a><a name="en-us_topic_0175230221_p17692840162410"></a>LOS_MuxPend</p>
</td>
<td class="cellrowborder" valign="top" width="56.85568556855686%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230221_p23833903162410"><a name="en-us_topic_0175230221_p23833903162410"></a><a name="en-us_topic_0175230221_p23833903162410"></a>Requests a mutex.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row13178541162410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230221_p60828923162410"><a name="en-us_topic_0175230221_p60828923162410"></a><a name="en-us_topic_0175230221_p60828923162410"></a>LOS_MuxPost</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230221_p28195705162410"><a name="en-us_topic_0175230221_p28195705162410"></a><a name="en-us_topic_0175230221_p28195705162410"></a>Releases a mutex.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>There are three mutex request modes: non-block mode, permanent block mode, and scheduled block mode.
>-   Non-block mode: When a task requests a mutex, the input parameter timeout is 0. If no task has acquired the mutex or the task that has acquired the mutex is the same as the requesting task, the operating system grants the mutex to the requesting task.
>-   Permanent mode: When a task requests a mutex, the input parameter timeout is 0xFFFFFFFF. If the mutex has not been acquired by any task, the operating system grants the mutex to the requesting task. Otherwise, the task enters the blocked state, and the system switches to the ready task with the highest priority and continues task execution. After a task enters the blocked state, the blocked task can be executed again only after another task releases the mutex.
>-   Scheduled block mode: When a task requests a mutex, the input parameter timeout is greater than 0 and smaller than 0xFFFFFFFF. If the mutex has not been acquired by any task, the operating system grants the mutex to the requesting task. Otherwise, the task enters the blocked state, and the system switches to the ready task with the highest priority and continues task execution. After a task enters the blocked state, if another task releases the mutex before the timeout occurs, the task can successfully obtain the mutex and continue to be executed. If the mutex is not obtained before the timeout occurs, the API returns the timeout error code.
>Releasing a mutex.
>-   If some tasks are blocked due to the mutex, the operating system wakes up the blocked task with highest priority. The woken-up task then enters Ready state and is scheduled.
>-   If no tasks are blocked due to the mutex, the operating system released the mutex successfully.

## Mutex Error Code<a name="en-us_topic_0175230221_section5167218318523"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230221_table1155510185440"></a>
<table><thead align="left"><tr id="en-us_topic_0175230221_row30398401185440"><th class="cellrowborder" valign="top" width="6.18%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230221_p14509506185440"><a name="en-us_topic_0175230221_p14509506185440"></a><a name="en-us_topic_0175230221_p14509506185440"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="21.05%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230221_p34419344185440"><a name="en-us_topic_0175230221_p34419344185440"></a><a name="en-us_topic_0175230221_p34419344185440"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="12.25%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230221_p36503519185440"><a name="en-us_topic_0175230221_p36503519185440"></a><a name="en-us_topic_0175230221_p36503519185440"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="27.02%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230221_p3995095185440"><a name="en-us_topic_0175230221_p3995095185440"></a><a name="en-us_topic_0175230221_p3995095185440"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="33.5%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230221_p55167263185440"><a name="en-us_topic_0175230221_p55167263185440"></a><a name="en-us_topic_0175230221_p55167263185440"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230221_row26743320185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p18725340185440"><a name="en-us_topic_0175230221_p18725340185440"></a><a name="en-us_topic_0175230221_p18725340185440"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p40357572185440"><a name="en-us_topic_0175230221_p40357572185440"></a><a name="en-us_topic_0175230221_p40357572185440"></a>LOS_ERRNO_MUX_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p47737913185440"><a name="en-us_topic_0175230221_p47737913185440"></a><a name="en-us_topic_0175230221_p47737913185440"></a>0x02001d00</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p41565730185440"><a name="en-us_topic_0175230221_p41565730185440"></a><a name="en-us_topic_0175230221_p41565730185440"></a>The memory is insufficient during initialization of the mutex module.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p829261710818"><a name="en-us_topic_0175230221_p829261710818"></a><a name="en-us_topic_0175230221_p829261710818"></a>Set the configuration item <strong id="en-us_topic_0175230221_b36711229495"><a name="en-us_topic_0175230221_b36711229495"></a><a name="en-us_topic_0175230221_b36711229495"></a>OS_SYS_MEM_SIZE</strong> to a larger system dynamic memory pool, or reduce the maximum number of mutexes supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row35319849185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p42335492185440"><a name="en-us_topic_0175230221_p42335492185440"></a><a name="en-us_topic_0175230221_p42335492185440"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p6622832185440"><a name="en-us_topic_0175230221_p6622832185440"></a><a name="en-us_topic_0175230221_p6622832185440"></a>LOS_ERRNO_MUX_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p66687380185440"><a name="en-us_topic_0175230221_p66687380185440"></a><a name="en-us_topic_0175230221_p66687380185440"></a>0x02001d01</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p32968729185440"><a name="en-us_topic_0175230221_p32968729185440"></a><a name="en-us_topic_0175230221_p32968729185440"></a>The mutex is unavailable.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p53221415185440"><a name="en-us_topic_0175230221_p53221415185440"></a><a name="en-us_topic_0175230221_p53221415185440"></a>Pass in a valid mutex ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row9230688185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p9488269185440"><a name="en-us_topic_0175230221_p9488269185440"></a><a name="en-us_topic_0175230221_p9488269185440"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p30352343185440"><a name="en-us_topic_0175230221_p30352343185440"></a><a name="en-us_topic_0175230221_p30352343185440"></a>LOS_ERRNO_MUX_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p42620709185440"><a name="en-us_topic_0175230221_p42620709185440"></a><a name="en-us_topic_0175230221_p42620709185440"></a>0x02001d02</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p29725408185440"><a name="en-us_topic_0175230221_p29725408185440"></a><a name="en-us_topic_0175230221_p29725408185440"></a>The input parameter is null during mutex creation.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p58947823185440"><a name="en-us_topic_0175230221_p58947823185440"></a><a name="en-us_topic_0175230221_p58947823185440"></a>Pass in a non-null parameter.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row60768362185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p23290259185440"><a name="en-us_topic_0175230221_p23290259185440"></a><a name="en-us_topic_0175230221_p23290259185440"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p7462788185440"><a name="en-us_topic_0175230221_p7462788185440"></a><a name="en-us_topic_0175230221_p7462788185440"></a>LOS_ERRNO_MUX_ALL_BUSY</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p506086185440"><a name="en-us_topic_0175230221_p506086185440"></a><a name="en-us_topic_0175230221_p506086185440"></a>0x02001d03</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p40992992185440"><a name="en-us_topic_0175230221_p40992992185440"></a><a name="en-us_topic_0175230221_p40992992185440"></a>No mutexes are available during mutex creation.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p32098075185440"><a name="en-us_topic_0175230221_p32098075185440"></a><a name="en-us_topic_0175230221_p32098075185440"></a>Increase the maximum number of mutexes supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row20447224185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p45612471185440"><a name="en-us_topic_0175230221_p45612471185440"></a><a name="en-us_topic_0175230221_p45612471185440"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p3622666185440"><a name="en-us_topic_0175230221_p3622666185440"></a><a name="en-us_topic_0175230221_p3622666185440"></a>LOS_ERRNO_MUX_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p25000538185440"><a name="en-us_topic_0175230221_p25000538185440"></a><a name="en-us_topic_0175230221_p25000538185440"></a>0x02001d04</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p11777678185440"><a name="en-us_topic_0175230221_p11777678185440"></a><a name="en-us_topic_0175230221_p11777678185440"></a>The mutex fails to be requested, because it has been acquired by another thread.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p14467855185440"><a name="en-us_topic_0175230221_p14467855185440"></a><a name="en-us_topic_0175230221_p14467855185440"></a>Wait for another thread to release the mutex. Alternatively, set a timeout period.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row63101831185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p10974693185440"><a name="en-us_topic_0175230221_p10974693185440"></a><a name="en-us_topic_0175230221_p10974693185440"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p16534980185440"><a name="en-us_topic_0175230221_p16534980185440"></a><a name="en-us_topic_0175230221_p16534980185440"></a>LOS_ERRNO_MUX_PEND_INTERR</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p64265036185440"><a name="en-us_topic_0175230221_p64265036185440"></a><a name="en-us_topic_0175230221_p64265036185440"></a>0x02001d05</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p38085412185440"><a name="en-us_topic_0175230221_p38085412185440"></a><a name="en-us_topic_0175230221_p38085412185440"></a>Use mutexes in interrupts.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p65019508185440"><a name="en-us_topic_0175230221_p65019508185440"></a><a name="en-us_topic_0175230221_p65019508185440"></a>Do not request or release mutexes during interrupts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row48304665185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p20363793185440"><a name="en-us_topic_0175230221_p20363793185440"></a><a name="en-us_topic_0175230221_p20363793185440"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p38854550185440"><a name="en-us_topic_0175230221_p38854550185440"></a><a name="en-us_topic_0175230221_p38854550185440"></a>LOS_ERRNO_MUX_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p60210831185440"><a name="en-us_topic_0175230221_p60210831185440"></a><a name="en-us_topic_0175230221_p60210831185440"></a>0x02001d06</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p45239141185440"><a name="en-us_topic_0175230221_p45239141185440"></a><a name="en-us_topic_0175230221_p45239141185440"></a>When task scheduling is disabled, tasks are not allowed to request a mutex in block mode.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p40491775185440"><a name="en-us_topic_0175230221_p40491775185440"></a><a name="en-us_topic_0175230221_p40491775185440"></a>Requests a mutex in non-block mode. Alternatively, enable task scheduling before requesting a mutex in block mode.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row28881657185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p57712885185440"><a name="en-us_topic_0175230221_p57712885185440"></a><a name="en-us_topic_0175230221_p57712885185440"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p44232105185440"><a name="en-us_topic_0175230221_p44232105185440"></a><a name="en-us_topic_0175230221_p44232105185440"></a>LOS_ERRNO_MUX_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p26030782185440"><a name="en-us_topic_0175230221_p26030782185440"></a><a name="en-us_topic_0175230221_p26030782185440"></a>0x02001d07</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p28118620185440"><a name="en-us_topic_0175230221_p28118620185440"></a><a name="en-us_topic_0175230221_p28118620185440"></a>Mutex request times out.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p63015722185440"><a name="en-us_topic_0175230221_p63015722185440"></a><a name="en-us_topic_0175230221_p63015722185440"></a>Increase the wait time. Alternatively, set the mutex to the permanent block mode.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row30270586185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p35998420185440"><a name="en-us_topic_0175230221_p35998420185440"></a><a name="en-us_topic_0175230221_p35998420185440"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p30190898185440"><a name="en-us_topic_0175230221_p30190898185440"></a><a name="en-us_topic_0175230221_p30190898185440"></a>LOS_ERRNO_MUX_OVERFLOW</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p29543677185440"><a name="en-us_topic_0175230221_p29543677185440"></a><a name="en-us_topic_0175230221_p29543677185440"></a>0x02001d08</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p5187305519220"><a name="en-us_topic_0175230221_p5187305519220"></a><a name="en-us_topic_0175230221_p5187305519220"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p25666808185440"><a name="en-us_topic_0175230221_p25666808185440"></a><a name="en-us_topic_0175230221_p25666808185440"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row29674680185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p54838841185440"><a name="en-us_topic_0175230221_p54838841185440"></a><a name="en-us_topic_0175230221_p54838841185440"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p12761131185440"><a name="en-us_topic_0175230221_p12761131185440"></a><a name="en-us_topic_0175230221_p12761131185440"></a>LOS_ERRNO_MUX_PENDED</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p27018677185440"><a name="en-us_topic_0175230221_p27018677185440"></a><a name="en-us_topic_0175230221_p27018677185440"></a>0x02001d09</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p41029248185440"><a name="en-us_topic_0175230221_p41029248185440"></a><a name="en-us_topic_0175230221_p41029248185440"></a>Delete a mutex that is being used.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p35034804185440"><a name="en-us_topic_0175230221_p35034804185440"></a><a name="en-us_topic_0175230221_p35034804185440"></a>Delete the mutex after it is released.</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row46877780185440"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p39003831185440"><a name="en-us_topic_0175230221_p39003831185440"></a><a name="en-us_topic_0175230221_p39003831185440"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p5193751185440"><a name="en-us_topic_0175230221_p5193751185440"></a><a name="en-us_topic_0175230221_p5193751185440"></a>LOS_ERRNO_MUX_GET_COUNT_ERR</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p18040667185440"><a name="en-us_topic_0175230221_p18040667185440"></a><a name="en-us_topic_0175230221_p18040667185440"></a>0x02001d0a</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p902268419229"><a name="en-us_topic_0175230221_p902268419229"></a><a name="en-us_topic_0175230221_p902268419229"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p51891633185440"><a name="en-us_topic_0175230221_p51891633185440"></a><a name="en-us_topic_0175230221_p51891633185440"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row41674524185752"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p20193313185752"><a name="en-us_topic_0175230221_p20193313185752"></a><a name="en-us_topic_0175230221_p20193313185752"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p25045679185752"><a name="en-us_topic_0175230221_p25045679185752"></a><a name="en-us_topic_0175230221_p25045679185752"></a>LOS_ERRNO_MUX_REG_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p15434093185752"><a name="en-us_topic_0175230221_p15434093185752"></a><a name="en-us_topic_0175230221_p15434093185752"></a>0x02001d0b</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p42201987185752"><a name="en-us_topic_0175230221_p42201987185752"></a><a name="en-us_topic_0175230221_p42201987185752"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p62917786185752"><a name="en-us_topic_0175230221_p62917786185752"></a><a name="en-us_topic_0175230221_p62917786185752"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230221_row16448413194410"><td class="cellrowborder" valign="top" width="6.18%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p4448121364413"><a name="en-us_topic_0175230221_p4448121364413"></a><a name="en-us_topic_0175230221_p4448121364413"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="21.05%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p64488131440"><a name="en-us_topic_0175230221_p64488131440"></a><a name="en-us_topic_0175230221_p64488131440"></a>LOS_ERRNO_MUX_PEND_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="12.25%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p12448613144412"><a name="en-us_topic_0175230221_p12448613144412"></a><a name="en-us_topic_0175230221_p12448613144412"></a>0x02001d0c</p>
</td>
<td class="cellrowborder" valign="top" width="27.02%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p18448141317446"><a name="en-us_topic_0175230221_p18448141317446"></a><a name="en-us_topic_0175230221_p18448141317446"></a>Mutexes are obtained from system tasks, including idle and software timer tasks.</p>
</td>
<td class="cellrowborder" valign="top" width="33.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p244816137442"><a name="en-us_topic_0175230221_p244816137442"></a><a name="en-us_topic_0175230221_p244816137442"></a>Do not request mutexes from system tasks.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the mutex module, and the value is 0x1d.

## Development Process<a name="en-us_topic_0175230221_section9569749113453"></a>

The development process of mutexes in typical scenarios is as follows: 

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Mutex**  to configure the mutex module.

    <a name="en-us_topic_0175230221_table1016571215273"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230221_row716519123275"><th class="cellrowborder" valign="top" width="20.96%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230221_p50963290171025"><a name="en-us_topic_0175230221_p50963290171025"></a><a name="en-us_topic_0175230221_p50963290171025"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="33.95%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230221_p34385849171025"><a name="en-us_topic_0175230221_p34385849171025"></a><a name="en-us_topic_0175230221_p34385849171025"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="12.770000000000001%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230221_p19411171414538"><a name="en-us_topic_0175230221_p19411171414538"></a><a name="en-us_topic_0175230221_p19411171414538"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="11.25%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230221_p52666268171025"><a name="en-us_topic_0175230221_p52666268171025"></a><a name="en-us_topic_0175230221_p52666268171025"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="21.07%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230221_p38109296171025"><a name="en-us_topic_0175230221_p38109296171025"></a><a name="en-us_topic_0175230221_p38109296171025"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230221_row4836101282912"><td class="cellrowborder" valign="top" width="20.96%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p58229317162733"><a name="en-us_topic_0175230221_p58229317162733"></a><a name="en-us_topic_0175230221_p58229317162733"></a>LOSCFG_BASE_IPC_MUX</p>
    </td>
    <td class="cellrowborder" valign="top" width="33.95%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p18954268162733"><a name="en-us_topic_0175230221_p18954268162733"></a><a name="en-us_topic_0175230221_p18954268162733"></a>Whether to enable the tailoring function of the mutex module.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.770000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p58900702162733"><a name="en-us_topic_0175230221_p58900702162733"></a><a name="en-us_topic_0175230221_p58900702162733"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.25%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p6227573162733"><a name="en-us_topic_0175230221_p6227573162733"></a><a name="en-us_topic_0175230221_p6227573162733"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p34671433162733"><a name="en-us_topic_0175230221_p34671433162733"></a><a name="en-us_topic_0175230221_p34671433162733"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230221_row4165181211278"><td class="cellrowborder" valign="top" width="20.96%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p123891942105715"><a name="en-us_topic_0175230221_p123891942105715"></a><a name="en-us_topic_0175230221_p123891942105715"></a>LOSCFG_MUTEX_WAITMODE_PRIO</p>
    </td>
    <td class="cellrowborder" valign="top" width="33.95%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p11216088171057"><a name="en-us_topic_0175230221_p11216088171057"></a><a name="en-us_topic_0175230221_p11216088171057"></a>Task priority-based waiting mode for mutexes.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.770000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p1319010382532"><a name="en-us_topic_0175230221_p1319010382532"></a><a name="en-us_topic_0175230221_p1319010382532"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.25%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p56826855171025"><a name="en-us_topic_0175230221_p56826855171025"></a><a name="en-us_topic_0175230221_p56826855171025"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p39572571171025"><a name="en-us_topic_0175230221_p39572571171025"></a><a name="en-us_topic_0175230221_p39572571171025"></a>LOSCFG_BASE_IPC_MUX</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230221_row41651012172712"><td class="cellrowborder" valign="top" width="20.96%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p17335125112573"><a name="en-us_topic_0175230221_p17335125112573"></a><a name="en-us_topic_0175230221_p17335125112573"></a>LOSCFG_MUTEX_WAITMODE_FIFO</p>
    </td>
    <td class="cellrowborder" valign="top" width="33.95%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p6335105115711"><a name="en-us_topic_0175230221_p6335105115711"></a><a name="en-us_topic_0175230221_p6335105115711"></a>FIFO-based waiting mode for mutexes.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.770000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p5335135195717"><a name="en-us_topic_0175230221_p5335135195717"></a><a name="en-us_topic_0175230221_p5335135195717"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.25%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p14335251155712"><a name="en-us_topic_0175230221_p14335251155712"></a><a name="en-us_topic_0175230221_p14335251155712"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p13335135115579"><a name="en-us_topic_0175230221_p13335135115579"></a><a name="en-us_topic_0175230221_p13335135115579"></a>LOSCFG_BASE_IPC_MUX</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230221_row1716511232718"><td class="cellrowborder" valign="top" width="20.96%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230221_p57239002171025"><a name="en-us_topic_0175230221_p57239002171025"></a><a name="en-us_topic_0175230221_p57239002171025"></a>LOSCFG_BASE_IPC_MUX_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="33.95%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230221_p15841531294"><a name="en-us_topic_0175230221_p15841531294"></a><a name="en-us_topic_0175230221_p15841531294"></a>Maximum number of mutexes supported by the system.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.770000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230221_p13411111415537"><a name="en-us_topic_0175230221_p13411111415537"></a><a name="en-us_topic_0175230221_p13411111415537"></a>&lt; 65535</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.25%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230221_p47029099171025"><a name="en-us_topic_0175230221_p47029099171025"></a><a name="en-us_topic_0175230221_p47029099171025"></a>1024</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230221_p51260679171025"><a name="en-us_topic_0175230221_p51260679171025"></a><a name="en-us_topic_0175230221_p51260679171025"></a>LOSCFG_BASE_IPC_MUX</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Call LOS\_MuxCreate to create a mutex.
3.  Call LOS\_MuxPend to request a mutex.
4.  Call LOS\_MuxPost to release a mutex.
5.  Call LOS\_MuxDelete to delete a mutex.

## Platform Differences<a name="en-us_topic_0175230221_section32521403144937"></a>

None


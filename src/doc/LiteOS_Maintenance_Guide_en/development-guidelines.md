# Development Guidelines<a name="EN-US_TOPIC_0312244421"></a>

## Usage Scenarios<a name="en-us_topic_0175230412_section42233910171524"></a>

Check whether the current system load exceeds the designed specifications based on the system CPU usage.

Check whether the CPU usage of each task in the system meets the designed requirements based on the CPU usage of each task.

## Functions<a name="en-us_topic_0175230412_section3382358420811"></a>

The CPU usage module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

**Table  1**  Functions

<a name="en-us_topic_0175230412_table423153820811"></a>
<table><thead align="left"><tr id="en-us_topic_0175230412_row1872972720811"><th class="cellrowborder" valign="top" width="21.32786721327867%" id="mcps1.2.4.1.1"><p id="en-us_topic_0175230412_p2197741720811"><a name="en-us_topic_0175230412_p2197741720811"></a><a name="en-us_topic_0175230412_p2197741720811"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="21.767823217678234%" id="mcps1.2.4.1.2"><p id="en-us_topic_0175230412_p1280752020811"><a name="en-us_topic_0175230412_p1280752020811"></a><a name="en-us_topic_0175230412_p1280752020811"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="56.9043095690431%" id="mcps1.2.4.1.3"><p id="en-us_topic_0175230412_p3595409320811"><a name="en-us_topic_0175230412_p3595409320811"></a><a name="en-us_topic_0175230412_p3595409320811"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230412_row3156887520811"><td class="cellrowborder" valign="top" width="21.32786721327867%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230412_p534411020433"><a name="en-us_topic_0175230412_p534411020433"></a><a name="en-us_topic_0175230412_p534411020433"></a>Obtaining the system CPU usage</p>
</td>
<td class="cellrowborder" valign="top" width="21.767823217678234%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230412_p6009666520811"><a name="en-us_topic_0175230412_p6009666520811"></a><a name="en-us_topic_0175230412_p6009666520811"></a>LOS_HistorySysCpuUsage</p>
</td>
<td class="cellrowborder" valign="top" width="56.9043095690431%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230412_p6683076820811"><a name="en-us_topic_0175230412_p6683076820811"></a><a name="en-us_topic_0175230412_p6683076820811"></a>Obtains the system CPU usage, excluding the CPU usage of idle tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row749511820811"><td class="cellrowborder" rowspan="2" valign="top" width="21.32786721327867%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230412_p157261813435"><a name="en-us_topic_0175230412_p157261813435"></a><a name="en-us_topic_0175230412_p157261813435"></a>Obtaining the task CPU usage</p>
</td>
<td class="cellrowborder" valign="top" width="21.767823217678234%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230412_p30313106201548"><a name="en-us_topic_0175230412_p30313106201548"></a><a name="en-us_topic_0175230412_p30313106201548"></a>LOS_HistoryTaskCpuUsage</p>
</td>
<td class="cellrowborder" valign="top" width="56.9043095690431%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230412_p6481168620811"><a name="en-us_topic_0175230412_p6481168620811"></a><a name="en-us_topic_0175230412_p6481168620811"></a>Obtains the CPU usage of the specified interrupt (by entering the interrupt ID) after LOSCFG_CPUP_INCLUDE_IRQ is enabled; or obtains the CPU usage of a specified task when LOSCFG_CPUP_INCLUDE_IRQ is disabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row11187400233110"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230412_p50546033233110"><a name="en-us_topic_0175230412_p50546033233110"></a><a name="en-us_topic_0175230412_p50546033233110"></a>LOS_AllCpuUsage</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230412_p1551115616178"><a name="en-us_topic_0175230412_p1551115616178"></a><a name="en-us_topic_0175230412_p1551115616178"></a>Obtains the CPU usage of all interrupts, when LOSCFG_CPUP_INCLUDE_IRQ is enabled and the input parameter flag is set to 0. Set the input parameter flag to a non-zero value or disable LOSCFG_CPUP_INCLUDE_IRQ to obtain the CPU usage of all tasks, including idle tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row56789172171319"><td class="cellrowborder" valign="top" width="21.32786721327867%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230412_p36520229171319"><a name="en-us_topic_0175230412_p36520229171319"></a><a name="en-us_topic_0175230412_p36520229171319"></a>Resetting the CPU usage</p>
</td>
<td class="cellrowborder" valign="top" width="21.767823217678234%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230412_p5348574171319"><a name="en-us_topic_0175230412_p5348574171319"></a><a name="en-us_topic_0175230412_p5348574171319"></a>LOS_CpupReset</p>
</td>
<td class="cellrowborder" valign="top" width="56.9043095690431%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230412_p30581384171319"><a name="en-us_topic_0175230412_p30581384171319"></a><a name="en-us_topic_0175230412_p30581384171319"></a>Resets the CPU usage data, including the CPU usage of the system and tasks.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>1. The CPU usage obtained using the preceding APIs is a permillage. Therefore, the valid range of the CPU usage is 0 to 1000. The system CPU usage is 1000, indicating that the system is fully loaded. The CPU usage of the task is 1000, indicating that the task keeps running during a period of time.
>2. The CPU usage of a task can be obtained in three modes, which are specified by the input parameter mode.
>    -   CPUP\_LAST\_TEN\_SECONDS \(the value is 0\): indicates that the CPU usage in the latest 10 seconds is obtained.
>    -   CPUP\_LAST\_ONE\_SECONDS \(the value is 1\): indicates that the CPU usage over the latest one second is obtained.
>    -   CPUP\_ALL\_TIME: indicates the CPU usage since the system starts. The value is 0xffff or any other value except 0 and 1.

## CPUP Error Codes<a name="en-us_topic_0175230412_section560718304572"></a>

<a name="en-us_topic_0175230412_table66521465191749"></a>
<table><thead align="left"><tr id="en-us_topic_0175230412_row66647056191749"><th class="cellrowborder" valign="top" width="6.827936012485369%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230412_p65995609191749"><a name="en-us_topic_0175230412_p65995609191749"></a><a name="en-us_topic_0175230412_p65995609191749"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="21.2348809988295%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230412_p44044076191749"><a name="en-us_topic_0175230412_p44044076191749"></a><a name="en-us_topic_0175230412_p44044076191749"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="10.280920795942256%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230412_p10800441191749"><a name="en-us_topic_0175230412_p10800441191749"></a><a name="en-us_topic_0175230412_p10800441191749"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="29.545454545454547%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230412_p39597633191844"><a name="en-us_topic_0175230412_p39597633191844"></a><a name="en-us_topic_0175230412_p39597633191844"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="32.11080764728834%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230412_p61844251191749"><a name="en-us_topic_0175230412_p61844251191749"></a><a name="en-us_topic_0175230412_p61844251191749"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230412_row134571758181017"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p4487040819218"><a name="en-us_topic_0175230412_p4487040819218"></a><a name="en-us_topic_0175230412_p4487040819218"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p951161311811"><a name="en-us_topic_0175230412_p951161311811"></a><a name="en-us_topic_0175230412_p951161311811"></a>LOS_ERRNO_CPUP_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p16806186191017"><a name="en-us_topic_0175230412_p16806186191017"></a><a name="en-us_topic_0175230412_p16806186191017"></a>0x02001e00</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p3955882919926"><a name="en-us_topic_0175230412_p3955882919926"></a><a name="en-us_topic_0175230412_p3955882919926"></a>The memory is insufficient during initialization of the CPU usage module.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p60375397191252"><a name="en-us_topic_0175230412_p60375397191252"></a><a name="en-us_topic_0175230412_p60375397191252"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for the CPU usage module.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row151402056115819"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p18369922191749"><a name="en-us_topic_0175230412_p18369922191749"></a><a name="en-us_topic_0175230412_p18369922191749"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p1332621616820"><a name="en-us_topic_0175230412_p1332621616820"></a><a name="en-us_topic_0175230412_p1332621616820"></a>LOS_ERRNO_CPUP_TASK_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p6826106121015"><a name="en-us_topic_0175230412_p6826106121015"></a><a name="en-us_topic_0175230412_p6826106121015"></a>0x02001e01</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p1732013725019"><a name="en-us_topic_0175230412_p1732013725019"></a><a name="en-us_topic_0175230412_p1732013725019"></a>The input parameters contain at least one null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p1314075613583"><a name="en-us_topic_0175230412_p1314075613583"></a><a name="en-us_topic_0175230412_p1314075613583"></a>Set the pointers correctly.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row6482175315584"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p20814974191749"><a name="en-us_topic_0175230412_p20814974191749"></a><a name="en-us_topic_0175230412_p20814974191749"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p1353114131819"><a name="en-us_topic_0175230412_p1353114131819"></a><a name="en-us_topic_0175230412_p1353114131819"></a>LOS_ERRNO_CPUP_NO_INIT</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p98261367102"><a name="en-us_topic_0175230412_p98261367102"></a><a name="en-us_topic_0175230412_p98261367102"></a>0x02001e02</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p14547152614018"><a name="en-us_topic_0175230412_p14547152614018"></a><a name="en-us_topic_0175230412_p14547152614018"></a>The CPU usage module is not initialized.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p11482135313584"><a name="en-us_topic_0175230412_p11482135313584"></a><a name="en-us_topic_0175230412_p11482135313584"></a>Initialize the CPU usage module before using it.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row193514965819"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p64337369191749"><a name="en-us_topic_0175230412_p64337369191749"></a><a name="en-us_topic_0175230412_p64337369191749"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p1853111137813"><a name="en-us_topic_0175230412_p1853111137813"></a><a name="en-us_topic_0175230412_p1853111137813"></a>LOS_ERRNO_CPUP_MAXNUM_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p682520620109"><a name="en-us_topic_0175230412_p682520620109"></a><a name="en-us_topic_0175230412_p682520620109"></a>0x02001e03</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p181431132184915"><a name="en-us_topic_0175230412_p181431132184915"></a><a name="en-us_topic_0175230412_p181431132184915"></a>The input parameter (maximum number of threads or interrupts) set for the LOS_AllCpuUsage API is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p2744733101114"><a name="en-us_topic_0175230412_p2744733101114"></a><a name="en-us_topic_0175230412_p2744733101114"></a>Enter a correct maximum number of threads or interrupts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row6517730619218"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p6877374191749"><a name="en-us_topic_0175230412_p6877374191749"></a><a name="en-us_topic_0175230412_p6877374191749"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p25313131813"><a name="en-us_topic_0175230412_p25313131813"></a><a name="en-us_topic_0175230412_p25313131813"></a>LOS_ERRNO_CPUP_THREAD_NO_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p1082517610102"><a name="en-us_topic_0175230412_p1082517610102"></a><a name="en-us_topic_0175230412_p1082517610102"></a>0x02001e04</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p1677174015619"><a name="en-us_topic_0175230412_p1677174015619"></a><a name="en-us_topic_0175230412_p1677174015619"></a>When the CPU usage of a specified task is obtained, the task does not exist.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p1372919138224"><a name="en-us_topic_0175230412_p1372919138224"></a><a name="en-us_topic_0175230412_p1372919138224"></a>The system can only collect statistics on the CPU usage of the tasks that have been created. Check whether the task corresponding to the entered task ID has been created.</p>
</td>
</tr>
<tr id="en-us_topic_0175230412_row33366968191749"><td class="cellrowborder" valign="top" width="6.827936012485369%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p44562467191749"><a name="en-us_topic_0175230412_p44562467191749"></a><a name="en-us_topic_0175230412_p44562467191749"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.2348809988295%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p1353051315819"><a name="en-us_topic_0175230412_p1353051315819"></a><a name="en-us_topic_0175230412_p1353051315819"></a>LOS_ERRNO_CPUP_TSK_ID_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="10.280920795942256%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p178242614103"><a name="en-us_topic_0175230412_p178242614103"></a><a name="en-us_topic_0175230412_p178242614103"></a>0x02001e05</p>
</td>
<td class="cellrowborder" valign="top" width="29.545454545454547%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p9238164164714"><a name="en-us_topic_0175230412_p9238164164714"></a><a name="en-us_topic_0175230412_p9238164164714"></a>An invalid task ID is set when the API for obtaining the CPU usage of a specified task is called.</p>
</td>
<td class="cellrowborder" valign="top" width="32.11080764728834%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p5424607519267"><a name="en-us_topic_0175230412_p5424607519267"></a><a name="en-us_topic_0175230412_p5424607519267"></a>Check whether the set task ID is correct.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](../LiteOS_Kernel_Developer_Guide_en/development-guidelines-14.md). Bits 8–15 belong to the CPUP module, and the value is 0x1e.

## Development Process<a name="en-us_topic_0175230412_section5280239320811"></a>

The typical development process of the CPU usage is as follows.

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Extend Kernel**  \>  **Enable Cpup**  to configure the CPU usage module.

    <a name="en-us_topic_0175230412_table06655375130"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230412_row8665203751318"><th class="cellrowborder" valign="top" width="20.910000000000004%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230412_p61687296155221"><a name="en-us_topic_0175230412_p61687296155221"></a><a name="en-us_topic_0175230412_p61687296155221"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="37.82%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230412_p25007692155221"><a name="en-us_topic_0175230412_p25007692155221"></a><a name="en-us_topic_0175230412_p25007692155221"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="10.730000000000002%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230412_p47081329204415"><a name="en-us_topic_0175230412_p47081329204415"></a><a name="en-us_topic_0175230412_p47081329204415"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="9.280000000000001%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230412_p7383544155221"><a name="en-us_topic_0175230412_p7383544155221"></a><a name="en-us_topic_0175230412_p7383544155221"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="21.260000000000005%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230412_p34917797155221"><a name="en-us_topic_0175230412_p34917797155221"></a><a name="en-us_topic_0175230412_p34917797155221"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230412_row9665837161320"><td class="cellrowborder" valign="top" width="20.910000000000004%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p3665143710130"><a name="en-us_topic_0175230412_p3665143710130"></a><a name="en-us_topic_0175230412_p3665143710130"></a>LOSCFG_KERNEL_CPUP</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p13665737191318"><a name="en-us_topic_0175230412_p13665737191318"></a><a name="en-us_topic_0175230412_p13665737191318"></a>Tailoring switch of the CPUP module</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.730000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p19665193721319"><a name="en-us_topic_0175230412_p19665193721319"></a><a name="en-us_topic_0175230412_p19665193721319"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="9.280000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p1866573713131"><a name="en-us_topic_0175230412_p1866573713131"></a><a name="en-us_topic_0175230412_p1866573713131"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.260000000000005%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p1266519378138"><a name="en-us_topic_0175230412_p1266519378138"></a><a name="en-us_topic_0175230412_p1266519378138"></a>LOSCFG_KERNEL_EXTKERNEL</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230412_row7665183761312"><td class="cellrowborder" valign="top" width="20.910000000000004%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230412_p666523715137"><a name="en-us_topic_0175230412_p666523715137"></a><a name="en-us_topic_0175230412_p666523715137"></a>LOSCFG_CPUP_INCLUDE_IRQ</p>
    </td>
    <td class="cellrowborder" valign="top" width="37.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230412_p766543721311"><a name="en-us_topic_0175230412_p766543721311"></a><a name="en-us_topic_0175230412_p766543721311"></a>After this configuration item is enabled, the CPU usage of interrupts can be obtained from the LOS_HistoryTaskCpuUsage and LOS_AllCpuUsage APIs. If this configuration item is disabled, only the CPU usage of tasks can be obtained.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.730000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230412_p9665183731312"><a name="en-us_topic_0175230412_p9665183731312"></a><a name="en-us_topic_0175230412_p9665183731312"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="9.280000000000001%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230412_p116651837181311"><a name="en-us_topic_0175230412_p116651837181311"></a><a name="en-us_topic_0175230412_p116651837181311"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.260000000000005%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230412_p1066517373133"><a name="en-us_topic_0175230412_p1066517373133"></a><a name="en-us_topic_0175230412_p1066517373133"></a>LOSCFG_KERNEL_CPUP</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Obtain the system CPU usage LOS\_HistorySysCpuUsage
3.  Obtain the CPU usage of a specified task or interrupt LOS\_HistoryTaskCpuUsage.
4.  Obtain the CPU usage of all tasks or interrupts LOS\_AllCpuUsage.

## Platform Differences<a name="en-us_topic_0175230412_section3340376820811"></a>

None


# Development Guidelines<a name="EN-US_TOPIC_0311018437"></a>

## Usage Scenarios<a name="en-us_topic_0175230408_section25528897165014"></a>

Read the topic when you want to learn more about system time and conversion between ticks and seconds/milliseconds.

## Functions<a name="en-us_topic_0175230408_section5599199614853"></a>

The time management module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230408_table1597633114853"></a>
<table><thead align="left"><tr id="en-us_topic_0175230408_row6547625614853"><th class="cellrowborder" valign="top" width="17.07%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230408_p197652314853"><a name="en-us_topic_0175230408_p197652314853"></a><a name="en-us_topic_0175230408_p197652314853"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="18.68%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230408_p3159926114853"><a name="en-us_topic_0175230408_p3159926114853"></a><a name="en-us_topic_0175230408_p3159926114853"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="64.25%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230408_p1752099814853"><a name="en-us_topic_0175230408_p1752099814853"></a><a name="en-us_topic_0175230408_p1752099814853"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230408_row37761567115936"><td class="cellrowborder" rowspan="2" valign="top" width="17.07%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p66321365112810"><a name="en-us_topic_0175230408_p66321365112810"></a><a name="en-us_topic_0175230408_p66321365112810"></a>Time conversion</p>
</td>
<td class="cellrowborder" valign="top" width="18.68%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p38788111115936"><a name="en-us_topic_0175230408_p38788111115936"></a><a name="en-us_topic_0175230408_p38788111115936"></a>LOS_MS2Tick</p>
</td>
<td class="cellrowborder" valign="top" width="64.25%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230408_p54829304115936"><a name="en-us_topic_0175230408_p54829304115936"></a><a name="en-us_topic_0175230408_p54829304115936"></a>Converts milliseconds into ticks</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row2021456214853"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p2676682114853"><a name="en-us_topic_0175230408_p2676682114853"></a><a name="en-us_topic_0175230408_p2676682114853"></a>LOS_Tick2MS</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p2062890114853"><a name="en-us_topic_0175230408_p2062890114853"></a><a name="en-us_topic_0175230408_p2062890114853"></a>Converts ticks into milliseconds</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row4646930012230"><td class="cellrowborder" rowspan="4" valign="top" width="17.07%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p54265225145715"><a name="en-us_topic_0175230408_p54265225145715"></a><a name="en-us_topic_0175230408_p54265225145715"></a>Time statistics</p>
</td>
<td class="cellrowborder" valign="top" width="18.68%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p951174812230"><a name="en-us_topic_0175230408_p951174812230"></a><a name="en-us_topic_0175230408_p951174812230"></a>LOS_CyclePerTickGet</p>
</td>
<td class="cellrowborder" valign="top" width="64.25%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230408_p3225415312230"><a name="en-us_topic_0175230408_p3225415312230"></a><a name="en-us_topic_0175230408_p3225415312230"></a>Number of cycles for each tick.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row5082319714571"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p33407133145715"><a name="en-us_topic_0175230408_p33407133145715"></a><a name="en-us_topic_0175230408_p33407133145715"></a>LOS_TickCountGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p21623253145715"><a name="en-us_topic_0175230408_p21623253145715"></a><a name="en-us_topic_0175230408_p21623253145715"></a>Obtains the number of ticks since the system starts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row155873165506"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p16587171610509"><a name="en-us_topic_0175230408_p16587171610509"></a><a name="en-us_topic_0175230408_p16587171610509"></a>LOS_GetCpuCycle</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p1758741635020"><a name="en-us_topic_0175230408_p1758741635020"></a><a name="en-us_topic_0175230408_p1758741635020"></a>Obtains the number of cycles since the system starts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row01423075018"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p15146306504"><a name="en-us_topic_0175230408_p15146306504"></a><a name="en-us_topic_0175230408_p15146306504"></a>LOS_CurrNanosec</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p1314173035019"><a name="en-us_topic_0175230408_p1314173035019"></a><a name="en-us_topic_0175230408_p1314173035019"></a>Obtains the number of nanoseconds since the system starts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row39991813135113"><td class="cellrowborder" rowspan="2" valign="top" width="17.07%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p164791723145114"><a name="en-us_topic_0175230408_p164791723145114"></a><a name="en-us_topic_0175230408_p164791723145114"></a>Delay management</p>
</td>
<td class="cellrowborder" valign="top" width="18.68%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p12999113115113"><a name="en-us_topic_0175230408_p12999113115113"></a><a name="en-us_topic_0175230408_p12999113115113"></a>LOS_Udelay</p>
</td>
<td class="cellrowborder" valign="top" width="64.25%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230408_p7999111315513"><a name="en-us_topic_0175230408_p7999111315513"></a><a name="en-us_topic_0175230408_p7999111315513"></a>Busy waiting in μs. Resources can be preempted by another task with a higher priority.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row1195310175513"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230408_p12953417125120"><a name="en-us_topic_0175230408_p12953417125120"></a><a name="en-us_topic_0175230408_p12953417125120"></a>LOS_Mdelay</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230408_p1195351765110"><a name="en-us_topic_0175230408_p1195351765110"></a><a name="en-us_topic_0175230408_p1195351765110"></a>Busy waiting in ms. Resources can be preempted by another task with a higher priority.</p>
</td>
</tr>
</tbody>
</table>

## Time Management Error Codes<a name="en-us_topic_0175230408_section1387861718220"></a>

Error codes are returned if errors occur during time conversion. The error codes can facilitate fault locating.

<a name="en-us_topic_0175230408_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230408_row2267197395642"><th class="cellrowborder" valign="top" width="5.65%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230408_p1908783195642"><a name="en-us_topic_0175230408_p1908783195642"></a><a name="en-us_topic_0175230408_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="22.41%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230408_p261046995642"><a name="en-us_topic_0175230408_p261046995642"></a><a name="en-us_topic_0175230408_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="12.15%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230408_p1012144095642"><a name="en-us_topic_0175230408_p1012144095642"></a><a name="en-us_topic_0175230408_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="20.72%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230408_p1453028795642"><a name="en-us_topic_0175230408_p1453028795642"></a><a name="en-us_topic_0175230408_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="39.07%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230408_p2753561710026"><a name="en-us_topic_0175230408_p2753561710026"></a><a name="en-us_topic_0175230408_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230408_row6366372295642"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230408_p5648782795642"><a name="en-us_topic_0175230408_p5648782795642"></a><a name="en-us_topic_0175230408_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="22.41%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230408_p1211123695642"><a name="en-us_topic_0175230408_p1211123695642"></a><a name="en-us_topic_0175230408_p1211123695642"></a>LOS_ERRNO_TICK_CFG_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="12.15%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230408_p74295235817"><a name="en-us_topic_0175230408_p74295235817"></a><a name="en-us_topic_0175230408_p74295235817"></a>0x02000400</p>
</td>
<td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230408_p492720095642"><a name="en-us_topic_0175230408_p492720095642"></a><a name="en-us_topic_0175230408_p492720095642"></a>The system tick configuration is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="39.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230408_p1579250110026"><a name="en-us_topic_0175230408_p1579250110026"></a><a name="en-us_topic_0175230408_p1579250110026"></a>Configure a valid system main clock frequency <strong id="en-us_topic_0175230408_b99873172392"><a name="en-us_topic_0175230408_b99873172392"></a><a name="en-us_topic_0175230408_b99873172392"></a>OS_SYS_CLOCK</strong> when configuring board-level adaption, and run the <strong id="en-us_topic_0175230408_b51738874416"><a name="en-us_topic_0175230408_b51738874416"></a><a name="en-us_topic_0175230408_b51738874416"></a>make menuconfig</strong> command to configure <strong id="en-us_topic_0175230408_b15991121054415"><a name="en-us_topic_0175230408_b15991121054415"></a><a name="en-us_topic_0175230408_b15991121054415"></a>LOSCFG_BASE_CORE_TICK_PER_SECOND</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row557969859212"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230408_p232619649212"><a name="en-us_topic_0175230408_p232619649212"></a><a name="en-us_topic_0175230408_p232619649212"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="22.41%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230408_p51709119212"><a name="en-us_topic_0175230408_p51709119212"></a><a name="en-us_topic_0175230408_p51709119212"></a>LOS_ERRNO_TICK_NO_HWTIMER</p>
</td>
<td class="cellrowborder" valign="top" width="12.15%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230408_p161906189212"><a name="en-us_topic_0175230408_p161906189212"></a><a name="en-us_topic_0175230408_p161906189212"></a>0x02000401</p>
</td>
<td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230408_p363716869212"><a name="en-us_topic_0175230408_p363716869212"></a><a name="en-us_topic_0175230408_p363716869212"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="39.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230408_p604254509212"><a name="en-us_topic_0175230408_p604254509212"></a><a name="en-us_topic_0175230408_p604254509212"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230408_row194798692831"><td class="cellrowborder" valign="top" width="5.65%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230408_p2356914592831"><a name="en-us_topic_0175230408_p2356914592831"></a><a name="en-us_topic_0175230408_p2356914592831"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="22.41%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230408_p3005261392831"><a name="en-us_topic_0175230408_p3005261392831"></a><a name="en-us_topic_0175230408_p3005261392831"></a>LOS_ERRNO_TICK_PER_SEC_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="12.15%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230408_p22084010581"><a name="en-us_topic_0175230408_p22084010581"></a><a name="en-us_topic_0175230408_p22084010581"></a>0x02000402</p>
</td>
<td class="cellrowborder" valign="top" width="20.72%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230408_p935741992831"><a name="en-us_topic_0175230408_p935741992831"></a><a name="en-us_topic_0175230408_p935741992831"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="39.07%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230408_p1975349392831"><a name="en-us_topic_0175230408_p1975349392831"></a><a name="en-us_topic_0175230408_p1975349392831"></a>-</p>
</td>
</tr>
</tbody>
</table>

## Development Process<a name="en-us_topic_0175230408_section51115536141116"></a>

The typical time management development process is as follows:

1.  Determine whether to enable the  **LOSCFG\_BASE\_CORE\_TICK\_HW\_TIME**  macro based on site requirements to select an external timer and configure the system main clock frequency  **OS\_SYS\_CLOCK**  \(in Hz\) when configuring broad-level adaption. The default value of  **OS\_SYS\_CLOCK**  is configured based on the hardware platform.
2.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Basic Config**  \>  **Task**  to configure LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND.

    <a name="en-us_topic_0175230408_table1124543155750"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230408_row32017475155750"><th class="cellrowborder" valign="top" width="27.400000000000002%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230408_p15912448155750"><a name="en-us_topic_0175230408_p15912448155750"></a><a name="en-us_topic_0175230408_p15912448155750"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="27.12%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230408_p13839897155750"><a name="en-us_topic_0175230408_p13839897155750"></a><a name="en-us_topic_0175230408_p13839897155750"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="17.02%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230408_p47289871155750"><a name="en-us_topic_0175230408_p47289871155750"></a><a name="en-us_topic_0175230408_p47289871155750"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.96%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230408_p5274313155750"><a name="en-us_topic_0175230408_p5274313155750"></a><a name="en-us_topic_0175230408_p5274313155750"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.5%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230408_p24566191155750"><a name="en-us_topic_0175230408_p24566191155750"></a><a name="en-us_topic_0175230408_p24566191155750"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230408_row58661230155951"><td class="cellrowborder" valign="top" width="27.400000000000002%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230408_p53939171155951"><a name="en-us_topic_0175230408_p53939171155951"></a><a name="en-us_topic_0175230408_p53939171155951"></a>LOSCFG_BASE_CORE_TICK_PER_SECOND</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230408_p6996745155951"><a name="en-us_topic_0175230408_p6996745155951"></a><a name="en-us_topic_0175230408_p6996745155951"></a>Number of ticks per second.</p>
    </td>
    <td class="cellrowborder" valign="top" width="17.02%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230408_p7058646112722"><a name="en-us_topic_0175230408_p7058646112722"></a><a name="en-us_topic_0175230408_p7058646112722"></a>(0, 1000]</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.96%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230408_p3184774155951"><a name="en-us_topic_0175230408_p3184774155951"></a><a name="en-us_topic_0175230408_p3184774155951"></a>100</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230408_p56640174155951"><a name="en-us_topic_0175230408_p56640174155951"></a><a name="en-us_topic_0175230408_p56640174155951"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

3.  Call the clock conversion/statistics API.


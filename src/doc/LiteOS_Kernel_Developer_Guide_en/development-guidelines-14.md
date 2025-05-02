# Development Guidelines<a name="EN-US_TOPIC_0311018405"></a>

## Error Codes<a name="en-us_topic_0175230332_section29852515161"></a>

An error may occur when an API is called. In this case, the API returns the corresponding error code to help quickly locate the error cause.

An error code is a 32-bit unsigned integer. Bits 31 to 24 indicate the error level. Bits 23 to 16 indicate the error code flag \(the current value is  **0**\). Digits 15 to 8 indicate the module to which the error code belongs. Bits 7 to 0 indicate the sequence number of the error code.

**Table  1**  Error levels in an error code

<a name="en-us_topic_0175230332_table181167813596"></a>
<table><thead align="left"><tr id="en-us_topic_0175230332_row1911615812594"><th class="cellrowborder" valign="top" width="25.81%" id="mcps1.2.4.1.1"><p id="en-us_topic_0175230332_p911618115920"><a name="en-us_topic_0175230332_p911618115920"></a><a name="en-us_topic_0175230332_p911618115920"></a>Error Level</p>
</th>
<th class="cellrowborder" valign="top" width="22.62%" id="mcps1.2.4.1.2"><p id="en-us_topic_0175230332_p1411678165911"><a name="en-us_topic_0175230332_p1411678165911"></a><a name="en-us_topic_0175230332_p1411678165911"></a>Value</p>
</th>
<th class="cellrowborder" valign="top" width="51.57000000000001%" id="mcps1.2.4.1.3"><p id="en-us_topic_0175230332_p187391162018"><a name="en-us_topic_0175230332_p187391162018"></a><a name="en-us_topic_0175230332_p187391162018"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230332_row131165816596"><td class="cellrowborder" valign="top" width="25.81%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230332_p171169885914"><a name="en-us_topic_0175230332_p171169885914"></a><a name="en-us_topic_0175230332_p171169885914"></a>NORMAL</p>
</td>
<td class="cellrowborder" valign="top" width="22.62%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230332_p5116888596"><a name="en-us_topic_0175230332_p5116888596"></a><a name="en-us_topic_0175230332_p5116888596"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="51.57000000000001%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230332_p1373917161105"><a name="en-us_topic_0175230332_p1373917161105"></a><a name="en-us_topic_0175230332_p1373917161105"></a>Information</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row411615816594"><td class="cellrowborder" valign="top" width="25.81%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230332_p1011618820594"><a name="en-us_topic_0175230332_p1011618820594"></a><a name="en-us_topic_0175230332_p1011618820594"></a>WARN</p>
</td>
<td class="cellrowborder" valign="top" width="22.62%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230332_p711616810596"><a name="en-us_topic_0175230332_p711616810596"></a><a name="en-us_topic_0175230332_p711616810596"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="51.57000000000001%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230332_p19739816003"><a name="en-us_topic_0175230332_p19739816003"></a><a name="en-us_topic_0175230332_p19739816003"></a>Alert</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row91161682594"><td class="cellrowborder" valign="top" width="25.81%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230332_p191161388595"><a name="en-us_topic_0175230332_p191161388595"></a><a name="en-us_topic_0175230332_p191161388595"></a>ERR</p>
</td>
<td class="cellrowborder" valign="top" width="22.62%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230332_p1111668185914"><a name="en-us_topic_0175230332_p1111668185914"></a><a name="en-us_topic_0175230332_p1111668185914"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="51.57000000000001%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230332_p18739171610017"><a name="en-us_topic_0175230332_p18739171610017"></a><a name="en-us_topic_0175230332_p18739171610017"></a>Critical</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row19968728185914"><td class="cellrowborder" valign="top" width="25.81%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230332_p1969162845914"><a name="en-us_topic_0175230332_p1969162845914"></a><a name="en-us_topic_0175230332_p1969162845914"></a>FATAL</p>
</td>
<td class="cellrowborder" valign="top" width="22.62%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230332_p696982810596"><a name="en-us_topic_0175230332_p696982810596"></a><a name="en-us_topic_0175230332_p696982810596"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="51.57000000000001%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230332_p074011618010"><a name="en-us_topic_0175230332_p074011618010"></a><a name="en-us_topic_0175230332_p074011618010"></a>Fatal</p>
</td>
</tr>
</tbody>
</table>

For example, error code LOS\_ERRNO\_TSK\_NO\_MEMORY in the task module is defined as a fatal-level error, the module ID is LOS\_MOD\_TSK, and the error code sequence number is 0. The definition is as follows:

```
#define LOS_ERRNO_TSK_NO_MEMORY  LOS_ERRNO_OS_FATAL(LOS_MOD_TSK, 0x00)
```

```
#define LOS_ERRNO_OS_FATAL(MID, ERRNO)  \
    (LOS_ERRTYPE_FATAL | LOS_ERRNO_OS_ID | ((UINT32)(MID) << 8) | ((UINT32)(ERRNO)))
```

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>**LOS\_ERRTYPE\_FATAL**: The error level is FATAL and the value is  **0x03000000**.
>**LOS\_ERRNO\_OS\_ID**: error code flag. The value is  **0x000000**.
>**MID**: module. The value of  **LOS\_MOD\_TSK**  is  **0x2**.
>**ERRNO**: sequence number of an error code.
>Therefore, the value of  **LOS\_ERRNO\_TSK\_NO\_MEMORY**  is  **0x03000200**.

## Error Code Management<a name="en-us_topic_0175230332_section4791065162858"></a>

Sometimes, faults cannot be quickly and accurately located -using only error codes. To facilitate error analysis, the error handling module supports the registration of the hook function for error handling. When an error occurs, you can call the LOS\_ErrHandle API to execute the function for error handling.

The error handling module of Huawei LiteOS provides the following APIs. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230332_table3025663162858"></a>
<table><thead align="left"><tr id="en-us_topic_0175230332_row33230790162858"><th class="cellrowborder" valign="top" width="16.22837716228377%" id="mcps1.1.5.1.1"><p id="en-us_topic_0175230332_p57624058162858"><a name="en-us_topic_0175230332_p57624058162858"></a><a name="en-us_topic_0175230332_p57624058162858"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="24.607539246075394%" id="mcps1.1.5.1.2"><p id="en-us_topic_0175230332_p37037121162858"><a name="en-us_topic_0175230332_p37037121162858"></a><a name="en-us_topic_0175230332_p37037121162858"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="25.857414258574142%" id="mcps1.1.5.1.3"><p id="en-us_topic_0175230332_p10255125316586"><a name="en-us_topic_0175230332_p10255125316586"></a><a name="en-us_topic_0175230332_p10255125316586"></a><strong id="en-us_topic_0175230332_b533125564411"><a name="en-us_topic_0175230332_b533125564411"></a><a name="en-us_topic_0175230332_b533125564411"></a>Parameter</strong></p>
</th>
<th class="cellrowborder" valign="top" width="33.306669333066694%" id="mcps1.1.5.1.4"><p id="en-us_topic_0175230332_p817653416591"><a name="en-us_topic_0175230332_p817653416591"></a><a name="en-us_topic_0175230332_p817653416591"></a>Remarks</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230332_row19886134212579"><td class="cellrowborder" valign="top" width="16.22837716228377%" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p3886204219573"><a name="en-us_topic_0175230332_p3886204219573"></a><a name="en-us_topic_0175230332_p3886204219573"></a>LOS_RegErrHandle</p>
</td>
<td class="cellrowborder" valign="top" width="24.607539246075394%" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p8887142155718"><a name="en-us_topic_0175230332_p8887142155718"></a><a name="en-us_topic_0175230332_p8887142155718"></a>Registers the hook function for error handling.</p>
</td>
<td class="cellrowborder" valign="top" width="25.857414258574142%" headers="mcps1.1.5.1.3 "><p id="en-us_topic_0175230332_p182558531589"><a name="en-us_topic_0175230332_p182558531589"></a><a name="en-us_topic_0175230332_p182558531589"></a><strong id="en-us_topic_0175230332_b14584135954510"><a name="en-us_topic_0175230332_b14584135954510"></a><a name="en-us_topic_0175230332_b14584135954510"></a>func</strong>: hook function for error handling.</p>
</td>
<td class="cellrowborder" valign="top" width="33.306669333066694%" headers="mcps1.1.5.1.4 "><p id="en-us_topic_0175230332_p17176143414596"><a name="en-us_topic_0175230332_p17176143414596"></a><a name="en-us_topic_0175230332_p17176143414596"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row65700953153913"><td class="cellrowborder" rowspan="5" valign="top" width="16.22837716228377%" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p23722761153913"><a name="en-us_topic_0175230332_p23722761153913"></a><a name="en-us_topic_0175230332_p23722761153913"></a>LOS_ErrHandle</p>
</td>
<td class="cellrowborder" rowspan="5" valign="top" width="24.607539246075394%" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p42495518153913"><a name="en-us_topic_0175230332_p42495518153913"></a><a name="en-us_topic_0175230332_p42495518153913"></a>Calls the hook function to handle the error.</p>
</td>
<td class="cellrowborder" valign="top" width="25.857414258574142%" headers="mcps1.1.5.1.3 "><p id="en-us_topic_0175230332_p1125515536588"><a name="en-us_topic_0175230332_p1125515536588"></a><a name="en-us_topic_0175230332_p1125515536588"></a><strong id="en-us_topic_0175230332_b7778143134614"><a name="en-us_topic_0175230332_b7778143134614"></a><a name="en-us_topic_0175230332_b7778143134614"></a>fileName</strong>: name of the file that stores error logs.</p>
</td>
<td class="cellrowborder" valign="top" width="33.306669333066694%" headers="mcps1.1.5.1.4 "><p id="en-us_topic_0175230332_p10176183455917"><a name="en-us_topic_0175230332_p10176183455917"></a><a name="en-us_topic_0175230332_p10176183455917"></a>During internal calling, the input parameter is <strong id="en-us_topic_0175230332_b133441959174610"><a name="en-us_topic_0175230332_b133441959174610"></a><a name="en-us_topic_0175230332_b133441959174610"></a>os_unspecific_file</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row147465137018"><td class="cellrowborder" valign="top" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p1874711319013"><a name="en-us_topic_0175230332_p1874711319013"></a><a name="en-us_topic_0175230332_p1874711319013"></a><strong id="en-us_topic_0175230332_b17311217473"><a name="en-us_topic_0175230332_b17311217473"></a><a name="en-us_topic_0175230332_b17311217473"></a>lineNo</strong>: line number of the code where an error occurs.</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p37471513301"><a name="en-us_topic_0175230332_p37471513301"></a><a name="en-us_topic_0175230332_p37471513301"></a>If the value is <strong id="en-us_topic_0175230332_b97081415474"><a name="en-us_topic_0175230332_b97081415474"></a><a name="en-us_topic_0175230332_b97081415474"></a>0xa1b2c3f8</strong> during internal calling, the line number is not transferred.</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row853242816018"><td class="cellrowborder" valign="top" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p16532328904"><a name="en-us_topic_0175230332_p16532328904"></a><a name="en-us_topic_0175230332_p16532328904"></a><strong id="en-us_topic_0175230332_b14366931184710"><a name="en-us_topic_0175230332_b14366931184710"></a><a name="en-us_topic_0175230332_b14366931184710"></a>errnoNo</strong>: error code.</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p653219281502"><a name="en-us_topic_0175230332_p653219281502"></a><a name="en-us_topic_0175230332_p653219281502"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row158973181308"><td class="cellrowborder" valign="top" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p689771819017"><a name="en-us_topic_0175230332_p689771819017"></a><a name="en-us_topic_0175230332_p689771819017"></a><strong id="en-us_topic_0175230332_b19954183616473"><a name="en-us_topic_0175230332_b19954183616473"></a><a name="en-us_topic_0175230332_b19954183616473"></a>paraLen</strong>: length of the input parameter <strong id="en-us_topic_0175230332_b11201154974716"><a name="en-us_topic_0175230332_b11201154974716"></a><a name="en-us_topic_0175230332_b11201154974716"></a>para</strong>.</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p128975181707"><a name="en-us_topic_0175230332_p128975181707"></a><a name="en-us_topic_0175230332_p128975181707"></a>During internal calling, the input parameter is <strong id="en-us_topic_0175230332_b8143959184710"><a name="en-us_topic_0175230332_b8143959184710"></a><a name="en-us_topic_0175230332_b8143959184710"></a>0</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230332_row8697181313"><td class="cellrowborder" valign="top" headers="mcps1.1.5.1.1 "><p id="en-us_topic_0175230332_p36914188113"><a name="en-us_topic_0175230332_p36914188113"></a><a name="en-us_topic_0175230332_p36914188113"></a><strong id="en-us_topic_0175230332_b33843984820"><a name="en-us_topic_0175230332_b33843984820"></a><a name="en-us_topic_0175230332_b33843984820"></a>para</strong>: error label.</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.5.1.2 "><p id="en-us_topic_0175230332_p1469121814119"><a name="en-us_topic_0175230332_p1469121814119"></a><a name="en-us_topic_0175230332_p1469121814119"></a>During internal calling, the input parameter is <strong id="en-us_topic_0175230332_b15770544151418"><a name="en-us_topic_0175230332_b15770544151418"></a><a name="en-us_topic_0175230332_b15770544151418"></a>NULL</strong>.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>The system actively calls the registered hook function at some error points that are difficult to locate. \(Currently, the hook function is actively called only in the mutex lock module and semaphore module.\)


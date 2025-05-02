# Development Guidelines<a name="EN-US_TOPIC_0311018411"></a>

## Usage Scenarios<a name="en-us_topic_0175230395_section4635261117538"></a>

Events are applicable in a variety of task synchronization scenarios and are partially similar to semaphore in some scenarios.

## Functions<a name="en-us_topic_0175230395_section1976906162710"></a>

The event module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230395_table31878844162710"></a>
<table><thead align="left"><tr id="en-us_topic_0175230395_row24909577162710"><th class="cellrowborder" valign="top" width="17.7%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230395_p4409895162710"><a name="en-us_topic_0175230395_p4409895162710"></a><a name="en-us_topic_0175230395_p4409895162710"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="18.18%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230395_p21657225162710"><a name="en-us_topic_0175230395_p21657225162710"></a><a name="en-us_topic_0175230395_p21657225162710"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="64.12%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230395_p9404824162710"><a name="en-us_topic_0175230395_p9404824162710"></a><a name="en-us_topic_0175230395_p9404824162710"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230395_row24183005155142"><td class="cellrowborder" valign="top" width="17.7%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p34065167155340"><a name="en-us_topic_0175230395_p34065167155340"></a><a name="en-us_topic_0175230395_p34065167155340"></a>Event initialization</p>
</td>
<td class="cellrowborder" valign="top" width="18.18%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p19341936155142"><a name="en-us_topic_0175230395_p19341936155142"></a><a name="en-us_topic_0175230395_p19341936155142"></a>LOS_EventInit</p>
</td>
<td class="cellrowborder" valign="top" width="64.12%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230395_p23193017155142"><a name="en-us_topic_0175230395_p23193017155142"></a><a name="en-us_topic_0175230395_p23193017155142"></a>Initializes an event control block.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row23593275162710"><td class="cellrowborder" rowspan="2" valign="top" width="17.7%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p32007131162710"><a name="en-us_topic_0175230395_p32007131162710"></a><a name="en-us_topic_0175230395_p32007131162710"></a>Event reading</p>
</td>
<td class="cellrowborder" valign="top" width="18.18%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p42440799162710"><a name="en-us_topic_0175230395_p42440799162710"></a><a name="en-us_topic_0175230395_p42440799162710"></a>LOS_EventRead</p>
</td>
<td class="cellrowborder" valign="top" width="64.12%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230395_p15152688162710"><a name="en-us_topic_0175230395_p15152688162710"></a><a name="en-us_topic_0175230395_p15152688162710"></a>Reads events of a specified type. The timeout period is a relative time. The unit is Tick.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row2156464162710"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p55702669162710"><a name="en-us_topic_0175230395_p55702669162710"></a><a name="en-us_topic_0175230395_p55702669162710"></a>LOS_EventWrite</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p15622373162710"><a name="en-us_topic_0175230395_p15622373162710"></a><a name="en-us_topic_0175230395_p15622373162710"></a>Writes events of a specified type.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row6383631162710"><td class="cellrowborder" valign="top" width="17.7%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p47312118162710"><a name="en-us_topic_0175230395_p47312118162710"></a><a name="en-us_topic_0175230395_p47312118162710"></a>Event clearance</p>
</td>
<td class="cellrowborder" valign="top" width="18.18%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p7076378162710"><a name="en-us_topic_0175230395_p7076378162710"></a><a name="en-us_topic_0175230395_p7076378162710"></a>LOS_EventClear</p>
</td>
<td class="cellrowborder" valign="top" width="64.12%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230395_p36315746162710"><a name="en-us_topic_0175230395_p36315746162710"></a><a name="en-us_topic_0175230395_p36315746162710"></a>Clears events of a specified type.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row13860287164240"><td class="cellrowborder" valign="top" width="17.7%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p48941496164240"><a name="en-us_topic_0175230395_p48941496164240"></a><a name="en-us_topic_0175230395_p48941496164240"></a>Event mask verification</p>
</td>
<td class="cellrowborder" valign="top" width="18.18%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p4838245164240"><a name="en-us_topic_0175230395_p4838245164240"></a><a name="en-us_topic_0175230395_p4838245164240"></a>LOS_EventPoll</p>
</td>
<td class="cellrowborder" valign="top" width="64.12%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230395_p56353583164240"><a name="en-us_topic_0175230395_p56353583164240"></a><a name="en-us_topic_0175230395_p56353583164240"></a>Determines whether an event meets the expectations based on the passed-in event ID, event mask, and read mode.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row5784255164347"><td class="cellrowborder" valign="top" width="17.7%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230395_p65871477164347"><a name="en-us_topic_0175230395_p65871477164347"></a><a name="en-us_topic_0175230395_p65871477164347"></a>Event destroying</p>
</td>
<td class="cellrowborder" valign="top" width="18.18%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230395_p33989420164347"><a name="en-us_topic_0175230395_p33989420164347"></a><a name="en-us_topic_0175230395_p33989420164347"></a>LOS_EventDestroy</p>
</td>
<td class="cellrowborder" valign="top" width="64.12%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230395_p1679627164347"><a name="en-us_topic_0175230395_p1679627164347"></a><a name="en-us_topic_0175230395_p1679627164347"></a>Destroys a specified event control block.</p>
</td>
</tr>
</tbody>
</table>

## Event Error Code<a name="en-us_topic_0175230395_section40811079115428"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230395_table874491812737"></a>
<table><thead align="left"><tr id="en-us_topic_0175230395_row2830196712737"><th class="cellrowborder" valign="top" width="5.45%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230395_p2971281012737"><a name="en-us_topic_0175230395_p2971281012737"></a><a name="en-us_topic_0175230395_p2971281012737"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="21.990000000000002%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230395_p5792739612737"><a name="en-us_topic_0175230395_p5792739612737"></a><a name="en-us_topic_0175230395_p5792739612737"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="12.879999999999999%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230395_p6160751912737"><a name="en-us_topic_0175230395_p6160751912737"></a><a name="en-us_topic_0175230395_p6160751912737"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="30.78%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230395_p2415315512737"><a name="en-us_topic_0175230395_p2415315512737"></a><a name="en-us_topic_0175230395_p2415315512737"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="28.9%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230395_p1024850212737"><a name="en-us_topic_0175230395_p1024850212737"></a><a name="en-us_topic_0175230395_p1024850212737"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230395_row35955500121027"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p26714373121027"><a name="en-us_topic_0175230395_p26714373121027"></a><a name="en-us_topic_0175230395_p26714373121027"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p16380603121027"><a name="en-us_topic_0175230395_p16380603121027"></a><a name="en-us_topic_0175230395_p16380603121027"></a>LOS_ERRNO_EVENT_SETBIT_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p12580778121054"><a name="en-us_topic_0175230395_p12580778121054"></a><a name="en-us_topic_0175230395_p12580778121054"></a>0x02001c00</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p31845851121027"><a name="en-us_topic_0175230395_p31845851121027"></a><a name="en-us_topic_0175230395_p31845851121027"></a>During event writing, bit 25 of the event ID is set to <strong id="en-us_topic_0175230395_b1650415294557"><a name="en-us_topic_0175230395_b1650415294557"></a><a name="en-us_topic_0175230395_b1650415294557"></a>1</strong>. This bit is reserved in the OS and cannot be set to <strong id="en-us_topic_0175230395_b21311656195515"><a name="en-us_topic_0175230395_b21311656195515"></a><a name="en-us_topic_0175230395_b21311656195515"></a>1</strong>.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p29377175121027"><a name="en-us_topic_0175230395_p29377175121027"></a><a name="en-us_topic_0175230395_p29377175121027"></a>Set bit 25 of the event ID to 0.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row2512766212737"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p2207475012737"><a name="en-us_topic_0175230395_p2207475012737"></a><a name="en-us_topic_0175230395_p2207475012737"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p4322430612737"><a name="en-us_topic_0175230395_p4322430612737"></a><a name="en-us_topic_0175230395_p4322430612737"></a>LOS_ERRNO_EVENT_READ_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p1150789112737"><a name="en-us_topic_0175230395_p1150789112737"></a><a name="en-us_topic_0175230395_p1150789112737"></a>0x02001c01</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p5972394412737"><a name="en-us_topic_0175230395_p5972394412737"></a><a name="en-us_topic_0175230395_p5972394412737"></a>Event reading times out.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p580127312737"><a name="en-us_topic_0175230395_p580127312737"></a><a name="en-us_topic_0175230395_p580127312737"></a>Increase the permitted wait time. Alternatively, re-read the event.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row5221146412737"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p127022412737"><a name="en-us_topic_0175230395_p127022412737"></a><a name="en-us_topic_0175230395_p127022412737"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p3577930412737"><a name="en-us_topic_0175230395_p3577930412737"></a><a name="en-us_topic_0175230395_p3577930412737"></a>LOS_ERRNO_EVENT_EVENTMASK_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p1244248812737"><a name="en-us_topic_0175230395_p1244248812737"></a><a name="en-us_topic_0175230395_p1244248812737"></a>0x02001c02</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p120861712737"><a name="en-us_topic_0175230395_p120861712737"></a><a name="en-us_topic_0175230395_p120861712737"></a>The passed-in event ID is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p3078915412737"><a name="en-us_topic_0175230395_p3078915412737"></a><a name="en-us_topic_0175230395_p3078915412737"></a>Pass in a valid event ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row866693812737"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p3093336312737"><a name="en-us_topic_0175230395_p3093336312737"></a><a name="en-us_topic_0175230395_p3093336312737"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p2257447312737"><a name="en-us_topic_0175230395_p2257447312737"></a><a name="en-us_topic_0175230395_p2257447312737"></a>LOS_ERRNO_EVENT_READ_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p1659299512737"><a name="en-us_topic_0175230395_p1659299512737"></a><a name="en-us_topic_0175230395_p1659299512737"></a>0x02001c03</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p185533712737"><a name="en-us_topic_0175230395_p185533712737"></a><a name="en-us_topic_0175230395_p185533712737"></a>The event is being read when an interrupt is being processed.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p1606464512737"><a name="en-us_topic_0175230395_p1606464512737"></a><a name="en-us_topic_0175230395_p1606464512737"></a>Start a new task to obtain events.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row1036407712737"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p3418393812737"><a name="en-us_topic_0175230395_p3418393812737"></a><a name="en-us_topic_0175230395_p3418393812737"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p1743561612737"><a name="en-us_topic_0175230395_p1743561612737"></a><a name="en-us_topic_0175230395_p1743561612737"></a>LOS_ERRNO_EVENT_FLAGS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p299876612737"><a name="en-us_topic_0175230395_p299876612737"></a><a name="en-us_topic_0175230395_p299876612737"></a>0x02001c04</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p4157346012737"><a name="en-us_topic_0175230395_p4157346012737"></a><a name="en-us_topic_0175230395_p4157346012737"></a>The mode of event reading is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p1200709912737"><a name="en-us_topic_0175230395_p1200709912737"></a><a name="en-us_topic_0175230395_p1200709912737"></a>Pass in a valid mode.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row4095503012737"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p2902313712737"><a name="en-us_topic_0175230395_p2902313712737"></a><a name="en-us_topic_0175230395_p2902313712737"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p206387212737"><a name="en-us_topic_0175230395_p206387212737"></a><a name="en-us_topic_0175230395_p206387212737"></a>LOS_ERRNO_EVENT_READ_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p3295593212737"><a name="en-us_topic_0175230395_p3295593212737"></a><a name="en-us_topic_0175230395_p3295593212737"></a>0x02001c05</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p5218479612737"><a name="en-us_topic_0175230395_p5218479612737"></a><a name="en-us_topic_0175230395_p5218479612737"></a>The task is locked and fails to read the event.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p6621892012737"><a name="en-us_topic_0175230395_p6621892012737"></a><a name="en-us_topic_0175230395_p6621892012737"></a>Unlock the task, and then let the task read the event.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row3924015812752"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p2433626212752"><a name="en-us_topic_0175230395_p2433626212752"></a><a name="en-us_topic_0175230395_p2433626212752"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p2508019012752"><a name="en-us_topic_0175230395_p2508019012752"></a><a name="en-us_topic_0175230395_p2508019012752"></a>LOS_ERRNO_EVENT_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p1822948612752"><a name="en-us_topic_0175230395_p1822948612752"></a><a name="en-us_topic_0175230395_p1822948612752"></a>0x02001c06</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p19340712752"><a name="en-us_topic_0175230395_p19340712752"></a><a name="en-us_topic_0175230395_p19340712752"></a>The passed-in pointer is null.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p1566600812752"><a name="en-us_topic_0175230395_p1566600812752"></a><a name="en-us_topic_0175230395_p1566600812752"></a>Pass in a non-null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row1451135712910"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p35145711295"><a name="en-us_topic_0175230395_p35145711295"></a><a name="en-us_topic_0175230395_p35145711295"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p1916535613425"><a name="en-us_topic_0175230395_p1916535613425"></a><a name="en-us_topic_0175230395_p1916535613425"></a>LOS_ERRNO_EVENT_READ_IN_SYSTEM_TASK</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p175155713292"><a name="en-us_topic_0175230395_p175155713292"></a><a name="en-us_topic_0175230395_p175155713292"></a>0x02001c07</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p195117571299"><a name="en-us_topic_0175230395_p195117571299"></a><a name="en-us_topic_0175230395_p195117571299"></a>Events are read from system tasks, including idle and software timer tasks.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p55155711292"><a name="en-us_topic_0175230395_p55155711292"></a><a name="en-us_topic_0175230395_p55155711292"></a>Start a new task to obtain events.</p>
</td>
</tr>
<tr id="en-us_topic_0175230395_row1787264972819"><td class="cellrowborder" valign="top" width="5.45%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p11872249152818"><a name="en-us_topic_0175230395_p11872249152818"></a><a name="en-us_topic_0175230395_p11872249152818"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="21.990000000000002%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p15873849132813"><a name="en-us_topic_0175230395_p15873849132813"></a><a name="en-us_topic_0175230395_p15873849132813"></a>LOS_ERRNO_EVENT_SHOULD_NOT_DESTORY</p>
</td>
<td class="cellrowborder" valign="top" width="12.879999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p1387374910289"><a name="en-us_topic_0175230395_p1387374910289"></a><a name="en-us_topic_0175230395_p1387374910289"></a>0x02001c08</p>
</td>
<td class="cellrowborder" valign="top" width="30.78%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p1987324952817"><a name="en-us_topic_0175230395_p1987324952817"></a><a name="en-us_topic_0175230395_p1987324952817"></a>The linked list of events still contains tasks and cannot be destroyed.</p>
</td>
<td class="cellrowborder" valign="top" width="28.9%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p187310492283"><a name="en-us_topic_0175230395_p187310492283"></a><a name="en-us_topic_0175230395_p187310492283"></a>Check whether the linked list of events is empty.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the event module, and the value is  **0x1c**.

## Development Process<a name="en-us_topic_0175230395_section64718916162724"></a>

The typical process of using the event module is as follows:

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Event**  to configure the event module.

    <a name="en-us_topic_0175230395_table32617244171025"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230395_row41225896171025"><th class="cellrowborder" valign="top" width="22.380000000000003%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230395_p50963290171025"><a name="en-us_topic_0175230395_p50963290171025"></a><a name="en-us_topic_0175230395_p50963290171025"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="29.67000000000001%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230395_p34385849171025"><a name="en-us_topic_0175230395_p34385849171025"></a><a name="en-us_topic_0175230395_p34385849171025"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.970000000000002%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230395_p1469319211502"><a name="en-us_topic_0175230395_p1469319211502"></a><a name="en-us_topic_0175230395_p1469319211502"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.3%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230395_p52666268171025"><a name="en-us_topic_0175230395_p52666268171025"></a><a name="en-us_topic_0175230395_p52666268171025"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="19.680000000000003%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230395_p38109296171025"><a name="en-us_topic_0175230395_p38109296171025"></a><a name="en-us_topic_0175230395_p38109296171025"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230395_row54575334171025"><td class="cellrowborder" valign="top" width="22.380000000000003%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p58525911171025"><a name="en-us_topic_0175230395_p58525911171025"></a><a name="en-us_topic_0175230395_p58525911171025"></a>LOSCFG_BASE_IPC_EVENT</p>
    </td>
    <td class="cellrowborder" valign="top" width="29.67000000000001%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p11216088171057"><a name="en-us_topic_0175230395_p11216088171057"></a><a name="en-us_topic_0175230395_p11216088171057"></a>Whether to enable the event tailoring function.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.970000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p10693142185017"><a name="en-us_topic_0175230395_p10693142185017"></a><a name="en-us_topic_0175230395_p10693142185017"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.3%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p56826855171025"><a name="en-us_topic_0175230395_p56826855171025"></a><a name="en-us_topic_0175230395_p56826855171025"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.680000000000003%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p39572571171025"><a name="en-us_topic_0175230395_p39572571171025"></a><a name="en-us_topic_0175230395_p39572571171025"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230395_row58701968171025"><td class="cellrowborder" valign="top" width="22.380000000000003%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230395_p57239002171025"><a name="en-us_topic_0175230395_p57239002171025"></a><a name="en-us_topic_0175230395_p57239002171025"></a>LOSCFG_BASE_IPC_EVENT_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="29.67000000000001%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230395_p5847613171025"><a name="en-us_topic_0175230395_p5847613171025"></a><a name="en-us_topic_0175230395_p5847613171025"></a>Maximum number of supported event control blocks</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.970000000000002%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230395_p9693172175013"><a name="en-us_topic_0175230395_p9693172175013"></a><a name="en-us_topic_0175230395_p9693172175013"></a>None</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.3%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230395_p47029099171025"><a name="en-us_topic_0175230395_p47029099171025"></a><a name="en-us_topic_0175230395_p47029099171025"></a>1024</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.680000000000003%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230395_p297512291482"><a name="en-us_topic_0175230395_p297512291482"></a><a name="en-us_topic_0175230395_p297512291482"></a>LOSCFG_BASE_IPC_EVENT</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Call the LOS\_EventInit API to initialize an event control block.
3.  Call the LOS\_EventWrite API to write events of a specified type.
4.  Call the LOS\_EventRead API and specify a read mode to read events.
5.  Call the LOS\_EventClear API to clear events of a specified type.

## Platform Differences<a name="en-us_topic_0175230395_section62116636104633"></a>

None


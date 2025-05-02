# Development Guidelines<a name="EN-US_TOPIC_0311018392"></a>

## Usage Scenarios<a name="en-us_topic_0175230255_section18537875142955"></a>

When a user needs memory of fixed size, the system calls the LOS\_MemboxAlloc API to allocate memory. When the user no longer needs the memory, the system calls the LOS\_MemboxFree API to free it up.

## Functions<a name="en-us_topic_0175230255_section46614967194125"></a>

The static memory management module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230255_table21283163141945"></a>
<table><thead align="left"><tr id="en-us_topic_0175230255_row14096080141945"><th class="cellrowborder" valign="top" width="24.192419241924192%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230255_p10280284141957"><a name="en-us_topic_0175230255_p10280284141957"></a><a name="en-us_topic_0175230255_p10280284141957"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="23.462346234623464%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230255_p27396658141957"><a name="en-us_topic_0175230255_p27396658141957"></a><a name="en-us_topic_0175230255_p27396658141957"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="52.34523452345235%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230255_p4536862141957"><a name="en-us_topic_0175230255_p4536862141957"></a><a name="en-us_topic_0175230255_p4536862141957"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230255_row21135277141945"><td class="cellrowborder" valign="top" width="24.192419241924192%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p34235853141945"><a name="en-us_topic_0175230255_p34235853141945"></a><a name="en-us_topic_0175230255_p34235853141945"></a>Static memory initialization</p>
</td>
<td class="cellrowborder" valign="top" width="23.462346234623464%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p21640718141945"><a name="en-us_topic_0175230255_p21640718141945"></a><a name="en-us_topic_0175230255_p21640718141945"></a>LOS_MemboxInit</p>
</td>
<td class="cellrowborder" valign="top" width="52.34523452345235%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230255_p65194657143738"><a name="en-us_topic_0175230255_p65194657143738"></a><a name="en-us_topic_0175230255_p65194657143738"></a>Initializes a static memory pool; defines the start address and total size of the static memory pool, as well as the size of each memory block.</p>
</td>
</tr>
<tr id="en-us_topic_0175230255_row18729516201325"><td class="cellrowborder" valign="top" width="24.192419241924192%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p34347916201325"><a name="en-us_topic_0175230255_p34347916201325"></a><a name="en-us_topic_0175230255_p34347916201325"></a>Static memory clearing</p>
</td>
<td class="cellrowborder" valign="top" width="23.462346234623464%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p30717796201325"><a name="en-us_topic_0175230255_p30717796201325"></a><a name="en-us_topic_0175230255_p30717796201325"></a>LOS_MemboxClr</p>
</td>
<td class="cellrowborder" valign="top" width="52.34523452345235%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230255_p5113524201325"><a name="en-us_topic_0175230255_p5113524201325"></a><a name="en-us_topic_0175230255_p5113524201325"></a>Clears data in a memory block of fixed size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230255_row5500759141945"><td class="cellrowborder" rowspan="2" valign="top" width="24.192419241924192%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p42908320141945"><a name="en-us_topic_0175230255_p42908320141945"></a><a name="en-us_topic_0175230255_p42908320141945"></a>Static memory application and free-up</p>
</td>
<td class="cellrowborder" valign="top" width="23.462346234623464%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p53021894141945"><a name="en-us_topic_0175230255_p53021894141945"></a><a name="en-us_topic_0175230255_p53021894141945"></a>LOS_MemboxAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="52.34523452345235%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230255_p3955814011224"><a name="en-us_topic_0175230255_p3955814011224"></a><a name="en-us_topic_0175230255_p3955814011224"></a>Applies for a memory block from a static memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230255_row65364018141945"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p27684738141945"><a name="en-us_topic_0175230255_p27684738141945"></a><a name="en-us_topic_0175230255_p27684738141945"></a>LOS_MemboxFree</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p27871335141945"><a name="en-us_topic_0175230255_p27871335141945"></a><a name="en-us_topic_0175230255_p27871335141945"></a>Frees up a static memory block.</p>
</td>
</tr>
<tr id="en-us_topic_0175230255_row1752554411417"><td class="cellrowborder" rowspan="2" valign="top" width="24.192419241924192%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p11525174461411"><a name="en-us_topic_0175230255_p11525174461411"></a><a name="en-us_topic_0175230255_p11525174461411"></a>Static memory pool information obtainment and printing</p>
</td>
<td class="cellrowborder" valign="top" width="23.462346234623464%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p950663017112"><a name="en-us_topic_0175230255_p950663017112"></a><a name="en-us_topic_0175230255_p950663017112"></a>LOS_MemboxStatisticsGet</p>
</td>
<td class="cellrowborder" valign="top" width="52.34523452345235%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230255_p7506530019"><a name="en-us_topic_0175230255_p7506530019"></a><a name="en-us_topic_0175230255_p7506530019"></a>Obtains information about a static memory pool, including the total number of memory blocks, number of allocated memory blocks, and size of each memory block.</p>
</td>
</tr>
<tr id="en-us_topic_0175230255_row84691501620"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230255_p670686831620"><a name="en-us_topic_0175230255_p670686831620"></a><a name="en-us_topic_0175230255_p670686831620"></a>LOS_ShowBox</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230255_p638542221620"><a name="en-us_topic_0175230255_p638542221620"></a><a name="en-us_topic_0175230255_p638542221620"></a>Prints all node information for a static memory pool (the print level is <strong id="en-us_topic_0175230255_b13690182134114"><a name="en-us_topic_0175230255_b13690182134114"></a><a name="en-us_topic_0175230255_b13690182134114"></a>LOS_INFO_LEVEL</strong>), including the start address of the memory pool, size of each memory block, total number of memory blocks, start address of each idle memory block, and start addresses of all memory blocks.</p>
</td>
</tr>
</tbody>
</table>

## Development Process<a name="en-us_topic_0175230255_section37525629143945"></a>

This section describes the development process of static memory in typical scenarios.

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Memory Management**  to configure the static memory management module.

    <a name="en-us_topic_0175230255_table1124543155750"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230255_row1056095314712"><th class="cellrowborder" valign="top" width="24.062406240624064%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230255_p15912448155750"><a name="en-us_topic_0175230255_p15912448155750"></a><a name="en-us_topic_0175230255_p15912448155750"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="30.8030803080308%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230255_p13839897155750"><a name="en-us_topic_0175230255_p13839897155750"></a><a name="en-us_topic_0175230255_p13839897155750"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.16131613161316%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230255_p47289871155750"><a name="en-us_topic_0175230255_p47289871155750"></a><a name="en-us_topic_0175230255_p47289871155750"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="11.97119711971197%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230255_p5274313155750"><a name="en-us_topic_0175230255_p5274313155750"></a><a name="en-us_topic_0175230255_p5274313155750"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="20.002000200020003%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230255_p24566191155750"><a name="en-us_topic_0175230255_p24566191155750"></a><a name="en-us_topic_0175230255_p24566191155750"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230255_row1285895105110"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230255_p385820511514"><a name="en-us_topic_0175230255_p385820511514"></a><a name="en-us_topic_0175230255_p385820511514"></a>LOSCFG_KERNEL_MEMBOX</p>
    </td>
    <td class="cellrowborder" valign="top" width="30.8030803080308%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230255_p1285865185120"><a name="en-us_topic_0175230255_p1285865185120"></a><a name="en-us_topic_0175230255_p1285865185120"></a>Whether to enable membox management.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.16131613161316%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230255_p98588575113"><a name="en-us_topic_0175230255_p98588575113"></a><a name="en-us_topic_0175230255_p98588575113"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.97119711971197%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230255_p4858165175114"><a name="en-us_topic_0175230255_p4858165175114"></a><a name="en-us_topic_0175230255_p4858165175114"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.002000200020003%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230255_p138583585110"><a name="en-us_topic_0175230255_p138583585110"></a><a name="en-us_topic_0175230255_p138583585110"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230255_row171786312512"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230255_p517823135115"><a name="en-us_topic_0175230255_p517823135115"></a><a name="en-us_topic_0175230255_p517823135115"></a>LOSCFG_KERNEL_MEMBOX_STATIC</p>
    </td>
    <td class="cellrowborder" valign="top" width="30.8030803080308%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230255_p19178036516"><a name="en-us_topic_0175230255_p19178036516"></a><a name="en-us_topic_0175230255_p19178036516"></a>Whether to implement membox in static memory mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.16131613161316%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230255_p16178739511"><a name="en-us_topic_0175230255_p16178739511"></a><a name="en-us_topic_0175230255_p16178739511"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.97119711971197%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230255_p017843125114"><a name="en-us_topic_0175230255_p017843125114"></a><a name="en-us_topic_0175230255_p017843125114"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.002000200020003%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230255_p19178163195111"><a name="en-us_topic_0175230255_p19178163195111"></a><a name="en-us_topic_0175230255_p19178163195111"></a>LOSCFG_KERNEL_MEMBOX</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230255_row817517013516"><td class="cellrowborder" valign="top" width="24.062406240624064%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230255_p1117516016516"><a name="en-us_topic_0175230255_p1117516016516"></a><a name="en-us_topic_0175230255_p1117516016516"></a>LOSCFG_KERNEL_MEMBOX_DYNAMIC</p>
    </td>
    <td class="cellrowborder" valign="top" width="30.8030803080308%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230255_p11754020514"><a name="en-us_topic_0175230255_p11754020514"></a><a name="en-us_topic_0175230255_p11754020514"></a>Whether to implement membox in dynamic memory mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.16131613161316%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230255_p1817513013516"><a name="en-us_topic_0175230255_p1817513013516"></a><a name="en-us_topic_0175230255_p1817513013516"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.97119711971197%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230255_p17175003512"><a name="en-us_topic_0175230255_p17175003512"></a><a name="en-us_topic_0175230255_p17175003512"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.002000200020003%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230255_p20176201511"><a name="en-us_topic_0175230255_p20176201511"></a><a name="en-us_topic_0175230255_p20176201511"></a>LOSCFG_KERNEL_MEMBOX</p>
    </td>
    </tr>
    </tbody>
    </table>


1.  Plan a memory region as the static memory pool.
2.  Call the LOS\_MemboxInit API to initialize the static memory pool.

    Initializes the static memory pool; divides the memory pool that matches the input parameters into N memory blocks, where N depends on the total size of static memory pool and the size of each static memory block\); adds all static memory blocks into a linked list of idle memory blocks; places a control header at the beginning of static memory pool.

3.  Call the LOS\_MemboxAlloc API to allocate static memory.

    Takes an idle memory block out of the linked list and returns the user space address of the memory block.

4.  Call the LOS\_MemboxClr API.

    Clears data in the static memory block that matches the input parameters.

5.  Call the LOS\_MemboxFree API.

    Adds the static memory block that has been freed up to the linked list.


## Platform Differences<a name="en-us_topic_0175230255_section22783633165217"></a>

None


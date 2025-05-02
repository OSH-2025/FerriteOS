# Development Guidelines<a name="EN-US_TOPIC_0312244426"></a>

## Usage Scenarios<a name="en-us_topic_0278792247_section4198546414140"></a>

-   Use the trace function to understand the system running track.

-   Use the trace function to analyze system operations before a system exception occurs and locate faults.

## Functions<a name="en-us_topic_0278792247_section513220500441"></a>

The trace module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

**Table  1**  Functions

<a name="en-us_topic_0278792247_table826553194917"></a>
<table><thead align="left"><tr id="en-us_topic_0278792247_row19261653124916"><th class="cellrowborder" valign="top" width="20.812081208120812%" id="mcps1.2.4.1.1"><p id="en-us_topic_0278792247_p137061421145020"><a name="en-us_topic_0278792247_p137061421145020"></a><a name="en-us_topic_0278792247_p137061421145020"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="21.42214221422142%" id="mcps1.2.4.1.2"><p id="en-us_topic_0278792247_p197061221105017"><a name="en-us_topic_0278792247_p197061221105017"></a><a name="en-us_topic_0278792247_p197061221105017"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="57.76577657765777%" id="mcps1.2.4.1.3"><p id="en-us_topic_0278792247_p117060212502"><a name="en-us_topic_0278792247_p117060212502"></a><a name="en-us_topic_0278792247_p117060212502"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0278792247_row62615354910"><td class="cellrowborder" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p1570622110504"><a name="en-us_topic_0278792247_p1570622110504"></a><a name="en-us_topic_0278792247_p1570622110504"></a>Configuring the trace buffer</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p77061021175013"><a name="en-us_topic_0278792247_p77061021175013"></a><a name="en-us_topic_0278792247_p77061021175013"></a>LOS_TraceInit</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p15706102115017"><a name="en-us_topic_0278792247_p15706102115017"></a><a name="en-us_topic_0278792247_p15706102115017"></a>Configures the address and size of the trace buffer.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row7277531490"><td class="cellrowborder" rowspan="2" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p1070622117508"><a name="en-us_topic_0278792247_p1070622117508"></a><a name="en-us_topic_0278792247_p1070622117508"></a>Starting/Stopping trace event recording</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p1070652116503"><a name="en-us_topic_0278792247_p1070652116503"></a><a name="en-us_topic_0278792247_p1070652116503"></a>LOS_TraceStart</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p870672112506"><a name="en-us_topic_0278792247_p870672112506"></a><a name="en-us_topic_0278792247_p870672112506"></a>Starts event recording.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row52705319498"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p167067218506"><a name="en-us_topic_0278792247_p167067218506"></a><a name="en-us_topic_0278792247_p167067218506"></a>LOS_TraceStop</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p137061321135015"><a name="en-us_topic_0278792247_p137061321135015"></a><a name="en-us_topic_0278792247_p137061321135015"></a>Stops event recording.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row1218417320179"><td class="cellrowborder" rowspan="3" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p74971212111717"><a name="en-us_topic_0278792247_p74971212111717"></a><a name="en-us_topic_0278792247_p74971212111717"></a>Recording data of trace operations</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p20497121231712"><a name="en-us_topic_0278792247_p20497121231712"></a><a name="en-us_topic_0278792247_p20497121231712"></a>LOS_TraceRecordDump</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p184971112191712"><a name="en-us_topic_0278792247_p184971112191712"></a><a name="en-us_topic_0278792247_p184971112191712"></a>Outputs trace buffer data.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row627175314920"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p13706421145011"><a name="en-us_topic_0278792247_p13706421145011"></a><a name="en-us_topic_0278792247_p13706421145011"></a>LOS_TraceRecordGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p15706421145019"><a name="en-us_topic_0278792247_p15706421145019"></a><a name="en-us_topic_0278792247_p15706421145019"></a>Obtains the first address of the trace buffer.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row727153194910"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p57061421145010"><a name="en-us_topic_0278792247_p57061421145010"></a><a name="en-us_topic_0278792247_p57061421145010"></a>LOS_TraceReset</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p1170617214502"><a name="en-us_topic_0278792247_p1170617214502"></a><a name="en-us_topic_0278792247_p1170617214502"></a>Clears events in the trace buffer.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row18271153114920"><td class="cellrowborder" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p1270672112501"><a name="en-us_topic_0278792247_p1270672112501"></a><a name="en-us_topic_0278792247_p1270672112501"></a>Module that filters trace records</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p570617212502"><a name="en-us_topic_0278792247_p570617212502"></a><a name="en-us_topic_0278792247_p570617212502"></a>LOS_TraceEventMaskSet</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p10706172110508"><a name="en-us_topic_0278792247_p10706172110508"></a><a name="en-us_topic_0278792247_p10706172110508"></a>Sets the event mask to record only events of some modules</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row1767361119506"><td class="cellrowborder" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p197061021155019"><a name="en-us_topic_0278792247_p197061021155019"></a><a name="en-us_topic_0278792247_p197061021155019"></a>Masking events with specific interrupt IDs</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p1670642185010"><a name="en-us_topic_0278792247_p1670642185010"></a><a name="en-us_topic_0278792247_p1670642185010"></a>LOS_TraceHwiFilterHookReg</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p10706321125012"><a name="en-us_topic_0278792247_p10706321125012"></a><a name="en-us_topic_0278792247_p10706321125012"></a>Registering the hook function for filtering events of a specific interrupt ID</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row9700201318364"><td class="cellrowborder" rowspan="2" valign="top" width="20.812081208120812%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p148201139133610"><a name="en-us_topic_0278792247_p148201139133610"></a><a name="en-us_topic_0278792247_p148201139133610"></a>Instrumentation function</p>
</td>
<td class="cellrowborder" valign="top" width="21.42214221422142%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p970111134363"><a name="en-us_topic_0278792247_p970111134363"></a><a name="en-us_topic_0278792247_p970111134363"></a>LOS_TRACE_EASY</p>
</td>
<td class="cellrowborder" valign="top" width="57.76577657765777%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0278792247_p167011613133619"><a name="en-us_topic_0278792247_p167011613133619"></a><a name="en-us_topic_0278792247_p167011613133619"></a>Simple instrumentation</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row140992673615"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0278792247_p12410526133613"><a name="en-us_topic_0278792247_p12410526133613"></a><a name="en-us_topic_0278792247_p12410526133613"></a>LOS_TRACE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0278792247_p24103265361"><a name="en-us_topic_0278792247_p24103265361"></a><a name="en-us_topic_0278792247_p24103265361"></a>Standard instrumentation</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>1. LOS\_TRACE\_EASY\(TYPE, IDENTITY, params...\) simple instrumentation.
>    -   Instrumentation in a sentence. You only need to insert the API into the target source code.
>    -   The value range of TYPE is \[0, 0xF\], indicating different event types.
>    -   IDENTITY type UINTPTR, indicating the main object of the event operation.
>    -   The Params type is UINTPTR, indicating the parameter of the event.
>    -   Example:
>        ```
>        LOS_TRACE_EASY(1, userId0, userParam1, userParam2);
>        LOS_TRACE_EASY(2, userId0);
>        LOS_TRACE_EASY(1, userId1, userParam1, userParam2);
>        LOS_TRACE_EASY(2, userId1);
>        ```
>2. LOS\_TRACE\(TYPE, IDENTITY, params...\) standard instrumentation.
>    -   Compared with simple instrumentation, dynamic event filtering and parameter tailoring are supported by standard instrumentation. You need to extend the functions based on rules.
>    -   TYPE is used to set the specific event type. You can customize the event type in enum LOS\_TRACE\_TYPE in the header file  **los\_trace.h**. For details about defining methods and rules, see other event types.
>    -   The types and meanings of IDENTITY and Params are the same as those of simple instrumentation.
>    -   Example:
>        ```
>        1. Define the event mask, that is, the module-level event type in enum LOS_TRACE_MASK. The definition specification is TRACE_#MOD#_FLAG, in which #MOD# indicates the module name. For example:
>          TRACE_FS_FLAG = 0x2000
>        2. Define the event type in enum LOS_TRACE_TYPE. The definition specification is #TYPE# = TRACE_#MOD#_FLAG | NUMBER. For example:
>          FS_READ  = TRACE_FS_FLAG | 0; // Read a file
>          FS_WRITE = TRACE_FS_FLAG | 1; // Write a file
>        3. Define event parameters. The definition specification is #TYPE#_PARAMS(IDENTITY, parma1...) IDENTITY, ...,
>          in which, #TYPE# is the same as the value of #TYPE# in the preceding specification in 2. For example:
>          #define FS_READ_PARAMS(fp, fd, flag, size)    fp, fd, flag, size
>          The parameters defined by the macro correspond to the event parameters recorded in the trace buffer. You can tailor any parameter fields.
>          If this parameter is left empty, events of this type are not traced:
>          #define FS_READ_PARAMS(fp, fd, flag, size) // Does not trace file reading events
>        4. Insert the code stub in a proper position. The definition specification is LOS_TRACE(#TYPE#, #TYPE#_PARAMS(IDENTITY, parma1...)).
>          LOS_TRACE(FS_READ, fp, fd, flag, size); // Code stub for reading files. The input parameter following #TYPE# is the input parameter of the FS_READ_PARAMS function as described in 3.
>        ```
>1.  The preset trace events and parameters of Huawei LiteOS can be tailored by using the preceding method. For details about the parameters, see  **kernel\\include\\los\_trace.h**.

## Trace Error Codes<a name="en-us_topic_0278792247_section13844216355"></a>

Error codes must be returned for trace operations that may fail to be performed, including initializing and starting trace, to quickly locate error causes. For other APIs that do not return any value, such as the APIs for stopping trace tasks, clearing trace data, and dumping trace data, the trace is the invalid state, and the system directly displays the error cause.

<a name="en-us_topic_0278792247_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0278792247_row2267197395642"><th class="cellrowborder" valign="top" width="6.845296303539997%" id="mcps1.1.6.1.1"><p id="en-us_topic_0278792247_p1908783195642"><a name="en-us_topic_0278792247_p1908783195642"></a><a name="en-us_topic_0278792247_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="24.124779972618814%" id="mcps1.1.6.1.2"><p id="en-us_topic_0278792247_p261046995642"><a name="en-us_topic_0278792247_p261046995642"></a><a name="en-us_topic_0278792247_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="10.219049481713279%" id="mcps1.1.6.1.3"><p id="en-us_topic_0278792247_p1012144095642"><a name="en-us_topic_0278792247_p1012144095642"></a><a name="en-us_topic_0278792247_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="26.276158810874247%" id="mcps1.1.6.1.4"><p id="en-us_topic_0278792247_p1453028795642"><a name="en-us_topic_0278792247_p1453028795642"></a><a name="en-us_topic_0278792247_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="32.534715431253666%" id="mcps1.1.6.1.5"><p id="en-us_topic_0278792247_p2753561710026"><a name="en-us_topic_0278792247_p2753561710026"></a><a name="en-us_topic_0278792247_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0278792247_row6366372295642"><td class="cellrowborder" valign="top" width="6.845296303539997%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p5648782795642"><a name="en-us_topic_0278792247_p5648782795642"></a><a name="en-us_topic_0278792247_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="24.124779972618814%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p1211123695642"><a name="en-us_topic_0278792247_p1211123695642"></a><a name="en-us_topic_0278792247_p1211123695642"></a>LOS_ERRNO_TRACE_ERROR_STATUS</p>
</td>
<td class="cellrowborder" valign="top" width="10.219049481713279%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p4148605495642"><a name="en-us_topic_0278792247_p4148605495642"></a><a name="en-us_topic_0278792247_p4148605495642"></a>0x02001400</p>
</td>
<td class="cellrowborder" valign="top" width="26.276158810874247%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p492720095642"><a name="en-us_topic_0278792247_p492720095642"></a><a name="en-us_topic_0278792247_p492720095642"></a>The status is incorrect when trace is initialized or started.</p>
</td>
<td class="cellrowborder" valign="top" width="32.534715431253666%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p144215555119"><a name="en-us_topic_0278792247_p144215555119"></a><a name="en-us_topic_0278792247_p144215555119"></a>Do not initialize trace repeatedly. Do not start trace before it is initialized.</p>
</td>
</tr>
<tr id="en-us_topic_0278792247_row112091915183814"><td class="cellrowborder" valign="top" width="6.845296303539997%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p2020918159388"><a name="en-us_topic_0278792247_p2020918159388"></a><a name="en-us_topic_0278792247_p2020918159388"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="24.124779972618814%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p8209171516380"><a name="en-us_topic_0278792247_p8209171516380"></a><a name="en-us_topic_0278792247_p8209171516380"></a>LOS_ERRNO_TRACE_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="10.219049481713279%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p182092153388"><a name="en-us_topic_0278792247_p182092153388"></a><a name="en-us_topic_0278792247_p182092153388"></a>0x02001401</p>
</td>
<td class="cellrowborder" valign="top" width="26.276158810874247%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p20209111510381"><a name="en-us_topic_0278792247_p20209111510381"></a><a name="en-us_topic_0278792247_p20209111510381"></a>The system fails to apply for a buffer when initializing trace.</p>
</td>
<td class="cellrowborder" valign="top" width="32.534715431253666%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p14525105518557"><a name="en-us_topic_0278792247_p14525105518557"></a><a name="en-us_topic_0278792247_p14525105518557"></a>Two solutions are available:</p>
<a name="en-us_topic_0278792247_ul17711443556"></a><a name="en-us_topic_0278792247_ul17711443556"></a><ul id="en-us_topic_0278792247_ul17711443556"><li>To reduce the size of the trace buffer, modify LOS_TRACE_BUFFER_SIZE in <strong id="en-us_topic_0278792247_b171912119017"><a name="en-us_topic_0278792247_b171912119017"></a><a name="en-us_topic_0278792247_b171912119017"></a>los_config.h</strong>.</li><li>Configure LOSCFG_BASE_CORE_TSK_LIMIT to reduce the maximum number of tasks.</li></ul>
</td>
</tr>
<tr id="en-us_topic_0278792247_row1698417245383"><td class="cellrowborder" valign="top" width="6.845296303539997%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p149841024163810"><a name="en-us_topic_0278792247_p149841024163810"></a><a name="en-us_topic_0278792247_p149841024163810"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="24.124779972618814%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p2098432413813"><a name="en-us_topic_0278792247_p2098432413813"></a><a name="en-us_topic_0278792247_p2098432413813"></a>LOS_ERRNO_TRACE_BUF_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="10.219049481713279%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p19984162443818"><a name="en-us_topic_0278792247_p19984162443818"></a><a name="en-us_topic_0278792247_p19984162443818"></a>0x02001402</p>
</td>
<td class="cellrowborder" valign="top" width="26.276158810874247%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p6984202473819"><a name="en-us_topic_0278792247_p6984202473819"></a><a name="en-us_topic_0278792247_p6984202473819"></a>When trace is initialized, the buffer size is set to a small value.</p>
</td>
<td class="cellrowborder" valign="top" width="32.534715431253666%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p1098419242384"><a name="en-us_topic_0278792247_p1098419242384"></a><a name="en-us_topic_0278792247_p1098419242384"></a>Increase the size of the trace buffer by modifying LOS_TRACE_BUFFER_SIZE in <strong id="en-us_topic_0278792247_b15457341412"><a name="en-us_topic_0278792247_b15457341412"></a><a name="en-us_topic_0278792247_b15457341412"></a>los_config.h</strong>.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](../LiteOS_Kernel_Developer_Guide_en/development-guidelines-14.md). Bits 8–15 belong to the trace module, and the value is 0x14.

## Development Process<a name="en-us_topic_0278792247_section15585181011559"></a>

The typical development process of the trace module is as follows:

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Extend Kernel**  \>  **Enable Trace Feature**  to configure the trace module.

    <a name="en-us_topic_0278792247_table06655375130"></a>
    <table><thead align="left"><tr id="en-us_topic_0278792247_row8665203751318"><th class="cellrowborder" valign="top" width="24.36%" id="mcps1.1.6.1.1"><p id="en-us_topic_0278792247_p61687296155221"><a name="en-us_topic_0278792247_p61687296155221"></a><a name="en-us_topic_0278792247_p61687296155221"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="32.09%" id="mcps1.1.6.1.2"><p id="en-us_topic_0278792247_p25007692155221"><a name="en-us_topic_0278792247_p25007692155221"></a><a name="en-us_topic_0278792247_p25007692155221"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="10.27%" id="mcps1.1.6.1.3"><p id="en-us_topic_0278792247_p47081329204415"><a name="en-us_topic_0278792247_p47081329204415"></a><a name="en-us_topic_0278792247_p47081329204415"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="8.64%" id="mcps1.1.6.1.4"><p id="en-us_topic_0278792247_p7383544155221"><a name="en-us_topic_0278792247_p7383544155221"></a><a name="en-us_topic_0278792247_p7383544155221"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="24.64%" id="mcps1.1.6.1.5"><p id="en-us_topic_0278792247_p34917797155221"><a name="en-us_topic_0278792247_p34917797155221"></a><a name="en-us_topic_0278792247_p34917797155221"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0278792247_row9665837161320"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p3665143710130"><a name="en-us_topic_0278792247_p3665143710130"></a><a name="en-us_topic_0278792247_p3665143710130"></a>LOSCFG_KERNEL_TRACE</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p13665737191318"><a name="en-us_topic_0278792247_p13665737191318"></a><a name="en-us_topic_0278792247_p13665737191318"></a>Tailoring switch of the trace module</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p19665193721319"><a name="en-us_topic_0278792247_p19665193721319"></a><a name="en-us_topic_0278792247_p19665193721319"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p1866573713131"><a name="en-us_topic_0278792247_p1866573713131"></a><a name="en-us_topic_0278792247_p1866573713131"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p1266519378138"><a name="en-us_topic_0278792247_p1266519378138"></a><a name="en-us_topic_0278792247_p1266519378138"></a>LOSCFG_KERNEL_EXTKERNEL</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row1193311701712"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p565351111510"><a name="en-us_topic_0278792247_p565351111510"></a><a name="en-us_topic_0278792247_p565351111510"></a>LOSCFG_RECORDER_MODE_OFFLINE</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p9933111721714"><a name="en-us_topic_0278792247_p9933111721714"></a><a name="en-us_topic_0278792247_p9933111721714"></a>Trace works in offline mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p6933141711719"><a name="en-us_topic_0278792247_p6933141711719"></a><a name="en-us_topic_0278792247_p6933141711719"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p13933131712173"><a name="en-us_topic_0278792247_p13933131712173"></a><a name="en-us_topic_0278792247_p13933131712173"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p79331717111713"><a name="en-us_topic_0278792247_p79331717111713"></a><a name="en-us_topic_0278792247_p79331717111713"></a>LOSCFG_KERNEL_TRACE</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row1363216235172"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p164432221258"><a name="en-us_topic_0278792247_p164432221258"></a><a name="en-us_topic_0278792247_p164432221258"></a>LOSCFG_RECORDER_MODE_ONLINE</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p463216237171"><a name="en-us_topic_0278792247_p463216237171"></a><a name="en-us_topic_0278792247_p463216237171"></a>Trace works in online mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p063232310177"><a name="en-us_topic_0278792247_p063232310177"></a><a name="en-us_topic_0278792247_p063232310177"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p136321523141717"><a name="en-us_topic_0278792247_p136321523141717"></a><a name="en-us_topic_0278792247_p136321523141717"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p13632623151714"><a name="en-us_topic_0278792247_p13632623151714"></a><a name="en-us_topic_0278792247_p13632623151714"></a>LOSCFG_KERNEL_TRACE</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row866021191717"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p176602181713"><a name="en-us_topic_0278792247_p176602181713"></a><a name="en-us_topic_0278792247_p176602181713"></a>LOSCFG_TRACE_CLIENT_INTERACT</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p66612114174"><a name="en-us_topic_0278792247_p66612114174"></a><a name="en-us_topic_0278792247_p66612114174"></a>Enables interaction with the Trace IDE (Huawei LiteOS Studio), including data visualization and process control.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p136610216173"><a name="en-us_topic_0278792247_p136610216173"></a><a name="en-us_topic_0278792247_p136610216173"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p8666214175"><a name="en-us_topic_0278792247_p8666214175"></a><a name="en-us_topic_0278792247_p8666214175"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p4662021201717"><a name="en-us_topic_0278792247_p4662021201717"></a><a name="en-us_topic_0278792247_p4662021201717"></a>LOSCFG_KERNEL_TRACE</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row1985044712205"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p1685054715202"><a name="en-us_topic_0278792247_p1685054715202"></a><a name="en-us_topic_0278792247_p1685054715202"></a>LOSCFG_TRACE_PIPELINE_SERIAL</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p13850147192010"><a name="en-us_topic_0278792247_p13850147192010"></a><a name="en-us_topic_0278792247_p13850147192010"></a>Selects a serial port as the IDE data visualization channel.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p1985024716201"><a name="en-us_topic_0278792247_p1985024716201"></a><a name="en-us_topic_0278792247_p1985024716201"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p1685024712012"><a name="en-us_topic_0278792247_p1685024712012"></a><a name="en-us_topic_0278792247_p1685024712012"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p53475337916"><a name="en-us_topic_0278792247_p53475337916"></a><a name="en-us_topic_0278792247_p53475337916"></a>LOSCFG_TRACE_CLIENT_INTERACT</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row38211350172019"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p48221509207"><a name="en-us_topic_0278792247_p48221509207"></a><a name="en-us_topic_0278792247_p48221509207"></a>LOSCFG_TRACE_CONTROL_VIA_SHELL</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p482235020204"><a name="en-us_topic_0278792247_p482235020204"></a><a name="en-us_topic_0278792247_p482235020204"></a>Selects shell as the IDE process control mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p12822750132015"><a name="en-us_topic_0278792247_p12822750132015"></a><a name="en-us_topic_0278792247_p12822750132015"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p13822105012014"><a name="en-us_topic_0278792247_p13822105012014"></a><a name="en-us_topic_0278792247_p13822105012014"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p12551133151117"><a name="en-us_topic_0278792247_p12551133151117"></a><a name="en-us_topic_0278792247_p12551133151117"></a>LOSCFG_TRACE_CLIENT_INTERACT &amp;&amp; LOSCFG_SHELL</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row20489356191111"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p548925610113"><a name="en-us_topic_0278792247_p548925610113"></a><a name="en-us_topic_0278792247_p548925610113"></a>LOSCFG_TRACE_CONTROL_AGENT</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p748975617115"><a name="en-us_topic_0278792247_p748975617115"></a><a name="en-us_topic_0278792247_p748975617115"></a>Selects an agent as the IDE process control mode.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p245718577126"><a name="en-us_topic_0278792247_p245718577126"></a><a name="en-us_topic_0278792247_p245718577126"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p5489956171118"><a name="en-us_topic_0278792247_p5489956171118"></a><a name="en-us_topic_0278792247_p5489956171118"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p7489115613117"><a name="en-us_topic_0278792247_p7489115613117"></a><a name="en-us_topic_0278792247_p7489115613117"></a>LOSCFG_TRACE_CLIENT_INTERACT</p>
    </td>
    </tr>
    <tr id="en-us_topic_0278792247_row10858152718144"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0278792247_p685952717142"><a name="en-us_topic_0278792247_p685952717142"></a><a name="en-us_topic_0278792247_p685952717142"></a>LOSCFG_TRACE_NO_CONTROL</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0278792247_p68591027191416"><a name="en-us_topic_0278792247_p68591027191416"></a><a name="en-us_topic_0278792247_p68591027191416"></a>Disables the IDE process control.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0278792247_p1540675816140"><a name="en-us_topic_0278792247_p1540675816140"></a><a name="en-us_topic_0278792247_p1540675816140"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0278792247_p188591827141416"><a name="en-us_topic_0278792247_p188591827141416"></a><a name="en-us_topic_0278792247_p188591827141416"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0278792247_p10859152716147"><a name="en-us_topic_0278792247_p10859152716147"></a><a name="en-us_topic_0278792247_p10859152716147"></a>LOSCFG_TRACE_CLIENT_INTERACT</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  \(Optional\) Preset event parameters and event stubs \(or use the default event parameter settings and event stubs\).
3.  \(Optional\) Clear the LOS\_TraceReset buffer after trace is stopped by calling LOS\_TraceStop. \(Trace is started by default.\)
4.  \(Optional\) Set the mask of the event to be traced by calling LOS\_TraceEventMaskSet. \(The default event mask enables only interrupt and task switching.\)
5.  Call the  **LOS\_TraceStart**  API at the start point of the code where the event needs to be recorded.
6.  Call the  **LOS\_TraceStop**  API at the end point of the code where the event needs to be recorded.
7.  Call LOS\_TraceRecordDump to output the data in the buffer. \(The input parameter of the function is of the Boolean type. FALSE indicates that the data is output in the formatted format, and TRUE indicates that the data is output to a Windows client.\)

## Platform Differences<a name="en-us_topic_0278792247_section15592521317"></a>

None


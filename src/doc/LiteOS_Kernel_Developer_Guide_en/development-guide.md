# Development Guide<a name="EN-US_TOPIC_0145350146"></a>

## Usage Scenarios<a name="section229254733516"></a>

Queues are used for communication between tasks to implement asynchronous message processing. In addition, the message sender and recipient do not need to communicate with each other because they are decoupled.

## Functions<a name="en-us_topic_0102498422_section267785513432"></a>

The queue module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230428_table23667517184354"></a>
<table><thead align="left"><tr id="en-us_topic_0175230428_row23800586184354"><th class="cellrowborder" valign="top" width="23.56%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230428_p5651424118443"><a name="en-us_topic_0175230428_p5651424118443"></a><a name="en-us_topic_0175230428_p5651424118443"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="24.29%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230428_p1425081918443"><a name="en-us_topic_0175230428_p1425081918443"></a><a name="en-us_topic_0175230428_p1425081918443"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="52.15%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230428_p1346571418443"><a name="en-us_topic_0175230428_p1346571418443"></a><a name="en-us_topic_0175230428_p1346571418443"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230428_row62888924184354"><td class="cellrowborder" rowspan="3" valign="top" width="23.56%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p3328741318443"><a name="en-us_topic_0175230428_p3328741318443"></a><a name="en-us_topic_0175230428_p3328741318443"></a>Creating or deleting a message queue</p>
</td>
<td class="cellrowborder" valign="top" width="24.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p1192591318443"><a name="en-us_topic_0175230428_p1192591318443"></a><a name="en-us_topic_0175230428_p1192591318443"></a>LOS_QueueCreate</p>
</td>
<td class="cellrowborder" valign="top" width="52.15%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230428_p2647489018443"><a name="en-us_topic_0175230428_p2647489018443"></a><a name="en-us_topic_0175230428_p2647489018443"></a>Creates a message queue whose the queue space is dynamically allocated by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row4459241419"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p123598451144"><a name="en-us_topic_0175230428_p123598451144"></a><a name="en-us_topic_0175230428_p123598451144"></a>LOS_QueueCreateStatic</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p1345192141410"><a name="en-us_topic_0175230428_p1345192141410"></a><a name="en-us_topic_0175230428_p1345192141410"></a>Creates a message queue whose queue space is allocated by the user and transferred to the API.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row151398386345"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p17568143915340"><a name="en-us_topic_0175230428_p17568143915340"></a><a name="en-us_topic_0175230428_p17568143915340"></a>LOS_QueueDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p11568153963413"><a name="en-us_topic_0175230428_p11568153963413"></a><a name="en-us_topic_0175230428_p11568153963413"></a>Deletes a specified queue based on the queue ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row25480013184354"><td class="cellrowborder" rowspan="3" valign="top" width="23.56%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p3995088818443"><a name="en-us_topic_0175230428_p3995088818443"></a><a name="en-us_topic_0175230428_p3995088818443"></a>Read/Write queue (without the content contained in the address)</p>
</td>
<td class="cellrowborder" valign="top" width="24.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p1479653518443"><a name="en-us_topic_0175230428_p1479653518443"></a><a name="en-us_topic_0175230428_p1479653518443"></a>LOS_QueueRead</p>
</td>
<td class="cellrowborder" valign="top" width="52.15%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230428_p5766871918443"><a name="en-us_topic_0175230428_p5766871918443"></a><a name="en-us_topic_0175230428_p5766871918443"></a>Reads the specified head node data in the queue. The node data in a queue is an address.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row13975964184354"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p4222878818443"><a name="en-us_topic_0175230428_p4222878818443"></a><a name="en-us_topic_0175230428_p4222878818443"></a>LOS_QueueWrite</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p6508866818443"><a name="en-us_topic_0175230428_p6508866818443"></a><a name="en-us_topic_0175230428_p6508866818443"></a>Writes the value of the input parameter <strong id="en-us_topic_0175230428_b72018615718"><a name="en-us_topic_0175230428_b72018615718"></a><a name="en-us_topic_0175230428_b72018615718"></a>bufferAddr</strong> (that is, the buffer address) to the tail node of a specified queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row147410295355"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p23501231123510"><a name="en-us_topic_0175230428_p23501231123510"></a><a name="en-us_topic_0175230428_p23501231123510"></a>LOS_QueueWriteHead</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p13503312359"><a name="en-us_topic_0175230428_p13503312359"></a><a name="en-us_topic_0175230428_p13503312359"></a>Write the value of the input parameter <strong id="en-us_topic_0175230428_b192773045717"><a name="en-us_topic_0175230428_b192773045717"></a><a name="en-us_topic_0175230428_b192773045717"></a>bufferAddr</strong> (that is, the buffer address) to the head node of a specified queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row49536790111815"><td class="cellrowborder" rowspan="3" valign="top" width="23.56%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p53057044111815"><a name="en-us_topic_0175230428_p53057044111815"></a><a name="en-us_topic_0175230428_p53057044111815"></a>Read/Write queue (with the content contained in the address)</p>
</td>
<td class="cellrowborder" valign="top" width="24.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p2653311111815"><a name="en-us_topic_0175230428_p2653311111815"></a><a name="en-us_topic_0175230428_p2653311111815"></a>LOS_QueueReadCopy</p>
</td>
<td class="cellrowborder" valign="top" width="52.15%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230428_p13591614111815"><a name="en-us_topic_0175230428_p13591614111815"></a><a name="en-us_topic_0175230428_p13591614111815"></a>Reads data from the head node of a specified queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row43811731111820"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p21507820111820"><a name="en-us_topic_0175230428_p21507820111820"></a><a name="en-us_topic_0175230428_p21507820111820"></a>LOS_QueueWriteCopy</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p64411829111820"><a name="en-us_topic_0175230428_p64411829111820"></a><a name="en-us_topic_0175230428_p64411829111820"></a>Writes the data saved in the input parameter <strong id="en-us_topic_0175230428_b541621516585"><a name="en-us_topic_0175230428_b541621516585"></a><a name="en-us_topic_0175230428_b541621516585"></a>bufferAddr</strong> to the tail node of a specified queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row53214216105859"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p38164146105859"><a name="en-us_topic_0175230428_p38164146105859"></a><a name="en-us_topic_0175230428_p38164146105859"></a>LOS_QueueWriteHeadCopy</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p4288110105859"><a name="en-us_topic_0175230428_p4288110105859"></a><a name="en-us_topic_0175230428_p4288110105859"></a>Writes the data saved in the input parameter <strong id="en-us_topic_0175230428_b15250925195818"><a name="en-us_topic_0175230428_b15250925195818"></a><a name="en-us_topic_0175230428_b15250925195818"></a>bufferAddr</strong> to the head node of a specified queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row16299901184354"><td class="cellrowborder" valign="top" width="23.56%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230428_p378995218443"><a name="en-us_topic_0175230428_p378995218443"></a><a name="en-us_topic_0175230428_p378995218443"></a>Obtaining queue information</p>
</td>
<td class="cellrowborder" valign="top" width="24.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230428_p3855073218443"><a name="en-us_topic_0175230428_p3855073218443"></a><a name="en-us_topic_0175230428_p3855073218443"></a>LOS_QueueInfoGet</p>
</td>
<td class="cellrowborder" valign="top" width="52.15%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230428_p3560155518443"><a name="en-us_topic_0175230428_p3560155518443"></a><a name="en-us_topic_0175230428_p3560155518443"></a>Obtains information about a specified queue, including the queue ID, queue length, message node size, head node, tail node, number of readable nodes, number of writable nodes, tasks waiting for read operations, tasks waiting for write operations, and tasks waiting for mail operations.</p>
</td>
</tr>
</tbody>
</table>

## Queue Error Codes<a name="section1814817995210"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230428_table66521465191749"></a>
<table><thead align="left"><tr id="en-us_topic_0175230428_row66647056191749"><th class="cellrowborder" valign="top" width="5.34%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230428_p65995609191749"><a name="en-us_topic_0175230428_p65995609191749"></a><a name="en-us_topic_0175230428_p65995609191749"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="20.21%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230428_p44044076191749"><a name="en-us_topic_0175230428_p44044076191749"></a><a name="en-us_topic_0175230428_p44044076191749"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="13.930000000000001%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230428_p10800441191749"><a name="en-us_topic_0175230428_p10800441191749"></a><a name="en-us_topic_0175230428_p10800441191749"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="24.610000000000003%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230428_p39597633191844"><a name="en-us_topic_0175230428_p39597633191844"></a><a name="en-us_topic_0175230428_p39597633191844"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="35.91%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230428_p61844251191749"><a name="en-us_topic_0175230428_p61844251191749"></a><a name="en-us_topic_0175230428_p61844251191749"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230428_row6517730619218"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p4487040819218"><a name="en-us_topic_0175230428_p4487040819218"></a><a name="en-us_topic_0175230428_p4487040819218"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p1062439819218"><a name="en-us_topic_0175230428_p1062439819218"></a><a name="en-us_topic_0175230428_p1062439819218"></a>LOS_ERRNO_QUEUE_MAXNUM_ZERO</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p5526990919218"><a name="en-us_topic_0175230428_p5526990919218"></a><a name="en-us_topic_0175230428_p5526990919218"></a>0x02000600</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p9238164164714"><a name="en-us_topic_0175230428_p9238164164714"></a><a name="en-us_topic_0175230428_p9238164164714"></a>The maximum number of queues supported by the system is 0.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p1372919138224"><a name="en-us_topic_0175230428_p1372919138224"></a><a name="en-us_topic_0175230428_p1372919138224"></a>The maximum number of queues supported by the system must be greater than 0. If the queue module is not used, set <strong id="en-us_topic_0175230428_b69000331016"><a name="en-us_topic_0175230428_b69000331016"></a><a name="en-us_topic_0175230428_b69000331016"></a>LOSCFG_BASE_IPC_QUEUE</strong> to <strong id="en-us_topic_0175230428_b61601636008"><a name="en-us_topic_0175230428_b61601636008"></a><a name="en-us_topic_0175230428_b61601636008"></a>NO</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row33366968191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p18369922191749"><a name="en-us_topic_0175230428_p18369922191749"></a><a name="en-us_topic_0175230428_p18369922191749"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p11568741191749"><a name="en-us_topic_0175230428_p11568741191749"></a><a name="en-us_topic_0175230428_p11568741191749"></a>LOS_ERRNO_QUEUE_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p64652808191749"><a name="en-us_topic_0175230428_p64652808191749"></a><a name="en-us_topic_0175230428_p64652808191749"></a>0x02000601</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p787615154915"><a name="en-us_topic_0175230428_p787615154915"></a><a name="en-us_topic_0175230428_p787615154915"></a>Failed to apply for memory from the dynamic memory pool during queue initialization.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p1687611564915"><a name="en-us_topic_0175230428_p1687611564915"></a><a name="en-us_topic_0175230428_p1687611564915"></a>Set the configuration item <strong id="en-us_topic_0175230428_b1543317318110"><a name="en-us_topic_0175230428_b1543317318110"></a><a name="en-us_topic_0175230428_b1543317318110"></a>OS_SYS_MEM_SIZE</strong> to a larger system dynamic memory pool, or reduce the maximum number of queues supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row33397154191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p20814974191749"><a name="en-us_topic_0175230428_p20814974191749"></a><a name="en-us_topic_0175230428_p20814974191749"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p8291367191749"><a name="en-us_topic_0175230428_p8291367191749"></a><a name="en-us_topic_0175230428_p8291367191749"></a>LOS_ERRNO_QUEUE_CREATE_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p512132191749"><a name="en-us_topic_0175230428_p512132191749"></a><a name="en-us_topic_0175230428_p512132191749"></a>0x02000602</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p181431132184915"><a name="en-us_topic_0175230428_p181431132184915"></a><a name="en-us_topic_0175230428_p181431132184915"></a>Failed to apply for memory from the dynamic memory pool during queue creation.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p614312322497"><a name="en-us_topic_0175230428_p614312322497"></a><a name="en-us_topic_0175230428_p614312322497"></a>Set <strong id="en-us_topic_0175230428_b81354264219"><a name="en-us_topic_0175230428_b81354264219"></a><a name="en-us_topic_0175230428_b81354264219"></a>OS_SYS_MEM_SIZE</strong> to a larger system dynamic memory pool, or reduce the length and message node size of the queue to be created.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row41391008191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p64337369191749"><a name="en-us_topic_0175230428_p64337369191749"></a><a name="en-us_topic_0175230428_p64337369191749"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p43944370191749"><a name="en-us_topic_0175230428_p43944370191749"></a><a name="en-us_topic_0175230428_p43944370191749"></a>LOS_ERRNO_QUEUE_SIZE_TOO_BIG</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p2724179191749"><a name="en-us_topic_0175230428_p2724179191749"></a><a name="en-us_topic_0175230428_p2724179191749"></a>0x02000603</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p197252184910"><a name="en-us_topic_0175230428_p197252184910"></a><a name="en-us_topic_0175230428_p197252184910"></a>The message node size exceeds the upper limit during queue creation.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p49735213493"><a name="en-us_topic_0175230428_p49735213493"></a><a name="en-us_topic_0175230428_p49735213493"></a>Change the input parameter for message node size to ensure that the size does not exceed the upper limit.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row10026959191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p6877374191749"><a name="en-us_topic_0175230428_p6877374191749"></a><a name="en-us_topic_0175230428_p6877374191749"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p20196417191749"><a name="en-us_topic_0175230428_p20196417191749"></a><a name="en-us_topic_0175230428_p20196417191749"></a>LOS_ERRNO_QUEUE_CB_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p25297089191749"><a name="en-us_topic_0175230428_p25297089191749"></a><a name="en-us_topic_0175230428_p25297089191749"></a>0x02000604</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p1732013725019"><a name="en-us_topic_0175230428_p1732013725019"></a><a name="en-us_topic_0175230428_p1732013725019"></a>No idle queue exists in the system during queue creation.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p232020755019"><a name="en-us_topic_0175230428_p232020755019"></a><a name="en-us_topic_0175230428_p232020755019"></a>Increase the maximum number of queues supported by the system.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row59373972191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p44562467191749"><a name="en-us_topic_0175230428_p44562467191749"></a><a name="en-us_topic_0175230428_p44562467191749"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p52790107191749"><a name="en-us_topic_0175230428_p52790107191749"></a><a name="en-us_topic_0175230428_p52790107191749"></a>LOS_ERRNO_QUEUE_NOT_FOUND</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p48140259191749"><a name="en-us_topic_0175230428_p48140259191749"></a><a name="en-us_topic_0175230428_p48140259191749"></a>0x02000605</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p60420222193129"><a name="en-us_topic_0175230428_p60420222193129"></a><a name="en-us_topic_0175230428_p60420222193129"></a>The queue ID transferred to the API for deleting a queue is greater than or equal to the maximum number of queues supported by the system.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p49643405193152"><a name="en-us_topic_0175230428_p49643405193152"></a><a name="en-us_topic_0175230428_p49643405193152"></a>Ensure that the queue ID is valid.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row36473565191749"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p1568774191749"><a name="en-us_topic_0175230428_p1568774191749"></a><a name="en-us_topic_0175230428_p1568774191749"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p59961868191749"><a name="en-us_topic_0175230428_p59961868191749"></a><a name="en-us_topic_0175230428_p59961868191749"></a>LOS_ERRNO_QUEUE_PEND_IN_LOCK</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p25073110191749"><a name="en-us_topic_0175230428_p25073110191749"></a><a name="en-us_topic_0175230428_p25073110191749"></a>0x02000606</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p19045334193230"><a name="en-us_topic_0175230428_p19045334193230"></a><a name="en-us_topic_0175230428_p19045334193230"></a>When a task is locked, the queue used by the task cannot block the wait for message writing or reading.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p20051835193257"><a name="en-us_topic_0175230428_p20051835193257"></a><a name="en-us_topic_0175230428_p20051835193257"></a>Unlock the locked task before using a queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row51263497192010"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p58702556192010"><a name="en-us_topic_0175230428_p58702556192010"></a><a name="en-us_topic_0175230428_p58702556192010"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p57286633192010"><a name="en-us_topic_0175230428_p57286633192010"></a><a name="en-us_topic_0175230428_p57286633192010"></a>LOS_ERRNO_QUEUE_TIMEOUT</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p9705686192010"><a name="en-us_topic_0175230428_p9705686192010"></a><a name="en-us_topic_0175230428_p9705686192010"></a>0x02000607</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p53994389193449"><a name="en-us_topic_0175230428_p53994389193449"></a><a name="en-us_topic_0175230428_p53994389193449"></a>Waiting for processing the queue timed out.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p51382338193520"><a name="en-us_topic_0175230428_p51382338193520"></a><a name="en-us_topic_0175230428_p51382338193520"></a>Check that the configured timeout interval is proper.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row36402998193312"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p62961745193312"><a name="en-us_topic_0175230428_p62961745193312"></a><a name="en-us_topic_0175230428_p62961745193312"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p66736597193312"><a name="en-us_topic_0175230428_p66736597193312"></a><a name="en-us_topic_0175230428_p66736597193312"></a>LOS_ERRNO_QUEUE_IN_TSKUSE</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p36955240193312"><a name="en-us_topic_0175230428_p36955240193312"></a><a name="en-us_topic_0175230428_p36955240193312"></a>0x02000608</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p3637555120413"><a name="en-us_topic_0175230428_p3637555120413"></a><a name="en-us_topic_0175230428_p3637555120413"></a>The queue is involved in a blocked task and cannot be deleted.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p66119138193312"><a name="en-us_topic_0175230428_p66119138193312"></a><a name="en-us_topic_0175230428_p66119138193312"></a>Enables tasks to obtain resources instead of being blocked in the queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row51203889193321"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p53874317193321"><a name="en-us_topic_0175230428_p53874317193321"></a><a name="en-us_topic_0175230428_p53874317193321"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p1743582193321"><a name="en-us_topic_0175230428_p1743582193321"></a><a name="en-us_topic_0175230428_p1743582193321"></a>LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p7012492193321"><a name="en-us_topic_0175230428_p7012492193321"></a><a name="en-us_topic_0175230428_p7012492193321"></a>0x02000609</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p31140990193321"><a name="en-us_topic_0175230428_p31140990193321"></a><a name="en-us_topic_0175230428_p31140990193321"></a>The queue cannot be written in block mode in the interrupt handler.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p51817932193749"><a name="en-us_topic_0175230428_p51817932193749"></a><a name="en-us_topic_0175230428_p51817932193749"></a>Set the write queue to non-block mode, that is, set the timeout interval of the write queue to <strong id="en-us_topic_0175230428_b97131235121"><a name="en-us_topic_0175230428_b97131235121"></a><a name="en-us_topic_0175230428_b97131235121"></a>0</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row58997007193325"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p14028263193325"><a name="en-us_topic_0175230428_p14028263193325"></a><a name="en-us_topic_0175230428_p14028263193325"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p62547532193325"><a name="en-us_topic_0175230428_p62547532193325"></a><a name="en-us_topic_0175230428_p62547532193325"></a>LOS_ERRNO_QUEUE_NOT_CREATE</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p33185317193325"><a name="en-us_topic_0175230428_p33185317193325"></a><a name="en-us_topic_0175230428_p33185317193325"></a>0x0200060a</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p3656155193325"><a name="en-us_topic_0175230428_p3656155193325"></a><a name="en-us_topic_0175230428_p3656155193325"></a>The queue has not been created.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p14251707193839"><a name="en-us_topic_0175230428_p14251707193839"></a><a name="en-us_topic_0175230428_p14251707193839"></a>Create the queue or replace it with an existing queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row23132180193345"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p61767288193345"><a name="en-us_topic_0175230428_p61767288193345"></a><a name="en-us_topic_0175230428_p61767288193345"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p37094461193345"><a name="en-us_topic_0175230428_p37094461193345"></a><a name="en-us_topic_0175230428_p37094461193345"></a>LOS_ERRNO_QUEUE_IN_TSKWRITE</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p51861387193345"><a name="en-us_topic_0175230428_p51861387193345"></a><a name="en-us_topic_0175230428_p51861387193345"></a>0x0200060b</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p6555415519399"><a name="en-us_topic_0175230428_p6555415519399"></a><a name="en-us_topic_0175230428_p6555415519399"></a>The read and write operations in the queue are not synchronized.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p20620767193345"><a name="en-us_topic_0175230428_p20620767193345"></a><a name="en-us_topic_0175230428_p20620767193345"></a>Synchronize the read and write operations in the queue. Specifically, multiple tasks are not allowed to concurrently read data from or write data into the same queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row21687970193352"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p11895156193352"><a name="en-us_topic_0175230428_p11895156193352"></a><a name="en-us_topic_0175230428_p11895156193352"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p23983603193352"><a name="en-us_topic_0175230428_p23983603193352"></a><a name="en-us_topic_0175230428_p23983603193352"></a>LOS_ERRNO_QUEUE_CREAT_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p63623686193352"><a name="en-us_topic_0175230428_p63623686193352"></a><a name="en-us_topic_0175230428_p63623686193352"></a>0x0200060c</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p3279290519404"><a name="en-us_topic_0175230428_p3279290519404"></a><a name="en-us_topic_0175230428_p3279290519404"></a>For the API for creating a queue, the input parameter for saving the queue ID is a null pointer.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p17873674193352"><a name="en-us_topic_0175230428_p17873674193352"></a><a name="en-us_topic_0175230428_p17873674193352"></a>Ensure that the input parameter is not a null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row32251036193358"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p62197112193358"><a name="en-us_topic_0175230428_p62197112193358"></a><a name="en-us_topic_0175230428_p62197112193358"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p4801272193358"><a name="en-us_topic_0175230428_p4801272193358"></a><a name="en-us_topic_0175230428_p4801272193358"></a>LOS_ERRNO_QUEUE_PARA_ISZERO</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p53358724193358"><a name="en-us_topic_0175230428_p53358724193358"></a><a name="en-us_topic_0175230428_p53358724193358"></a>0x0200060d</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p27309930194051"><a name="en-us_topic_0175230428_p27309930194051"></a><a name="en-us_topic_0175230428_p27309930194051"></a>For the API for creating a queue, the queue length or message node size is 0.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p46758883193358"><a name="en-us_topic_0175230428_p46758883193358"></a><a name="en-us_topic_0175230428_p46758883193358"></a>Input correct queue length and message node size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row6471318219343"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p727640719343"><a name="en-us_topic_0175230428_p727640719343"></a><a name="en-us_topic_0175230428_p727640719343"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p5251806419343"><a name="en-us_topic_0175230428_p5251806419343"></a><a name="en-us_topic_0175230428_p5251806419343"></a>LOS_ERRNO_QUEUE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p2610475319343"><a name="en-us_topic_0175230428_p2610475319343"></a><a name="en-us_topic_0175230428_p2610475319343"></a>0x0200060e</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p5223249920528"><a name="en-us_topic_0175230428_p5223249920528"></a><a name="en-us_topic_0175230428_p5223249920528"></a>The queue ID transferred to the API for reading data from a queue, writing data to queue, or obtaining the queue information is greater than or equal to the maximum number of queues supported by the system.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p329454442063"><a name="en-us_topic_0175230428_p329454442063"></a><a name="en-us_topic_0175230428_p329454442063"></a>Ensure that the queue ID is valid.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row3895023819348"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p85270419348"><a name="en-us_topic_0175230428_p85270419348"></a><a name="en-us_topic_0175230428_p85270419348"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p196018619348"><a name="en-us_topic_0175230428_p196018619348"></a><a name="en-us_topic_0175230428_p196018619348"></a>LOS_ERRNO_QUEUE_READ_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p2455739719348"><a name="en-us_topic_0175230428_p2455739719348"></a><a name="en-us_topic_0175230428_p2455739719348"></a>0x0200060f</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p5676363194253"><a name="en-us_topic_0175230428_p5676363194253"></a><a name="en-us_topic_0175230428_p5676363194253"></a>The pointer transferred to the read queue API is null.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p37131993194325"><a name="en-us_topic_0175230428_p37131993194325"></a><a name="en-us_topic_0175230428_p37131993194325"></a>Ensure that the input parameter is not a null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row15616099193413"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p56944481193413"><a name="en-us_topic_0175230428_p56944481193413"></a><a name="en-us_topic_0175230428_p56944481193413"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p49100215193413"><a name="en-us_topic_0175230428_p49100215193413"></a><a name="en-us_topic_0175230428_p49100215193413"></a>LOS_ERRNO_QUEUE_READSIZE_IS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p17694478193413"><a name="en-us_topic_0175230428_p17694478193413"></a><a name="en-us_topic_0175230428_p17694478193413"></a>0x02000610</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p23966605193413"><a name="en-us_topic_0175230428_p23966605193413"></a><a name="en-us_topic_0175230428_p23966605193413"></a>The size of the buffer transferred to the read queue API is 0 or greater than 0xFFFB.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p62246877193413"><a name="en-us_topic_0175230428_p62246877193413"></a><a name="en-us_topic_0175230428_p62246877193413"></a>The input buffer size must be greater than 0 and less than 0xFFFC.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row24056666194424"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p2432890194424"><a name="en-us_topic_0175230428_p2432890194424"></a><a name="en-us_topic_0175230428_p2432890194424"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p62846374194424"><a name="en-us_topic_0175230428_p62846374194424"></a><a name="en-us_topic_0175230428_p62846374194424"></a>LOS_ERRNO_QUEUE_WRITE_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p57391559194424"><a name="en-us_topic_0175230428_p57391559194424"></a><a name="en-us_topic_0175230428_p57391559194424"></a>0x02000612</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p2160612619463"><a name="en-us_topic_0175230428_p2160612619463"></a><a name="en-us_topic_0175230428_p2160612619463"></a>The buffer pointer transferred to the write queue API is null.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p65297927194424"><a name="en-us_topic_0175230428_p65297927194424"></a><a name="en-us_topic_0175230428_p65297927194424"></a>Ensure that the input parameter is not a null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row23479934194430"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p22826525194430"><a name="en-us_topic_0175230428_p22826525194430"></a><a name="en-us_topic_0175230428_p22826525194430"></a>19</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p37009232194430"><a name="en-us_topic_0175230428_p37009232194430"></a><a name="en-us_topic_0175230428_p37009232194430"></a>LOS_ERRNO_QUEUE_WRITESIZE_ISZERO</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p44957824194430"><a name="en-us_topic_0175230428_p44957824194430"></a><a name="en-us_topic_0175230428_p44957824194430"></a>0x02000613</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p28691532194638"><a name="en-us_topic_0175230428_p28691532194638"></a><a name="en-us_topic_0175230428_p28691532194638"></a>The size of the buffer transferred to the write queue API is 0.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p24827637194430"><a name="en-us_topic_0175230428_p24827637194430"></a><a name="en-us_topic_0175230428_p24827637194430"></a>Input a correct buffer size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row2499390194657"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p1124069194657"><a name="en-us_topic_0175230428_p1124069194657"></a><a name="en-us_topic_0175230428_p1124069194657"></a>20</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p23940727194657"><a name="en-us_topic_0175230428_p23940727194657"></a><a name="en-us_topic_0175230428_p23940727194657"></a>LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p60150750194657"><a name="en-us_topic_0175230428_p60150750194657"></a><a name="en-us_topic_0175230428_p60150750194657"></a>0x02000615</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p40372610194657"><a name="en-us_topic_0175230428_p40372610194657"></a><a name="en-us_topic_0175230428_p40372610194657"></a>The size of the buffer transferred to the write queue API is greater than the message node size of the queue.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p7860434194847"><a name="en-us_topic_0175230428_p7860434194847"></a><a name="en-us_topic_0175230428_p7860434194847"></a>Reduce the buffer size or increase the message node size of the queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row4823728219471"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p1490580719471"><a name="en-us_topic_0175230428_p1490580719471"></a><a name="en-us_topic_0175230428_p1490580719471"></a>21</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p6651970819471"><a name="en-us_topic_0175230428_p6651970819471"></a><a name="en-us_topic_0175230428_p6651970819471"></a>LOS_ERRNO_QUEUE_ISFULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p1938724919471"><a name="en-us_topic_0175230428_p1938724919471"></a><a name="en-us_topic_0175230428_p1938724919471"></a>0x02000616</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p15921102185619"><a name="en-us_topic_0175230428_p15921102185619"></a><a name="en-us_topic_0175230428_p15921102185619"></a>No idle node is available when data is written to the queue.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p892182115562"><a name="en-us_topic_0175230428_p892182115562"></a><a name="en-us_topic_0175230428_p892182115562"></a>Before writing data to a queue, ensure that there is an available idle node in the queue or use the block mode to write data to the queue. That is, set the timeout interval of the write queue to a value greater than 0.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row33549139194916"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p33125773194917"><a name="en-us_topic_0175230428_p33125773194917"></a><a name="en-us_topic_0175230428_p33125773194917"></a>22</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p65941984194917"><a name="en-us_topic_0175230428_p65941984194917"></a><a name="en-us_topic_0175230428_p65941984194917"></a>LOS_ERRNO_QUEUE_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p39700467194917"><a name="en-us_topic_0175230428_p39700467194917"></a><a name="en-us_topic_0175230428_p39700467194917"></a>0x02000617</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p1581313369567"><a name="en-us_topic_0175230428_p1581313369567"></a><a name="en-us_topic_0175230428_p1581313369567"></a>The pointer transferred to the API for obtaining queue information is null.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p1981333617564"><a name="en-us_topic_0175230428_p1981333617564"></a><a name="en-us_topic_0175230428_p1981333617564"></a>Ensure that the input parameter is not a null pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row39947507194923"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p14522634194923"><a name="en-us_topic_0175230428_p14522634194923"></a><a name="en-us_topic_0175230428_p14522634194923"></a>23</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p35482697194923"><a name="en-us_topic_0175230428_p35482697194923"></a><a name="en-us_topic_0175230428_p35482697194923"></a>LOS_ERRNO_QUEUE_READ_IN_INTERRUPT</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p55526233194923"><a name="en-us_topic_0175230428_p55526233194923"></a><a name="en-us_topic_0175230428_p55526233194923"></a>0x02000618</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p719617491561"><a name="en-us_topic_0175230428_p719617491561"></a><a name="en-us_topic_0175230428_p719617491561"></a>The queue cannot be read in block mode in an interrupt handler.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p4196154910564"><a name="en-us_topic_0175230428_p4196154910564"></a><a name="en-us_topic_0175230428_p4196154910564"></a>Set the read queue to non-block mode, that is, set the timeout interval of the read queue to <strong id="en-us_topic_0175230428_b17333155117201"><a name="en-us_topic_0175230428_b17333155117201"></a><a name="en-us_topic_0175230428_b17333155117201"></a>0</strong>.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row50011219194929"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p24376929194929"><a name="en-us_topic_0175230428_p24376929194929"></a><a name="en-us_topic_0175230428_p24376929194929"></a>24</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p28374236194929"><a name="en-us_topic_0175230428_p28374236194929"></a><a name="en-us_topic_0175230428_p28374236194929"></a>LOS_ERRNO_QUEUE_MAIL_HANDLE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p16611755194929"><a name="en-us_topic_0175230428_p16611755194929"></a><a name="en-us_topic_0175230428_p16611755194929"></a>0x02000619</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p419411103577"><a name="en-us_topic_0175230428_p419411103577"></a><a name="en-us_topic_0175230428_p419411103577"></a>When the memory block of the mail queue in CMSIS-RTOS 1.0 is released, the input mail queue ID is invalid.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p55981730143813"><a name="en-us_topic_0175230428_p55981730143813"></a><a name="en-us_topic_0175230428_p55981730143813"></a>Ensure that the input mail queue ID is correct.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row41671503194933"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p19948548194933"><a name="en-us_topic_0175230428_p19948548194933"></a><a name="en-us_topic_0175230428_p19948548194933"></a>25</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p5219669194933"><a name="en-us_topic_0175230428_p5219669194933"></a><a name="en-us_topic_0175230428_p5219669194933"></a>LOS_ERRNO_QUEUE_MAIL_PTR_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p20140076194933"><a name="en-us_topic_0175230428_p20140076194933"></a><a name="en-us_topic_0175230428_p20140076194933"></a>0x0200061a</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p128852235715"><a name="en-us_topic_0175230428_p128852235715"></a><a name="en-us_topic_0175230428_p128852235715"></a>When the memory block of the mail queue in CMSIS-RTOS 1.0 is released, the pointer of the input mail memory pool is null.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p828862235714"><a name="en-us_topic_0175230428_p828862235714"></a><a name="en-us_topic_0175230428_p828862235714"></a>Input non-null mail memory pool pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row24094539194938"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p5500604194938"><a name="en-us_topic_0175230428_p5500604194938"></a><a name="en-us_topic_0175230428_p5500604194938"></a>26</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p42895774194938"><a name="en-us_topic_0175230428_p42895774194938"></a><a name="en-us_topic_0175230428_p42895774194938"></a>LOS_ERRNO_QUEUE_MAIL_FREE_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p52005656194938"><a name="en-us_topic_0175230428_p52005656194938"></a><a name="en-us_topic_0175230428_p52005656194938"></a>0x0200061b</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p1834033520578"><a name="en-us_topic_0175230428_p1834033520578"></a><a name="en-us_topic_0175230428_p1834033520578"></a>The memory block of the mail queue in CMSIS-RTOS 1.0 failed to be released.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p834011351571"><a name="en-us_topic_0175230428_p834011351571"></a><a name="en-us_topic_0175230428_p834011351571"></a>Input non-null memory block pointer of the mail queue.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row32525342195533"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p17307058195533"><a name="en-us_topic_0175230428_p17307058195533"></a><a name="en-us_topic_0175230428_p17307058195533"></a>27</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p59694421195533"><a name="en-us_topic_0175230428_p59694421195533"></a><a name="en-us_topic_0175230428_p59694421195533"></a>LOS_ERRNO_QUEUE_ISEMPTY</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p3409970195533"><a name="en-us_topic_0175230428_p3409970195533"></a><a name="en-us_topic_0175230428_p3409970195533"></a>0x0200061d</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p7772143195533"><a name="en-us_topic_0175230428_p7772143195533"></a><a name="en-us_topic_0175230428_p7772143195533"></a>The queue is empty.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p65865508195654"><a name="en-us_topic_0175230428_p65865508195654"></a><a name="en-us_topic_0175230428_p65865508195654"></a>Before reading a queue, ensure that there are unread messages in the queue or use the block mode to read the queue, that is, set the read queue timeout interval to a value greater than 0.</p>
</td>
</tr>
<tr id="en-us_topic_0175230428_row14065892195540"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p65595428195540"><a name="en-us_topic_0175230428_p65595428195540"></a><a name="en-us_topic_0175230428_p65595428195540"></a>28</p>
</td>
<td class="cellrowborder" valign="top" width="20.21%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p11629424195540"><a name="en-us_topic_0175230428_p11629424195540"></a><a name="en-us_topic_0175230428_p11629424195540"></a>LOS_ERRNO_QUEUE_READ_SIZE_TOO_SMALL</p>
</td>
<td class="cellrowborder" valign="top" width="13.930000000000001%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p2459313195540"><a name="en-us_topic_0175230428_p2459313195540"></a><a name="en-us_topic_0175230428_p2459313195540"></a>0x0200061f</p>
</td>
<td class="cellrowborder" valign="top" width="24.610000000000003%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p910254135819"><a name="en-us_topic_0175230428_p910254135819"></a><a name="en-us_topic_0175230428_p910254135819"></a>The size of the read buffer transferred to the read queue API is less than the message node size of the queue.</p>
</td>
<td class="cellrowborder" valign="top" width="35.91%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p4713788520034"><a name="en-us_topic_0175230428_p4713788520034"></a><a name="en-us_topic_0175230428_p4713788520034"></a>Increase the buffer size or reduce the message node size of the queue.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>-   For details about the error code definition, see  [Error Codes](development-guidelines-14.md). Bits 8–15 belong to the queue module, and the value is 0x06.
>-   The error codes 0x11 and 0x14 in the queue module are not defined and cannot be used.

## Development Process<a name="en-us_topic_0102498422_section3269044813432"></a>

The typical process of using the queue module is as follows:

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Queue**  to configure the queue module.

    <a name="en-us_topic_0175230428_table32617244171025"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230428_row41225896171025"><th class="cellrowborder" valign="top" width="23.25%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230428_p50963290171025"><a name="en-us_topic_0175230428_p50963290171025"></a><a name="en-us_topic_0175230428_p50963290171025"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="31.22%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230428_p34385849171025"><a name="en-us_topic_0175230428_p34385849171025"></a><a name="en-us_topic_0175230428_p34385849171025"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="12.78%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230428_p19411171414538"><a name="en-us_topic_0175230428_p19411171414538"></a><a name="en-us_topic_0175230428_p19411171414538"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="11.14%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230428_p52666268171025"><a name="en-us_topic_0175230428_p52666268171025"></a><a name="en-us_topic_0175230428_p52666268171025"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="21.61%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230428_p38109296171025"><a name="en-us_topic_0175230428_p38109296171025"></a><a name="en-us_topic_0175230428_p38109296171025"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230428_row1811644317012"><td class="cellrowborder" valign="top" width="23.25%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p1510306017"><a name="en-us_topic_0175230428_p1510306017"></a><a name="en-us_topic_0175230428_p1510306017"></a>LOSCFG_BASE_IPC_QUEUE</p>
    </td>
    <td class="cellrowborder" valign="top" width="31.22%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p7309049162730"><a name="en-us_topic_0175230428_p7309049162730"></a><a name="en-us_topic_0175230428_p7309049162730"></a>Whether to tailor the queue module.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.78%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p55162128162730"><a name="en-us_topic_0175230428_p55162128162730"></a><a name="en-us_topic_0175230428_p55162128162730"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.14%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p38947346162730"><a name="en-us_topic_0175230428_p38947346162730"></a><a name="en-us_topic_0175230428_p38947346162730"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.61%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p618473162730"><a name="en-us_topic_0175230428_p618473162730"></a><a name="en-us_topic_0175230428_p618473162730"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230428_row54575334171025"><td class="cellrowborder" valign="top" width="23.25%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p58525911171025"><a name="en-us_topic_0175230428_p58525911171025"></a><a name="en-us_topic_0175230428_p58525911171025"></a>LOSCFG_QUEUE_STATIC_ALLOCATION</p>
    </td>
    <td class="cellrowborder" valign="top" width="31.22%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p11216088171057"><a name="en-us_topic_0175230428_p11216088171057"></a><a name="en-us_topic_0175230428_p11216088171057"></a>Whether to create a queue whose memory is allocated by users.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.78%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p1319010382532"><a name="en-us_topic_0175230428_p1319010382532"></a><a name="en-us_topic_0175230428_p1319010382532"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.14%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p123251518512"><a name="en-us_topic_0175230428_p123251518512"></a><a name="en-us_topic_0175230428_p123251518512"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.61%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p39572571171025"><a name="en-us_topic_0175230428_p39572571171025"></a><a name="en-us_topic_0175230428_p39572571171025"></a>LOSCFG_BASE_IPC_QUEUE</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230428_row58701968171025"><td class="cellrowborder" valign="top" width="23.25%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230428_p57239002171025"><a name="en-us_topic_0175230428_p57239002171025"></a><a name="en-us_topic_0175230428_p57239002171025"></a>LOSCFG_BASE_IPC_QUEUE_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="31.22%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230428_p5847613171025"><a name="en-us_topic_0175230428_p5847613171025"></a><a name="en-us_topic_0175230428_p5847613171025"></a>Maximum number of queues supported by the system.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.78%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230428_p13411111415537"><a name="en-us_topic_0175230428_p13411111415537"></a><a name="en-us_topic_0175230428_p13411111415537"></a>&lt;65535</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.14%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230428_p47029099171025"><a name="en-us_topic_0175230428_p47029099171025"></a><a name="en-us_topic_0175230428_p47029099171025"></a>1024</p>
    </td>
    <td class="cellrowborder" valign="top" width="21.61%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230428_p51260679171025"><a name="en-us_topic_0175230428_p51260679171025"></a><a name="en-us_topic_0175230428_p51260679171025"></a>LOSCFG_BASE_IPC_QUEUE</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Create a queue. After the queue is created, you can obtain the queue ID.
3.  Write data to the queue.
4.  Read the queue.
5.  Obtain the queue information.
6.  Delete the queue.

## Platform Differences<a name="en-us_topic_0102498422_section769015813432"></a>

None


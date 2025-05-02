# Development Guidelines<a name="EN-US_TOPIC_0311018442"></a>

## Functions<a name="en-us_topic_0175230574_section1976906162710"></a>

The doubly linked list module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230574_table31878844162710"></a>
<table><thead align="left"><tr id="en-us_topic_0175230574_row24909577162710"><th class="cellrowborder" valign="top" width="20.84%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230574_p4409895162710"><a name="en-us_topic_0175230574_p4409895162710"></a><a name="en-us_topic_0175230574_p4409895162710"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="21.88%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230574_p21657225162710"><a name="en-us_topic_0175230574_p21657225162710"></a><a name="en-us_topic_0175230574_p21657225162710"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="57.28%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230574_p9404824162710"><a name="en-us_topic_0175230574_p9404824162710"></a><a name="en-us_topic_0175230574_p9404824162710"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230574_row3332575162537"><td class="cellrowborder" rowspan="2" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p1503143162537"><a name="en-us_topic_0175230574_p1503143162537"></a><a name="en-us_topic_0175230574_p1503143162537"></a>List initialization</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p54645735162537"><a name="en-us_topic_0175230574_p54645735162537"></a><a name="en-us_topic_0175230574_p54645735162537"></a>LOS_ListInit</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p64228424162537"><a name="en-us_topic_0175230574_p64228424162537"></a><a name="en-us_topic_0175230574_p64228424162537"></a>Initializes a specified node to a doubly linked list node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row63311156131410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p833117569148"><a name="en-us_topic_0175230574_p833117569148"></a><a name="en-us_topic_0175230574_p833117569148"></a>LOS_DL_LIST_HEAD</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p833111569142"><a name="en-us_topic_0175230574_p833111569142"></a><a name="en-us_topic_0175230574_p833111569142"></a>Defines a node and initializes it to a doubly linked list node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row23593275162710"><td class="cellrowborder" rowspan="3" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p20608486161731"><a name="en-us_topic_0175230574_p20608486161731"></a><a name="en-us_topic_0175230574_p20608486161731"></a>Node insertion</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p42440799162710"><a name="en-us_topic_0175230574_p42440799162710"></a><a name="en-us_topic_0175230574_p42440799162710"></a>LOS_ListAdd</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p15152688162710"><a name="en-us_topic_0175230574_p15152688162710"></a><a name="en-us_topic_0175230574_p15152688162710"></a>Inserts a specified node to the head of a doubly linked list.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row56015283114519"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p28138252114519"><a name="en-us_topic_0175230574_p28138252114519"></a><a name="en-us_topic_0175230574_p28138252114519"></a>LOS_ListHeadInsert</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p64605955114519"><a name="en-us_topic_0175230574_p64605955114519"></a><a name="en-us_topic_0175230574_p64605955114519"></a>Inserts a specified node to the head of a doubly linked list, which is the same as LOS_ListAdd.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row9935195212551"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p55702669162710"><a name="en-us_topic_0175230574_p55702669162710"></a><a name="en-us_topic_0175230574_p55702669162710"></a>LOS_ListTailInsert</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p15622373162710"><a name="en-us_topic_0175230574_p15622373162710"></a><a name="en-us_topic_0175230574_p15622373162710"></a>Inserts a specified node to the tail of a doubly linked list.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row448985511526"><td class="cellrowborder" rowspan="2" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p49686117162150"><a name="en-us_topic_0175230574_p49686117162150"></a><a name="en-us_topic_0175230574_p49686117162150"></a>Node deletion</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p6426197611526"><a name="en-us_topic_0175230574_p6426197611526"></a><a name="en-us_topic_0175230574_p6426197611526"></a>LOS_ListDelete</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p52712195115237"><a name="en-us_topic_0175230574_p52712195115237"></a><a name="en-us_topic_0175230574_p52712195115237"></a>Deletes a specified node from a doubly linked list.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row1023610324012"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p307431620736"><a name="en-us_topic_0175230574_p307431620736"></a><a name="en-us_topic_0175230574_p307431620736"></a>LOS_ListDelInit</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p2658453620736"><a name="en-us_topic_0175230574_p2658453620736"></a><a name="en-us_topic_0175230574_p2658453620736"></a>Deletes a specified node from the linked list and uses the node to initialize the linked list.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row20058016115551"><td class="cellrowborder" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p14086595115551"><a name="en-us_topic_0175230574_p14086595115551"></a><a name="en-us_topic_0175230574_p14086595115551"></a>Checking whether a doubly linked list is empty</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p163555115551"><a name="en-us_topic_0175230574_p163555115551"></a><a name="en-us_topic_0175230574_p163555115551"></a>LOS_ListEmpty</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p13247975115551"><a name="en-us_topic_0175230574_p13247975115551"></a><a name="en-us_topic_0175230574_p13247975115551"></a>Checks whether a doubly linked list is empty.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row247318308166"><td class="cellrowborder" rowspan="2" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p1547433061614"><a name="en-us_topic_0175230574_p1547433061614"></a><a name="en-us_topic_0175230574_p1547433061614"></a>Obtaining a node</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p147463071610"><a name="en-us_topic_0175230574_p147463071610"></a><a name="en-us_topic_0175230574_p147463071610"></a>LOS_DL_LIST_LAST</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p11474330171618"><a name="en-us_topic_0175230574_p11474330171618"></a><a name="en-us_topic_0175230574_p11474330171618"></a>Obtains the previous node of a specified node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row8121720112211"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p14121172052216"><a name="en-us_topic_0175230574_p14121172052216"></a><a name="en-us_topic_0175230574_p14121172052216"></a>LOS_DL_LIST_FIRST</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p26938427224"><a name="en-us_topic_0175230574_p26938427224"></a><a name="en-us_topic_0175230574_p26938427224"></a>Obtains the next node of a specified node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row4549184416612"><td class="cellrowborder" rowspan="2" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p354934420619"><a name="en-us_topic_0175230574_p354934420619"></a><a name="en-us_topic_0175230574_p354934420619"></a>Obtaining structure information</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p135494441562"><a name="en-us_topic_0175230574_p135494441562"></a><a name="en-us_topic_0175230574_p135494441562"></a>LOS_DL_LIST_ENTRY</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p1764811219106"><a name="en-us_topic_0175230574_p1764811219106"></a><a name="en-us_topic_0175230574_p1764811219106"></a>Obtains the address of the structure that contains the linked list. The first input parameter of the API indicates a node in the linked list, the second input parameter indicates the name of the structure to be obtained, and the third input parameter indicates the name of the linked list in the structure.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row15765744192214"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p5765124418226"><a name="en-us_topic_0175230574_p5765124418226"></a><a name="en-us_topic_0175230574_p5765124418226"></a>LOS_OFF_SET_OF</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p67652044152220"><a name="en-us_topic_0175230574_p67652044152220"></a><a name="en-us_topic_0175230574_p67652044152220"></a>Obtains the offset of a member in a specified structure relative to the start address of the structure.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row16797171711230"><td class="cellrowborder" rowspan="2" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p3797151712318"><a name="en-us_topic_0175230574_p3797151712318"></a><a name="en-us_topic_0175230574_p3797151712318"></a>Traversing the doubly linked list</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p10797141711235"><a name="en-us_topic_0175230574_p10797141711235"></a><a name="en-us_topic_0175230574_p10797141711235"></a>LOS_DL_LIST_FOR_EACH</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p157971517122317"><a name="en-us_topic_0175230574_p157971517122317"></a><a name="en-us_topic_0175230574_p157971517122317"></a>Traverses a doubly linked list.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row163687110268"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p153686172620"><a name="en-us_topic_0175230574_p153686172620"></a><a name="en-us_topic_0175230574_p153686172620"></a>LOS_DL_LIST_FOR_EACH_SAFE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p236812162616"><a name="en-us_topic_0175230574_p236812162616"></a><a name="en-us_topic_0175230574_p236812162616"></a>Traverses a doubly linked list, and stores the next node of the current node for security verification.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row522163914275"><td class="cellrowborder" rowspan="3" valign="top" width="20.84%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p9221183932712"><a name="en-us_topic_0175230574_p9221183932712"></a><a name="en-us_topic_0175230574_p9221183932712"></a>Traversing the structure that contains the doubly linked list</p>
</td>
<td class="cellrowborder" valign="top" width="21.88%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p422114390274"><a name="en-us_topic_0175230574_p422114390274"></a><a name="en-us_topic_0175230574_p422114390274"></a>LOS_DL_LIST_FOR_EACH_ENTRY</p>
</td>
<td class="cellrowborder" valign="top" width="57.28%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230574_p322153910274"><a name="en-us_topic_0175230574_p322153910274"></a><a name="en-us_topic_0175230574_p322153910274"></a>Traverses a specified doubly linked list and obtain the address of the structure that contains the linked list node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row1433317199289"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p933319196285"><a name="en-us_topic_0175230574_p933319196285"></a><a name="en-us_topic_0175230574_p933319196285"></a>LOS_DL_LIST_FOR_EACH_ENTRY_SAFE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p1833341992818"><a name="en-us_topic_0175230574_p1833341992818"></a><a name="en-us_topic_0175230574_p1833341992818"></a>Traverses a specified doubly linked list, obtains the structure address of the node that contains the linked list, and stores the structure address that contains the next node of the current node.</p>
</td>
</tr>
<tr id="en-us_topic_0175230574_row27341222192815"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230574_p3734222162811"><a name="en-us_topic_0175230574_p3734222162811"></a><a name="en-us_topic_0175230574_p3734222162811"></a>LOS_DL_LIST_FOR_EACH_ENTRY_HOOK</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230574_p12734722182811"><a name="en-us_topic_0175230574_p12734722182811"></a><a name="en-us_topic_0175230574_p12734722182811"></a>Traverses a specified doubly linked list, obtain the structure address of the link list node, and call the hook function in each loop.</p>
</td>
</tr>
</tbody>
</table>

## Development Process<a name="en-us_topic_0175230574_section64718916162724"></a>

The doubly linked list development process is as follows:

1.  Call  **LOS\_ListInit/LOS\_DL\_LIST\_HEAD**  to initialize the doubly linked list.
2.  Call LOS\_ListAdd/LOS\_ListHeadInsert to insert a node to the head of the linked list.
3.  Call the LOS\_ListTailInsert API to insert a node into the tail of the list.
4.  Call the LOS\_ListDelete API to delete a node from the list.
5.  Call the LOS\_ListEmpty API to determine whether the doubly linked list is empty.
6.  Call the LOS\_ListDelInit API to delete a node and use the node to initialize the doubly linked list.


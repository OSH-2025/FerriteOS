# Development Guidelines<a name="EN-US_TOPIC_0311018452"></a>

## Usage Scenarios<a name="en-us_topic_0175230360_section3383949133619"></a>

Huawei LiteOS can set a flag bit to 1 or clear the bit, change the flag bit content, and obtain the most significant bit \(MSB\) and the least significant bit \(LSB\) with a flag bit of 1 from the SW. Developers can also perform bit operations on system registers.

## Functions<a name="en-us_topic_0175230360_section30372907115550"></a>

The bit operation module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230360_table43157872115614"></a>
<table><thead align="left"><tr id="en-us_topic_0175230360_row61966233115614"><th class="cellrowborder" valign="top" width="23.87238723872387%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230360_p9118648115614"><a name="en-us_topic_0175230360_p9118648115614"></a><a name="en-us_topic_0175230360_p9118648115614"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="22.94229422942294%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230360_p413017115614"><a name="en-us_topic_0175230360_p413017115614"></a><a name="en-us_topic_0175230360_p413017115614"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="53.185318531853184%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230360_p33454382115614"><a name="en-us_topic_0175230360_p33454382115614"></a><a name="en-us_topic_0175230360_p33454382115614"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230360_row32653983115614"><td class="cellrowborder" rowspan="2" valign="top" width="23.87238723872387%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230360_p27727006115614"><a name="en-us_topic_0175230360_p27727006115614"></a><a name="en-us_topic_0175230360_p27727006115614"></a>Setting the flag bit to 1 or 0</p>
</td>
<td class="cellrowborder" valign="top" width="22.94229422942294%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230360_p6665151352510"><a name="en-us_topic_0175230360_p6665151352510"></a><a name="en-us_topic_0175230360_p6665151352510"></a>LOS_BitmapSet</p>
</td>
<td class="cellrowborder" valign="top" width="53.185318531853184%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230360_p51870782115614"><a name="en-us_topic_0175230360_p51870782115614"></a><a name="en-us_topic_0175230360_p51870782115614"></a>Sets a flag bit of an SW to 1.</p>
</td>
</tr>
<tr id="en-us_topic_0175230360_row64183855115614"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230360_p2156603115614"><a name="en-us_topic_0175230360_p2156603115614"></a><a name="en-us_topic_0175230360_p2156603115614"></a>LOS_BitmapClr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230360_p40467168115614"><a name="en-us_topic_0175230360_p40467168115614"></a><a name="en-us_topic_0175230360_p40467168115614"></a>Clears a flag bit of an SW to 0.</p>
</td>
</tr>
<tr id="en-us_topic_0175230360_row28660194115614"><td class="cellrowborder" rowspan="2" valign="top" width="23.87238723872387%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230360_p39774405115614"><a name="en-us_topic_0175230360_p39774405115614"></a><a name="en-us_topic_0175230360_p39774405115614"></a>Obtaining the MSB with a flag bit of 1</p>
</td>
<td class="cellrowborder" valign="top" width="22.94229422942294%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230360_p501352115614"><a name="en-us_topic_0175230360_p501352115614"></a><a name="en-us_topic_0175230360_p501352115614"></a>LOS_HighBitGet</p>
</td>
<td class="cellrowborder" valign="top" width="53.185318531853184%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230360_p40609523115614"><a name="en-us_topic_0175230360_p40609523115614"></a><a name="en-us_topic_0175230360_p40609523115614"></a>Obtains the MSB with a flag bit of 1 in the SW.</p>
</td>
</tr>
<tr id="en-us_topic_0175230360_row29941391115614"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230360_p17827586115614"><a name="en-us_topic_0175230360_p17827586115614"></a><a name="en-us_topic_0175230360_p17827586115614"></a>LOS_LowBitGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230360_p34748382115614"><a name="en-us_topic_0175230360_p34748382115614"></a><a name="en-us_topic_0175230360_p34748382115614"></a>Obtains the LSB with a flag bit of 1 in the SW.</p>
</td>
</tr>
</tbody>
</table>


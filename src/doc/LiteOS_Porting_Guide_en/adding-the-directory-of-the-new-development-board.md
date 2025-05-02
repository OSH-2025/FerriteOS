# Adding the Directory of the New Development Board<a name="EN-US_TOPIC_0314628525"></a>

The STM32F407 development board uses the STM32F4 chip. For details, see the Cloud\_STM32F429IGTx\_FIRE project code.

Copy the Cloud\_STM32F429IGTx\_FIRE directory in the  **target**  directory of the LiteOS source code, and rename the directory as the name of the new development board, for example, STM32F407\_OpenEdv. The following table lists the subdirectories and files in the STM32F407\_OpenEdv directory. Only the contents related to the porting are listed. Files and directories that are not related to the porting can be deleted.

**Table  1**  Directory structure of the new development board

<a name="table21661548135412"></a>
<table><thead align="left"><tr id="row21661485542"><th class="cellrowborder" valign="top" width="20.369999999999997%" id="mcps1.2.3.1.1"><p id="p838233610550"><a name="p838233610550"></a><a name="p838233610550"></a>Directory or File</p>
</th>
<th class="cellrowborder" valign="top" width="79.63%" id="mcps1.2.3.1.2"><p id="p17777155205513"><a name="p17777155205513"></a><a name="p17777155205513"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row1316624817544"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p15382183612550"><a name="p15382183612550"></a><a name="p15382183612550"></a>Inc</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p87771652145517"><a name="p87771652145517"></a><a name="p87771652145517"></a>Directory of header files for configuring chip peripherals. Only <strong id="b16158105716322"><a name="b16158105716322"></a><a name="b16158105716322"></a>main.h</strong>, <strong id="b14646226330"><a name="b14646226330"></a><a name="b14646226330"></a>stm32f4xx.h</strong>, <strong id="b63901052338"><a name="b63901052338"></a><a name="b63901052338"></a>stm32f4xx_hal_conf.h</strong>, <strong id="b3861118123311"><a name="b3861118123311"></a><a name="b3861118123311"></a>sys_init.h</strong>, <strong id="b27811911103310"><a name="b27811911103310"></a><a name="b27811911103310"></a>system_stm32f4xx.h</strong>, <strong id="b20686214103315"><a name="b20686214103315"></a><a name="b20686214103315"></a>tim.h</strong>, and <strong id="b2429122311331"><a name="b2429122311331"></a><a name="b2429122311331"></a>usart.h</strong> need to be retained.</p>
</td>
</tr>
<tr id="row10166164845419"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p153821036165519"><a name="p153821036165519"></a><a name="p153821036165519"></a>include</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p117777527557"><a name="p117777527557"></a><a name="p117777527557"></a>Directory of LiteOS-related configuration header files</p>
</td>
</tr>
<tr id="row1716714815415"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p73831362557"><a name="p73831362557"></a><a name="p73831362557"></a>os_adapt</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p13777185210556"><a name="p13777185210556"></a><a name="p13777185210556"></a>Interface file for LiteOS adaptation</p>
</td>
</tr>
<tr id="row1516734814544"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p2038353611557"><a name="p2038353611557"></a><a name="p2038353611557"></a>Src</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p147771452155511"><a name="p147771452155511"></a><a name="p147771452155511"></a>Directory of source files for configuring chip peripherals. Only <strong id="b11454143319330"><a name="b11454143319330"></a><a name="b11454143319330"></a>main.c</strong>, <strong id="b29321135143315"><a name="b29321135143315"></a><a name="b29321135143315"></a>system_stm32f4xx.c</strong>, <strong id="b20349113963311"><a name="b20349113963311"></a><a name="b20349113963311"></a>usart.c</strong>, <strong id="b12900124143311"><a name="b12900124143311"></a><a name="b12900124143311"></a>sys_init.c</strong>, <strong id="b15181444153310"><a name="b15181444153310"></a><a name="b15181444153310"></a>user_task.c</strong>, and <strong id="b17556450153311"><a name="b17556450153311"></a><a name="b17556450153311"></a>tim.c</strong> need to be retained.</p>
</td>
</tr>
<tr id="row516754815410"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p1738383615558"><a name="p1738383615558"></a><a name="p1738383615558"></a>config.mk</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p577745211554"><a name="p577745211554"></a><a name="p577745211554"></a>Build configuration file of the current development board project</p>
</td>
</tr>
<tr id="row141671148115413"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p03834365559"><a name="p03834365559"></a><a name="p03834365559"></a>liteos.ld</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p477765245517"><a name="p477765245517"></a><a name="p477765245517"></a>Link file of the current development board project</p>
</td>
</tr>
<tr id="row11167204825415"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p1238303665519"><a name="p1238303665519"></a><a name="p1238303665519"></a>los_startup_gcc.S</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p1577775219559"><a name="p1577775219559"></a><a name="p1577775219559"></a>Chip startup file, including the stack definition</p>
</td>
</tr>
<tr id="row121671948115419"><td class="cellrowborder" valign="top" width="20.369999999999997%" headers="mcps1.2.3.1.1 "><p id="p7383153615514"><a name="p7383153615514"></a><a name="p7383153615514"></a>Makefile</p>
</td>
<td class="cellrowborder" valign="top" width="79.63%" headers="mcps1.2.3.1.2 "><p id="p87772521551"><a name="p87772521551"></a><a name="p87772521551"></a>Makefile of the current development board project</p>
</td>
</tr>
</tbody>
</table>


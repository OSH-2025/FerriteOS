# Porting Directory Structure<a name="EN-US_TOPIC_0314628479"></a>

The following table lists LiteOS source code directories. Directories and files in bold need to be modified during porting.

**Table  1**  LiteOS source code directory

<a name="table82889451213"></a>
<table><thead align="left"><tr id="row20288945132110"><th class="cellrowborder" valign="top" width="17.741774177417742%" id="mcps1.2.4.1.1"><p id="p1179115410228"><a name="p1179115410228"></a><a name="p1179115410228"></a>Level-1 Directory</p>
</th>
<th class="cellrowborder" valign="top" width="18.971897189718973%" id="mcps1.2.4.1.2"><p id="p979134162213"><a name="p979134162213"></a><a name="p979134162213"></a>Level-2 Directory or File</p>
</th>
<th class="cellrowborder" valign="top" width="63.28632863286329%" id="mcps1.2.4.1.3"><p id="p779110419229"><a name="p779110419229"></a><a name="p779110419229"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row128884532117"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p117911944225"><a name="p117911944225"></a><a name="p117911944225"></a>arch</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p079144152213"><a name="p079144152213"></a><a name="p079144152213"></a>Chip architecture support</p>
</td>
</tr>
<tr id="row18288204522115"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p87911348226"><a name="p87911348226"></a><a name="p87911348226"></a>build</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p47918418222"><a name="p47918418222"></a><a name="p47918418222"></a>Configurations and scripts required by the LiteOS build system</p>
</td>
</tr>
<tr id="row12882045132117"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p479194112211"><a name="p479194112211"></a><a name="p479194112211"></a>compat</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p1479154172219"><a name="p1479154172219"></a><a name="p1479154172219"></a>CMSIS-RTOS 1.0 and 2.0 APIs provided by LiteOS</p>
</td>
</tr>
<tr id="row17289445132111"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p1579119411222"><a name="p1579119411222"></a><a name="p1579119411222"></a>components</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p57912492219"><a name="p57912492219"></a><a name="p57912492219"></a>Component code</p>
</td>
</tr>
<tr id="row172891545152114"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p127914410228"><a name="p127914410228"></a><a name="p127914410228"></a>demos</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p47913419221"><a name="p47913419221"></a><a name="p47913419221"></a>Demos of components and kernels</p>
</td>
</tr>
<tr id="row62891045152117"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p137915422218"><a name="p137915422218"></a><a name="p137915422218"></a>doc</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p3791184142220"><a name="p3791184142220"></a><a name="p3791184142220"></a>LiteOS documents</p>
</td>
</tr>
<tr id="row228994552114"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p1879164192218"><a name="p1879164192218"></a><a name="p1879164192218"></a>include</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p7791340229"><a name="p7791340229"></a><a name="p7791340229"></a>Header file of each module in components</p>
</td>
</tr>
<tr id="row5289445172116"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p167919462215"><a name="p167919462215"></a><a name="p167919462215"></a>kernel</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p479113411224"><a name="p479113411224"></a><a name="p479113411224"></a>Kernel code</p>
</td>
</tr>
<tr id="row828984512214"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p117913410228"><a name="p117913410228"></a><a name="p117913410228"></a>lib</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p9791845228"><a name="p9791845228"></a><a name="p9791845228"></a>libc/zlib/posix APIs</p>
</td>
</tr>
<tr id="row0289645142113"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p679118412225"><a name="p679118412225"></a><a name="p679118412225"></a>osdepends</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p207923402215"><a name="p207923402215"></a><a name="p207923402215"></a>Certain OS adaptation APIs provided by LiteOS</p>
</td>
</tr>
<tr id="row1289144562116"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p147921546227"><a name="p147921546227"></a><a name="p147921546227"></a>targets</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p97923412228"><a name="p97923412228"></a><a name="p97923412228"></a>bsp</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p5792148229"><a name="p5792148229"></a><a name="p5792148229"></a>Generic board support package</p>
</td>
</tr>
<tr id="row11289184518218"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p1979294152216"><a name="p1979294152216"></a><a name="p1979294152216"></a>Cloud_STM32F429IGTx_FIRE</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p1679217412224"><a name="p1679217412224"></a><a name="p1679217412224"></a>Development project source code package of the Wildfire STM32F429 (ARM Cortex-M4) development board</p>
</td>
</tr>
<tr id="row74811921201112"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p19491105111126"><a name="p19491105111126"></a><a name="p19491105111126"></a>qemu-virt-a53</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p2491205119128"><a name="p2491205119128"></a><a name="p2491205119128"></a>QEMU development project source code package of Cortex-A53</p>
</td>
</tr>
<tr id="row11928241113"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p204911251101214"><a name="p204911251101214"></a><a name="p204911251101214"></a>realview-pbx-a9</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p1849145151211"><a name="p1849145151211"></a><a name="p1849145151211"></a>QEMU development project source code package of Cortex-A9</p>
</td>
</tr>
<tr id="row4289545152113"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p14911351101212"><a name="p14911351101212"></a><a name="p14911351101212"></a>STM32F072_Nucleo</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p14913513121"><a name="p14913513121"></a><a name="p14913513121"></a>Development project source code package of the STM32F072_Nucleo (ARM Cortex-M0) development board</p>
</td>
</tr>
<tr id="row3269161012113"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p1549118511124"><a name="p1549118511124"></a><a name="p1549118511124"></a>STM32F103_FIRE_Arbitrary</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p749115181220"><a name="p749115181220"></a><a name="p749115181220"></a>Development project source code package of the Wildfire STM32F103 (ARM Cortex-M3) Hegemony development board</p>
</td>
</tr>
<tr id="row196849158116"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p27929414222"><a name="p27929414222"></a><a name="p27929414222"></a>STM32F769IDISCOVERY</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p1679211472218"><a name="p1679211472218"></a><a name="p1679211472218"></a>Development project source code package of the STM32F769IDISCOVERY (ARM Cortex-M7) development board</p>
</td>
</tr>
<tr id="row1426961012116"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p19491145121211"><a name="p19491145121211"></a><a name="p19491145121211"></a>...</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p204921551141213"><a name="p204921551141213"></a><a name="p204921551141213"></a>Development project source code package of other development boards</p>
</td>
</tr>
<tr id="row1128954552110"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p107927418225"><a name="p107927418225"></a><a name="p107927418225"></a>Kconfig</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 ">&nbsp;&nbsp;</td>
</tr>
<tr id="row8289445192115"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p579210442213"><a name="p579210442213"></a><a name="p579210442213"></a>Makefile</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 ">&nbsp;&nbsp;</td>
</tr>
<tr id="row10289845112111"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p479213422211"><a name="p479213422211"></a><a name="p479213422211"></a><strong id="b19561725152314"><a name="b19561725152314"></a><a name="b19561725152314"></a>targets.mk</strong></p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 ">&nbsp;&nbsp;</td>
</tr>
<tr id="row92892045192113"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p10792043220"><a name="p10792043220"></a><a name="p10792043220"></a>tools</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p1179218412224"><a name="p1179218412224"></a><a name="p1179218412224"></a><strong id="b167211359237"><a name="b167211359237"></a><a name="b167211359237"></a>build/config</strong></p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p10792194112219"><a name="p10792194112219"></a><a name="p10792194112219"></a>Build configuration file of each development board supported by LiteOS. When a new development board is ported, the build configuration file of the new development board needs to be added to this directory.</p>
</td>
</tr>
<tr id="row828915455218"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 "><p id="p10792104152216"><a name="p10792104152216"></a><a name="p10792104152216"></a>menuconfig</p>
</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p979220492217"><a name="p979220492217"></a><a name="p979220492217"></a>Menuconfig script required for building LiteOS</p>
</td>
</tr>
<tr id="row122905457212"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p57921245222"><a name="p57921245222"></a><a name="p57921245222"></a>Makefile</p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p17792194182215"><a name="p17792194182215"></a><a name="p17792194182215"></a>Makefile of LiteOS</p>
</td>
</tr>
<tr id="row1929074515214"><td class="cellrowborder" valign="top" width="17.741774177417742%" headers="mcps1.2.4.1.1 "><p id="p197925422216"><a name="p197925422216"></a><a name="p197925422216"></a><strong id="b721613419238"><a name="b721613419238"></a><a name="b721613419238"></a>.config</strong></p>
</td>
<td class="cellrowborder" valign="top" width="18.971897189718973%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="63.28632863286329%" headers="mcps1.2.4.1.3 "><p id="p1879219411225"><a name="p1879219411225"></a><a name="p1879219411225"></a>Build configuration file of the development board. The default value is the configuration file of the Cloud_STM32F429IGTx_FIRE development board. During porting, you need to replace the default configuration file with the build configuration file of the new development board.</p>
</td>
</tr>
</tbody>
</table>

The  **target**  directory stores the supported source code of the development board project. When porting a new development board, you need to add the directory of the development board to the  **target**  directory. For details about the directory structure and code, see the directory of the supported development board. For example:

-   For details about porting STM32F4 series, see the Cloud\_STM32F429IGTx\_FIRE project.
-   For details about porting STM32F7 series, see the STM32F769IDISCOVERY project.
-   For details about porting STM32L4 series, see the STM32L431\_BearPi project.


# Generating the Bare Metal Project Code<a name="EN-US_TOPIC_0314628519"></a>

After the peripheral and project configurations are complete, the bare metal project code is generated, as shown in the following figure.

**Figure  1**  Generate project<a name="fig74894405817"></a>  
![](figures/generate_project.png "generated-project")

The following table lists the directory structure of the generated bare metal project.

**Table  1**  Directory structure of a bare metal project

<a name="table1355043241010"></a>
<table><thead align="left"><tr id="row6550732141014"><th class="cellrowborder" valign="top" width="23.78%" id="mcps1.2.3.1.1"><p id="p955023201015"><a name="p955023201015"></a><a name="p955023201015"></a>Directory or File</p>
</th>
<th class="cellrowborder" valign="top" width="76.22%" id="mcps1.2.3.1.2"><p id="p1551103211018"><a name="p1551103211018"></a><a name="p1551103211018"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="row7551103213106"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p911519532107"><a name="p911519532107"></a><a name="p911519532107"></a>build</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p492631615117"><a name="p492631615117"></a><a name="p492631615117"></a>Directory used to store files generated during building</p>
</td>
</tr>
<tr id="row7551632101015"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p20115115317106"><a name="p20115115317106"></a><a name="p20115115317106"></a>Core</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p169261616141113"><a name="p169261616141113"></a><a name="p169261616141113"></a>User code and basic configuration file of the development board</p>
</td>
</tr>
<tr id="row195511328107"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p1711515391010"><a name="p1711515391010"></a><a name="p1711515391010"></a>Drivers</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p49262169111"><a name="p49262169111"></a><a name="p49262169111"></a>Official STM32 HAL library</p>
</td>
</tr>
<tr id="row14551163281018"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p181151153131010"><a name="p181151153131010"></a><a name="p181151153131010"></a>Makefile</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p1892611671116"><a name="p1892611671116"></a><a name="p1892611671116"></a>Makefile of the bare metal project</p>
</td>
</tr>
<tr id="row14551123210105"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p2115165316101"><a name="p2115165316101"></a><a name="p2115165316101"></a>startup_stm32f407xx.s</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p109261516151116"><a name="p109261516151116"></a><a name="p109261516151116"></a>Chip startup file, including the stack definition</p>
</td>
</tr>
<tr id="row117273425108"><td class="cellrowborder" valign="top" width="23.78%" headers="mcps1.2.3.1.1 "><p id="p51152053131017"><a name="p51152053131017"></a><a name="p51152053131017"></a>STM32F407ZGTx_FLASH.ld</p>
</td>
<td class="cellrowborder" valign="top" width="76.22%" headers="mcps1.2.3.1.2 "><p id="p1792691651118"><a name="p1792691651118"></a><a name="p1792691651118"></a>Link script of the bare metal project</p>
</td>
</tr>
</tbody>
</table>


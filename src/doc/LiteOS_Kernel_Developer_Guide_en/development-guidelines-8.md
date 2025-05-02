# Development Guidelines<a name="EN-US_TOPIC_0311018396"></a>

## Usage Scenarios<a name="en-us_topic_0175230351_section47834271164120"></a>

When an interrupt request is generated, the CPU suspends the current task and responds to the peripheral request. You can apply for an interrupt, register an interrupt handler, and specify the operation to be performed when the CPU responds to an interrupt request.

## Functions<a name="en-us_topic_0175230351_section1659236113710"></a>

The interrupt module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230351_table1194515532475"></a>
<table><thead align="left"><tr id="en-us_topic_0175230351_row16946153174710"><th class="cellrowborder" valign="top" width="19.900000000000002%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230351_p19946175304711"><a name="en-us_topic_0175230351_p19946175304711"></a><a name="en-us_topic_0175230351_p19946175304711"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="18.43%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230351_p49461953174712"><a name="en-us_topic_0175230351_p49461953174712"></a><a name="en-us_topic_0175230351_p49461953174712"></a><strong id="en-us_topic_0175230351_b745683317557"><a name="en-us_topic_0175230351_b745683317557"></a><a name="en-us_topic_0175230351_b745683317557"></a>API</strong></p>
</th>
<th class="cellrowborder" valign="top" width="61.67%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230351_p16946165311479"><a name="en-us_topic_0175230351_p16946165311479"></a><a name="en-us_topic_0175230351_p16946165311479"></a><strong id="en-us_topic_0175230351_b1564093512551"><a name="en-us_topic_0175230351_b1564093512551"></a><a name="en-us_topic_0175230351_b1564093512551"></a>Description</strong></p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230351_row8946155384716"><td class="cellrowborder" rowspan="2" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p294635315474"><a name="en-us_topic_0175230351_p294635315474"></a><a name="en-us_topic_0175230351_p294635315474"></a>Creating and deleting an interrupt</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p129461653194713"><a name="en-us_topic_0175230351_p129461653194713"></a><a name="en-us_topic_0175230351_p129461653194713"></a>LOS_HwiCreate</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p13946753134713"><a name="en-us_topic_0175230351_p13946753134713"></a><a name="en-us_topic_0175230351_p13946753134713"></a>Creates an interrupt, and registers the interrupt number, interrupt trigger mode, interrupt priority, and interrupt handler. When an interrupt is triggered, the handleIrq calls the interrupt handler.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row12946115374712"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p7946205344718"><a name="en-us_topic_0175230351_p7946205344718"></a><a name="en-us_topic_0175230351_p7946205344718"></a>LOS_HwiDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p18946753194718"><a name="en-us_topic_0175230351_p18946753194718"></a><a name="en-us_topic_0175230351_p18946753194718"></a>Deletes interrupts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row139468532470"><td class="cellrowborder" rowspan="3" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p179466536477"><a name="en-us_topic_0175230351_p179466536477"></a><a name="en-us_topic_0175230351_p179466536477"></a>Enabling and disabling all interrupts</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p1094655384720"><a name="en-us_topic_0175230351_p1094655384720"></a><a name="en-us_topic_0175230351_p1094655384720"></a>LOS_IntUnLock</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p194655311479"><a name="en-us_topic_0175230351_p194655311479"></a><a name="en-us_topic_0175230351_p194655311479"></a>Enables all interrupt responses of the current processor.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row194610538477"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p394695374712"><a name="en-us_topic_0175230351_p394695374712"></a><a name="en-us_topic_0175230351_p394695374712"></a>LOS_IntLock</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p894675374717"><a name="en-us_topic_0175230351_p894675374717"></a><a name="en-us_topic_0175230351_p894675374717"></a>Disables all interrupt responses of the current processor.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row894695314477"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p9946155310478"><a name="en-us_topic_0175230351_p9946155310478"></a><a name="en-us_topic_0175230351_p9946155310478"></a>LOS_IntRestore</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p1494615311473"><a name="en-us_topic_0175230351_p1494615311473"></a><a name="en-us_topic_0175230351_p1494615311473"></a>Restores to the status before all interrupts are disabled by using LOS_IntLock.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row1094620535479"><td class="cellrowborder" rowspan="2" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p1383017198493"><a name="en-us_topic_0175230351_p1383017198493"></a><a name="en-us_topic_0175230351_p1383017198493"></a>Enabling and disabling a specified interrupt</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p594615312472"><a name="en-us_topic_0175230351_p594615312472"></a><a name="en-us_topic_0175230351_p594615312472"></a>LOS_HwiDisable</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p14946155334717"><a name="en-us_topic_0175230351_p14946155334717"></a><a name="en-us_topic_0175230351_p14946155334717"></a>Disables an interrupt by configuring the register to prevent the CPU from responding to the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row179464531477"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p294665320471"><a name="en-us_topic_0175230351_p294665320471"></a><a name="en-us_topic_0175230351_p294665320471"></a>LOS_HwiEnable</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p9946853144711"><a name="en-us_topic_0175230351_p9946853144711"></a><a name="en-us_topic_0175230351_p9946853144711"></a>Enables an interrupt by configuring the register to allow the CPU to respond to the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row094635354716"><td class="cellrowborder" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p1894635319479"><a name="en-us_topic_0175230351_p1894635319479"></a><a name="en-us_topic_0175230351_p1894635319479"></a>Setting interrupt priorities</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p1294625364710"><a name="en-us_topic_0175230351_p1294625364710"></a><a name="en-us_topic_0175230351_p1294625364710"></a>LOS_HwiSetPriority</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p1694612537477"><a name="en-us_topic_0175230351_p1694612537477"></a><a name="en-us_topic_0175230351_p1694612537477"></a>Sets the interrupt priority.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row0946105310471"><td class="cellrowborder" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p199461553154712"><a name="en-us_topic_0175230351_p199461553154712"></a><a name="en-us_topic_0175230351_p199461553154712"></a>Triggering interrupts</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p7946205314715"><a name="en-us_topic_0175230351_p7946205314715"></a><a name="en-us_topic_0175230351_p7946205314715"></a>LOS_HwiTrigger</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p18947153154719"><a name="en-us_topic_0175230351_p18947153154719"></a><a name="en-us_topic_0175230351_p18947153154719"></a>Triggers an interrupt by writing data into the related register of the interrupt controller to simulate an external interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row1594735318472"><td class="cellrowborder" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p11947353104717"><a name="en-us_topic_0175230351_p11947353104717"></a><a name="en-us_topic_0175230351_p11947353104717"></a>Clearing the status of the interrupt register</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p169472537475"><a name="en-us_topic_0175230351_p169472537475"></a><a name="en-us_topic_0175230351_p169472537475"></a>LOS_HwiClear</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p494715313472"><a name="en-us_topic_0175230351_p494715313472"></a><a name="en-us_topic_0175230351_p494715313472"></a>Clears the status bit of the interrupt register corresponding to the interrupt number. This API depends on the interrupt controller version and is optional.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row9947753174714"><td class="cellrowborder" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p16947953164719"><a name="en-us_topic_0175230351_p16947953164719"></a><a name="en-us_topic_0175230351_p16947953164719"></a>Inter-core interrupt</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p18947195319476"><a name="en-us_topic_0175230351_p18947195319476"></a><a name="en-us_topic_0175230351_p18947195319476"></a>LOS_HwiSendIpi</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p3947185314716"><a name="en-us_topic_0175230351_p3947185314716"></a><a name="en-us_topic_0175230351_p3947185314716"></a>Sends inter-core interrupts to a specified core. This API depends on the interrupt controller version and CPU architecture, and is supported only in SMP mode.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row18200133895019"><td class="cellrowborder" valign="top" width="19.900000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230351_p142904448296"><a name="en-us_topic_0175230351_p142904448296"></a><a name="en-us_topic_0175230351_p142904448296"></a>Setting the interrupt affinity</p>
</td>
<td class="cellrowborder" valign="top" width="18.43%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230351_p4200113845016"><a name="en-us_topic_0175230351_p4200113845016"></a><a name="en-us_topic_0175230351_p4200113845016"></a>LOS_HwiSetAffinity</p>
</td>
<td class="cellrowborder" valign="top" width="61.67%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230351_p9200193895019"><a name="en-us_topic_0175230351_p9200193895019"></a><a name="en-us_topic_0175230351_p9200193895019"></a>Sets the interrupt affinity, that is, sets the interrupt response to a fixed core. The function is supported only in SMP mode.</p>
</td>
</tr>
</tbody>
</table>

## HWI Error Codes<a name="en-us_topic_0175230351_section15699448173937"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230351_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230351_row2267197395642"><th class="cellrowborder" valign="top" width="5.76%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230351_p1908783195642"><a name="en-us_topic_0175230351_p1908783195642"></a><a name="en-us_topic_0175230351_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="18.12%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230351_p261046995642"><a name="en-us_topic_0175230351_p261046995642"></a><a name="en-us_topic_0175230351_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="13.819999999999999%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230351_p1012144095642"><a name="en-us_topic_0175230351_p1012144095642"></a><a name="en-us_topic_0175230351_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="30.990000000000002%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230351_p1453028795642"><a name="en-us_topic_0175230351_p1453028795642"></a><a name="en-us_topic_0175230351_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="31.31%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230351_p2753561710026"><a name="en-us_topic_0175230351_p2753561710026"></a><a name="en-us_topic_0175230351_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230351_row6366372295642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p5648782795642"><a name="en-us_topic_0175230351_p5648782795642"></a><a name="en-us_topic_0175230351_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p1211123695642"><a name="en-us_topic_0175230351_p1211123695642"></a><a name="en-us_topic_0175230351_p1211123695642"></a>OS_ERRNO_HWI_NUM_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p4148605495642"><a name="en-us_topic_0175230351_p4148605495642"></a><a name="en-us_topic_0175230351_p4148605495642"></a>0x02000900</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p492720095642"><a name="en-us_topic_0175230351_p492720095642"></a><a name="en-us_topic_0175230351_p492720095642"></a>An invalid interrupt number is input when an interrupt is created or deleted.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1579250110026"><a name="en-us_topic_0175230351_p1579250110026"></a><a name="en-us_topic_0175230351_p1579250110026"></a>Check the interrupt number and provide a valid interrupt number.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row4434480695642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p3515954495642"><a name="en-us_topic_0175230351_p3515954495642"></a><a name="en-us_topic_0175230351_p3515954495642"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p2935083995642"><a name="en-us_topic_0175230351_p2935083995642"></a><a name="en-us_topic_0175230351_p2935083995642"></a>OS_ERRNO_HWI_PROC_FUNC_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p2860775495642"><a name="en-us_topic_0175230351_p2860775495642"></a><a name="en-us_topic_0175230351_p2860775495642"></a>0x02000901</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p3552674695642"><a name="en-us_topic_0175230351_p3552674695642"></a><a name="en-us_topic_0175230351_p3552674695642"></a>The input interrupt handler pointer is null during interrupt creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p412423410026"><a name="en-us_topic_0175230351_p412423410026"></a><a name="en-us_topic_0175230351_p412423410026"></a>Input a non-empty interrupt handler pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row5130526095642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p6208537795642"><a name="en-us_topic_0175230351_p6208537795642"></a><a name="en-us_topic_0175230351_p6208537795642"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p6285965595642"><a name="en-us_topic_0175230351_p6285965595642"></a><a name="en-us_topic_0175230351_p6285965595642"></a>OS_ERRNO_HWI_CB_UNAVAILABLE</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p5846730195642"><a name="en-us_topic_0175230351_p5846730195642"></a><a name="en-us_topic_0175230351_p5846730195642"></a>0x02000902</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p3823098095642"><a name="en-us_topic_0175230351_p3823098095642"></a><a name="en-us_topic_0175230351_p3823098095642"></a>No available interrupt resource.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p6562756010026"><a name="en-us_topic_0175230351_p6562756010026"></a><a name="en-us_topic_0175230351_p6562756010026"></a>This error code is not used currently.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row853450895642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p2020651095642"><a name="en-us_topic_0175230351_p2020651095642"></a><a name="en-us_topic_0175230351_p2020651095642"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p2611462995642"><a name="en-us_topic_0175230351_p2611462995642"></a><a name="en-us_topic_0175230351_p2611462995642"></a>OS_ERRNO_HWI_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p3491023395642"><a name="en-us_topic_0175230351_p3491023395642"></a><a name="en-us_topic_0175230351_p3491023395642"></a>0x02000903</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p915662595642"><a name="en-us_topic_0175230351_p915662595642"></a><a name="en-us_topic_0175230351_p915662595642"></a>The memory is insufficient during interrupt creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p63511053020"><a name="en-us_topic_0175230351_p63511053020"></a><a name="en-us_topic_0175230351_p63511053020"></a>Expand the dynamic memory space in either of the following ways:</p>
<a name="en-us_topic_0175230351_ul8816111853016"></a><a name="en-us_topic_0175230351_ul8816111853016"></a><ul id="en-us_topic_0175230351_ul8816111853016"><li>Set a larger system dynamic memory pool. The configuration item is <strong id="en-us_topic_0175230351_b1055525717234"><a name="en-us_topic_0175230351_b1055525717234"></a><a name="en-us_topic_0175230351_b1055525717234"></a>OS_SYS_MEM_SIZE</strong>.</li><li>Release a part of the dynamic memory.</li></ul>
</td>
</tr>
<tr id="en-us_topic_0175230351_row1530076695642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p3140256895642"><a name="en-us_topic_0175230351_p3140256895642"></a><a name="en-us_topic_0175230351_p3140256895642"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p6058010795642"><a name="en-us_topic_0175230351_p6058010795642"></a><a name="en-us_topic_0175230351_p6058010795642"></a>OS_ERRNO_HWI_ALREADY_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p804161495642"><a name="en-us_topic_0175230351_p804161495642"></a><a name="en-us_topic_0175230351_p804161495642"></a>0x02000904</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p4739096995642"><a name="en-us_topic_0175230351_p4739096995642"></a><a name="en-us_topic_0175230351_p4739096995642"></a>The interrupt number to be registered has been created during interrupt creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1195087610026"><a name="en-us_topic_0175230351_p1195087610026"></a><a name="en-us_topic_0175230351_p1195087610026"></a>For a non-shared interrupt number, check whether the input interrupt number has been created. For a shared interrupt number, check whether the device ID that matches the function parameter exists in the linked list of the interrupt number.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row2386554195642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p5406070595642"><a name="en-us_topic_0175230351_p5406070595642"></a><a name="en-us_topic_0175230351_p5406070595642"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p1684095295642"><a name="en-us_topic_0175230351_p1684095295642"></a><a name="en-us_topic_0175230351_p1684095295642"></a>OS_ERRNO_HWI_PRIO_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p2193989295642"><a name="en-us_topic_0175230351_p2193989295642"></a><a name="en-us_topic_0175230351_p2193989295642"></a>0x02000905</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p3230086195642"><a name="en-us_topic_0175230351_p3230086195642"></a><a name="en-us_topic_0175230351_p3230086195642"></a>The input interrupt priority is invalid during interrupt creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p2849689910026"><a name="en-us_topic_0175230351_p2849689910026"></a><a name="en-us_topic_0175230351_p2849689910026"></a>Input a valid interrupt priority. The priority range depends on the hardware and can be configured externally.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row2227229895642"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p5922568095642"><a name="en-us_topic_0175230351_p5922568095642"></a><a name="en-us_topic_0175230351_p5922568095642"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p3255077595642"><a name="en-us_topic_0175230351_p3255077595642"></a><a name="en-us_topic_0175230351_p3255077595642"></a>OS_ERRNO_HWI_MODE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p1936711095642"><a name="en-us_topic_0175230351_p1936711095642"></a><a name="en-us_topic_0175230351_p1936711095642"></a>0x02000906</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p2523210295642"><a name="en-us_topic_0175230351_p2523210295642"></a><a name="en-us_topic_0175230351_p2523210295642"></a>Invalid interrupt mode.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p62917066101048"><a name="en-us_topic_0175230351_p62917066101048"></a><a name="en-us_topic_0175230351_p62917066101048"></a>Input a valid interrupt mode. The value range is from 0 to 1.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row43458669101114"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p30491270101114"><a name="en-us_topic_0175230351_p30491270101114"></a><a name="en-us_topic_0175230351_p30491270101114"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p53873767101114"><a name="en-us_topic_0175230351_p53873767101114"></a><a name="en-us_topic_0175230351_p53873767101114"></a>OS_ERRNO_HWI_FASTMODE_ALREADY_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p1699001101114"><a name="en-us_topic_0175230351_p1699001101114"></a><a name="en-us_topic_0175230351_p1699001101114"></a>0x02000907</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p26494371175025"><a name="en-us_topic_0175230351_p26494371175025"></a><a name="en-us_topic_0175230351_p26494371175025"></a>The interrupt number to be registered has been used a fast interrupt during hard interrupt creation.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p7076302101114"><a name="en-us_topic_0175230351_p7076302101114"></a><a name="en-us_topic_0175230351_p7076302101114"></a>This error code is not used currently.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row12143904101125"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p44132204101125"><a name="en-us_topic_0175230351_p44132204101125"></a><a name="en-us_topic_0175230351_p44132204101125"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p17938739101125"><a name="en-us_topic_0175230351_p17938739101125"></a><a name="en-us_topic_0175230351_p17938739101125"></a>OS_ERRNO_HWI_INTERR</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p43751775101125"><a name="en-us_topic_0175230351_p43751775101125"></a><a name="en-us_topic_0175230351_p43751775101125"></a>0x02000908</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p54232920101125"><a name="en-us_topic_0175230351_p54232920101125"></a><a name="en-us_topic_0175230351_p54232920101125"></a>This API is called in an interrupt.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p30790405101125"><a name="en-us_topic_0175230351_p30790405101125"></a><a name="en-us_topic_0175230351_p30790405101125"></a>This error code is not used currently.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row1350795310248"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p2520881310247"><a name="en-us_topic_0175230351_p2520881310247"></a><a name="en-us_topic_0175230351_p2520881310247"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p2864801210247"><a name="en-us_topic_0175230351_p2864801210247"></a><a name="en-us_topic_0175230351_p2864801210247"></a>OS_ERRNO_HWI_SHARED_ERROR</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p3878766810247"><a name="en-us_topic_0175230351_p3878766810247"></a><a name="en-us_topic_0175230351_p3878766810247"></a>0x02000909</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p5479338810247"><a name="en-us_topic_0175230351_p5479338810247"></a><a name="en-us_topic_0175230351_p5479338810247"></a>Creating an interrupt: <strong id="en-us_topic_0175230351_b39024425170"><a name="en-us_topic_0175230351_b39024425170"></a><a name="en-us_topic_0175230351_b39024425170"></a>hwiMode</strong> specifies that a shared interrupt is created, but the device ID is not set. <strong id="en-us_topic_0175230351_b45031516152314"><a name="en-us_topic_0175230351_b45031516152314"></a><a name="en-us_topic_0175230351_b45031516152314"></a>hwiMode</strong> specifies that a non-shared interrupt is created, but the interrupt number has been used for a shared interrupt before. Or <strong id="en-us_topic_0175230351_b1011542992317"><a name="en-us_topic_0175230351_b1011542992317"></a><a name="en-us_topic_0175230351_b1011542992317"></a>LOSCFG_NO_SHARED_IRQ</strong> is set to <strong id="en-us_topic_0175230351_b106481131112313"><a name="en-us_topic_0175230351_b106481131112313"></a><a name="en-us_topic_0175230351_b106481131112313"></a>yes</strong>, but the input parameter specifies that the shared interrupt is created during interrupt creation.</p>
<p id="en-us_topic_0175230351_p5350228132614"><a name="en-us_topic_0175230351_p5350228132614"></a><a name="en-us_topic_0175230351_p5350228132614"></a>Deleting an interrupt: If a shared interrupt is created based on the setting during the creation of device ID, and the device ID is not set during interrupt deletion, the interrupt cannot be deleted.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p907942010247"><a name="en-us_topic_0175230351_p907942010247"></a><a name="en-us_topic_0175230351_p907942010247"></a>Check the input parameters. During creation, the value of <strong id="en-us_topic_0175230351_b13917617172815"><a name="en-us_topic_0175230351_b13917617172815"></a><a name="en-us_topic_0175230351_b13917617172815"></a>hwiMode</strong> must be the same as that of <strong id="en-us_topic_0175230351_b15221727152817"><a name="en-us_topic_0175230351_b15221727152817"></a><a name="en-us_topic_0175230351_b15221727152817"></a>irqParam</strong>. If the value of <strong id="en-us_topic_0175230351_b35701111296"><a name="en-us_topic_0175230351_b35701111296"></a><a name="en-us_topic_0175230351_b35701111296"></a>hwiMode</strong> is <strong id="en-us_topic_0175230351_b349191511291"><a name="en-us_topic_0175230351_b349191511291"></a><a name="en-us_topic_0175230351_b349191511291"></a>0</strong>, the interrupt is not shared. In this case, the value of <strong id="en-us_topic_0175230351_b124921325142915"><a name="en-us_topic_0175230351_b124921325142915"></a><a name="en-us_topic_0175230351_b124921325142915"></a>irqParam</strong> must be <strong id="en-us_topic_0175230351_b14725828162916"><a name="en-us_topic_0175230351_b14725828162916"></a><a name="en-us_topic_0175230351_b14725828162916"></a>NULL</strong>. When <strong id="en-us_topic_0175230351_b792313337298"><a name="en-us_topic_0175230351_b792313337298"></a><a name="en-us_topic_0175230351_b792313337298"></a>hwiMode</strong> is set to <strong id="en-us_topic_0175230351_b186662381295"><a name="en-us_topic_0175230351_b186662381295"></a><a name="en-us_topic_0175230351_b186662381295"></a>IRQF_SHARD</strong>, the interrupt is shared. In this case, you need to set <strong id="en-us_topic_0175230351_b149941046122914"><a name="en-us_topic_0175230351_b149941046122914"></a><a name="en-us_topic_0175230351_b149941046122914"></a>irqParam</strong> to the device ID. When <strong id="en-us_topic_0175230351_b207406225309"><a name="en-us_topic_0175230351_b207406225309"></a><a name="en-us_topic_0175230351_b207406225309"></a>LOSCFG_NO_SHARED_IRQ</strong> is set to <strong id="en-us_topic_0175230351_b3853924193013"><a name="en-us_topic_0175230351_b3853924193013"></a><a name="en-us_topic_0175230351_b3853924193013"></a>YES</strong>, that is, in non-shared interrupt mode, only non-shared interrupts can be created. During interrupt deletion, the value of <strong id="en-us_topic_0175230351_b1780316571301"><a name="en-us_topic_0175230351_b1780316571301"></a><a name="en-us_topic_0175230351_b1780316571301"></a>irqParam</strong> must be the same as that set during interrupt creation.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row214141974320"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p81427192436"><a name="en-us_topic_0175230351_p81427192436"></a><a name="en-us_topic_0175230351_p81427192436"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p2142121919432"><a name="en-us_topic_0175230351_p2142121919432"></a><a name="en-us_topic_0175230351_p2142121919432"></a>OS_ERRNO_HWI_ARG_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p1614251911438"><a name="en-us_topic_0175230351_p1614251911438"></a><a name="en-us_topic_0175230351_p1614251911438"></a>0x0200090a</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p214231984311"><a name="en-us_topic_0175230351_p214231984311"></a><a name="en-us_topic_0175230351_p214231984311"></a>The input parameter is incorrect during interrupt registration.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1014281911436"><a name="en-us_topic_0175230351_p1014281911436"></a><a name="en-us_topic_0175230351_p1014281911436"></a>This error code is not used currently.</p>
</td>
</tr>
<tr id="en-us_topic_0175230351_row177203124310"><td class="cellrowborder" valign="top" width="5.76%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p167714316439"><a name="en-us_topic_0175230351_p167714316439"></a><a name="en-us_topic_0175230351_p167714316439"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="18.12%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p1077631204312"><a name="en-us_topic_0175230351_p1077631204312"></a><a name="en-us_topic_0175230351_p1077631204312"></a>OS_ERRNO_HWI_HWINUM_UNCREATE</p>
</td>
<td class="cellrowborder" valign="top" width="13.819999999999999%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p1977431124317"><a name="en-us_topic_0175230351_p1977431124317"></a><a name="en-us_topic_0175230351_p1977431124317"></a>0x0200090b</p>
</td>
<td class="cellrowborder" valign="top" width="30.990000000000002%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p1178331114317"><a name="en-us_topic_0175230351_p1178331114317"></a><a name="en-us_topic_0175230351_p1178331114317"></a>In interrupt sharing mode, when an interrupt is deleted, the corresponding device ID cannot be matched in the linked list of the interrupt number.</p>
</td>
<td class="cellrowborder" valign="top" width="31.31%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1178133113436"><a name="en-us_topic_0175230351_p1178133113436"></a><a name="en-us_topic_0175230351_p1178133113436"></a>For a shared interrupt number, check whether the device ID that matches the function parameter exists in the linked list of the interrupt number.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the interrupt module, and the value is 0x09.

## Development Process<a name="en-us_topic_0175230351_section53273340143412"></a>

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Interrupt Management**  to configure the interrupt module.

    <a name="en-us_topic_0175230351_table307549115016"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230351_row6708500315016"><th class="cellrowborder" valign="top" width="22.81771822817718%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230351_p1839509215016"><a name="en-us_topic_0175230351_p1839509215016"></a><a name="en-us_topic_0175230351_p1839509215016"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="27.997200279972006%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230351_p1360747715016"><a name="en-us_topic_0175230351_p1360747715016"></a><a name="en-us_topic_0175230351_p1360747715016"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.138586141385861%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230351_p2846381815016"><a name="en-us_topic_0175230351_p2846381815016"></a><a name="en-us_topic_0175230351_p2846381815016"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.308569143085691%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230351_p2386789915016"><a name="en-us_topic_0175230351_p2386789915016"></a><a name="en-us_topic_0175230351_p2386789915016"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="20.73792620737926%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230351_p5425169815016"><a name="en-us_topic_0175230351_p5425169815016"></a><a name="en-us_topic_0175230351_p5425169815016"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230351_row2853386715016"><td class="cellrowborder" valign="top" width="22.81771822817718%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p6517619115016"><a name="en-us_topic_0175230351_p6517619115016"></a><a name="en-us_topic_0175230351_p6517619115016"></a>LOSCFG_ARCH_INTERRUPT_PREEMPTION</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.997200279972006%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p4478011915016"><a name="en-us_topic_0175230351_p4478011915016"></a><a name="en-us_topic_0175230351_p4478011915016"></a>Whether to enable interrupt nesting.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.138586141385861%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p5143025815447"><a name="en-us_topic_0175230351_p5143025815447"></a><a name="en-us_topic_0175230351_p5143025815447"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.308569143085691%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p6686734915016"><a name="en-us_topic_0175230351_p6686734915016"></a><a name="en-us_topic_0175230351_p6686734915016"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.73792620737926%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p4754618715016"><a name="en-us_topic_0175230351_p4754618715016"></a><a name="en-us_topic_0175230351_p4754618715016"></a>ARMv8, RISC-V</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230351_row15729155514413"><td class="cellrowborder" valign="top" width="22.81771822817718%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p1772955512415"><a name="en-us_topic_0175230351_p1772955512415"></a><a name="en-us_topic_0175230351_p1772955512415"></a>LOSCFG_IRQ_USE_STANDALONE_STACK</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.997200279972006%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p17729755746"><a name="en-us_topic_0175230351_p17729755746"></a><a name="en-us_topic_0175230351_p17729755746"></a>Whether to use independent interrupt stacks.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.138586141385861%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p57293551241"><a name="en-us_topic_0175230351_p57293551241"></a><a name="en-us_topic_0175230351_p57293551241"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.308569143085691%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p1772995510411"><a name="en-us_topic_0175230351_p1772995510411"></a><a name="en-us_topic_0175230351_p1772995510411"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.73792620737926%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1873019557411"><a name="en-us_topic_0175230351_p1873019557411"></a><a name="en-us_topic_0175230351_p1873019557411"></a>It depends on the core. Some platforms may not have this configuration item.</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230351_row65851552164"><td class="cellrowborder" valign="top" width="22.81771822817718%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p195851952663"><a name="en-us_topic_0175230351_p195851952663"></a><a name="en-us_topic_0175230351_p195851952663"></a>LOSCFG_SHARED_IRQ</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.997200279972006%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p15585252968"><a name="en-us_topic_0175230351_p15585252968"></a><a name="en-us_topic_0175230351_p15585252968"></a>Whether to enable interrupt sharing.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.138586141385861%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p205856521362"><a name="en-us_topic_0175230351_p205856521362"></a><a name="en-us_topic_0175230351_p205856521362"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.308569143085691%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p10585652567"><a name="en-us_topic_0175230351_p10585652567"></a><a name="en-us_topic_0175230351_p10585652567"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.73792620737926%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p758514524618"><a name="en-us_topic_0175230351_p758514524618"></a><a name="en-us_topic_0175230351_p758514524618"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230351_row4465140515016"><td class="cellrowborder" valign="top" width="22.81771822817718%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p5547821915016"><a name="en-us_topic_0175230351_p5547821915016"></a><a name="en-us_topic_0175230351_p5547821915016"></a>LOSCFG_PLATFORM_HWI_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.997200279972006%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p6455075115016"><a name="en-us_topic_0175230351_p6455075115016"></a><a name="en-us_topic_0175230351_p6455075115016"></a>Maximum number of used interrupts.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.138586141385861%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p6122837215016"><a name="en-us_topic_0175230351_p6122837215016"></a><a name="en-us_topic_0175230351_p6122837215016"></a>Adapted based on the chip manual.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.308569143085691%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p6055112215016"><a name="en-us_topic_0175230351_p6055112215016"></a><a name="en-us_topic_0175230351_p6055112215016"></a>Adapted based on the chip manual.</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.73792620737926%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p569382415016"><a name="en-us_topic_0175230351_p569382415016"></a><a name="en-us_topic_0175230351_p569382415016"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230351_row179809201138"><td class="cellrowborder" valign="top" width="22.81771822817718%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230351_p89819203131"><a name="en-us_topic_0175230351_p89819203131"></a><a name="en-us_topic_0175230351_p89819203131"></a>LOSCFG_HWI_PRIO_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.997200279972006%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230351_p129811206139"><a name="en-us_topic_0175230351_p129811206139"></a><a name="en-us_topic_0175230351_p129811206139"></a>Number of configurable interrupt priorities.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.138586141385861%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230351_p198102019135"><a name="en-us_topic_0175230351_p198102019135"></a><a name="en-us_topic_0175230351_p198102019135"></a>Adapted based on the chip manual.</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.308569143085691%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230351_p12981192018139"><a name="en-us_topic_0175230351_p12981192018139"></a><a name="en-us_topic_0175230351_p12981192018139"></a>Adapted based on the chip manual.</p>
    </td>
    <td class="cellrowborder" valign="top" width="20.73792620737926%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230351_p1098152010139"><a name="en-us_topic_0175230351_p1098152010139"></a><a name="en-us_topic_0175230351_p1098152010139"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Create an interrupt by calling the LOS\_HwiCreate API.
3.  If the SMP mode is used, call the LOS\_HwiSetAffinity API to set the affinity of the interrupt. Otherwise, go to  [4](#en-us_topic_0175230351_li2638862517612).
4.  <a name="en-us_topic_0175230351_li2638862517612"></a>Enable a specified interrupt by calling the LOS\_HwiEnable API.
5.  Trigger a specified interrupt by calling the LOS\_HwiTrigger API. \(This API simulates an external interrupt by writing data to the related register of the interrupt controller. This step is not required for common peripheral devices.\)
6.  Disable a specified interrupt by calling the LOS\_HwiDisable API. This API is used as required and determines whether to disable the interrupt.
7.  Delete a specified interrupt by calling the LOS\_HwiDelete API. This API is used as required to determine whether to delete the interrupt.


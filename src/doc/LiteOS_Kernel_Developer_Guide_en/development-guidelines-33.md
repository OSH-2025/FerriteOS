# Development Guidelines<a name="EN-US_TOPIC_0311018426"></a>

## Usage Scenarios<a name="en-us_topic_0175230570_section13436514165422"></a>

-   If you want to trigger a timer event only once, create a one-shot timer. When the timer expires, the user-defined callback function is executed.
-   If you want to trigger a timer event periodically, create a periodic timer. When the timer expires, the user-defined callback function is executed.

## Functions<a name="en-us_topic_0175230570_section56429059144646"></a>

The software timer module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

**Table  1** 

<a name="en-us_topic_0175230570_table40995251191116"></a>
<table><thead align="left"><tr id="en-us_topic_0175230570_row4794074191116"><th class="cellrowborder" valign="top" width="21.272127212721273%" id="mcps1.2.4.1.1"><p id="en-us_topic_0175230570_p62695608191128"><a name="en-us_topic_0175230570_p62695608191128"></a><a name="en-us_topic_0175230570_p62695608191128"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="21.362136213621362%" id="mcps1.2.4.1.2"><p id="en-us_topic_0175230570_p45179468191128"><a name="en-us_topic_0175230570_p45179468191128"></a><a name="en-us_topic_0175230570_p45179468191128"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="57.365736573657365%" id="mcps1.2.4.1.3"><p id="en-us_topic_0175230570_p35658285191128"><a name="en-us_topic_0175230570_p35658285191128"></a><a name="en-us_topic_0175230570_p35658285191128"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230570_row21373124191116"><td class="cellrowborder" rowspan="2" valign="top" width="21.272127212721273%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230570_p58786840191814"><a name="en-us_topic_0175230570_p58786840191814"></a><a name="en-us_topic_0175230570_p58786840191814"></a>Timer creation and deletion</p>
</td>
<td class="cellrowborder" valign="top" width="21.362136213621362%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230570_p38654909191116"><a name="en-us_topic_0175230570_p38654909191116"></a><a name="en-us_topic_0175230570_p38654909191116"></a>LOS_SwtmrCreate</p>
</td>
<td class="cellrowborder" valign="top" width="57.365736573657365%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230570_p44039894191116"><a name="en-us_topic_0175230570_p44039894191116"></a><a name="en-us_topic_0175230570_p44039894191116"></a>Creates a software timer, sets the time interval, timer mode, and callback function, and returns the timer ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row60814726191116"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230570_p43221843191116"><a name="en-us_topic_0175230570_p43221843191116"></a><a name="en-us_topic_0175230570_p43221843191116"></a>LOS_SwtmrDelete</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230570_p11308433191116"><a name="en-us_topic_0175230570_p11308433191116"></a><a name="en-us_topic_0175230570_p11308433191116"></a>Deletes a software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row34667038191116"><td class="cellrowborder" rowspan="2" valign="top" width="21.272127212721273%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230570_p17418294192043"><a name="en-us_topic_0175230570_p17418294192043"></a><a name="en-us_topic_0175230570_p17418294192043"></a>Timer start and stop</p>
</td>
<td class="cellrowborder" valign="top" width="21.362136213621362%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230570_p18500437191116"><a name="en-us_topic_0175230570_p18500437191116"></a><a name="en-us_topic_0175230570_p18500437191116"></a>LOS_SwtmrStart</p>
</td>
<td class="cellrowborder" valign="top" width="57.365736573657365%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230570_p22140466191116"><a name="en-us_topic_0175230570_p22140466191116"></a><a name="en-us_topic_0175230570_p22140466191116"></a>Starts a software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row65046469191116"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230570_p24618586191116"><a name="en-us_topic_0175230570_p24618586191116"></a><a name="en-us_topic_0175230570_p24618586191116"></a>LOS_SwtmrStop</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230570_p47948415191116"><a name="en-us_topic_0175230570_p47948415191116"></a><a name="en-us_topic_0175230570_p47948415191116"></a>Stops a software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row2588454612258"><td class="cellrowborder" valign="top" width="21.272127212721273%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230570_p1627345912258"><a name="en-us_topic_0175230570_p1627345912258"></a><a name="en-us_topic_0175230570_p1627345912258"></a>Measurement of remaining ticks prior to timer expiry</p>
</td>
<td class="cellrowborder" valign="top" width="21.362136213621362%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230570_p4308183412258"><a name="en-us_topic_0175230570_p4308183412258"></a><a name="en-us_topic_0175230570_p4308183412258"></a>LOS_SwtmrTimeGet</p>
</td>
<td class="cellrowborder" valign="top" width="57.365736573657365%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230570_p6707649212258"><a name="en-us_topic_0175230570_p6707649212258"></a><a name="en-us_topic_0175230570_p6707649212258"></a>Measures the number of remaining ticks prior to expiry of a software timer.</p>
</td>
</tr>
</tbody>
</table>

## Software Timer Error Codes<a name="en-us_topic_0175230570_section49465627194043"></a>

Error codes are returned for operations that may fail to be performed to quickly locate error causes.

<a name="en-us_topic_0175230570_table6015294495642"></a>
<table><thead align="left"><tr id="en-us_topic_0175230570_row2267197395642"><th class="cellrowborder" valign="top" width="5.34%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230570_p1908783195642"><a name="en-us_topic_0175230570_p1908783195642"></a><a name="en-us_topic_0175230570_p1908783195642"></a>No.</p>
</th>
<th class="cellrowborder" valign="top" width="24.82%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230570_p261046995642"><a name="en-us_topic_0175230570_p261046995642"></a><a name="en-us_topic_0175230570_p261046995642"></a>Definition</p>
</th>
<th class="cellrowborder" valign="top" width="13.3%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230570_p1012144095642"><a name="en-us_topic_0175230570_p1012144095642"></a><a name="en-us_topic_0175230570_p1012144095642"></a>Error Code</p>
</th>
<th class="cellrowborder" valign="top" width="27.01%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230570_p1453028795642"><a name="en-us_topic_0175230570_p1453028795642"></a><a name="en-us_topic_0175230570_p1453028795642"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="29.53%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230570_p2753561710026"><a name="en-us_topic_0175230570_p2753561710026"></a><a name="en-us_topic_0175230570_p2753561710026"></a>Reference Solution</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230570_row6366372295642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p5648782795642"><a name="en-us_topic_0175230570_p5648782795642"></a><a name="en-us_topic_0175230570_p5648782795642"></a>1</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p1211123695642"><a name="en-us_topic_0175230570_p1211123695642"></a><a name="en-us_topic_0175230570_p1211123695642"></a>LOS_ERRNO_SWTMR_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p4148605495642"><a name="en-us_topic_0175230570_p4148605495642"></a><a name="en-us_topic_0175230570_p4148605495642"></a>0x02000300</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p492720095642"><a name="en-us_topic_0175230570_p492720095642"></a><a name="en-us_topic_0175230570_p492720095642"></a>The callback function of the software timer is null.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p1579250110026"><a name="en-us_topic_0175230570_p1579250110026"></a><a name="en-us_topic_0175230570_p1579250110026"></a>Define the callback function of the software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row4434480695642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p3515954495642"><a name="en-us_topic_0175230570_p3515954495642"></a><a name="en-us_topic_0175230570_p3515954495642"></a>2</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p2935083995642"><a name="en-us_topic_0175230570_p2935083995642"></a><a name="en-us_topic_0175230570_p2935083995642"></a>LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p2860775495642"><a name="en-us_topic_0175230570_p2860775495642"></a><a name="en-us_topic_0175230570_p2860775495642"></a>0x02000301</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p3552674695642"><a name="en-us_topic_0175230570_p3552674695642"></a><a name="en-us_topic_0175230570_p3552674695642"></a>The time interval of the software timer is 0.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p412423410026"><a name="en-us_topic_0175230570_p412423410026"></a><a name="en-us_topic_0175230570_p412423410026"></a>Redefine the time interval of the software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row5130526095642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p6208537795642"><a name="en-us_topic_0175230570_p6208537795642"></a><a name="en-us_topic_0175230570_p6208537795642"></a>3</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p14395618185923"><a name="en-us_topic_0175230570_p14395618185923"></a><a name="en-us_topic_0175230570_p14395618185923"></a>LOS_ERRNO_SWTMR_MODE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p5846730195642"><a name="en-us_topic_0175230570_p5846730195642"></a><a name="en-us_topic_0175230570_p5846730195642"></a>0x02000302</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p3823098095642"><a name="en-us_topic_0175230570_p3823098095642"></a><a name="en-us_topic_0175230570_p3823098095642"></a>The mode of the software timer is incorrect.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p6562756010026"><a name="en-us_topic_0175230570_p6562756010026"></a><a name="en-us_topic_0175230570_p6562756010026"></a>Change the mode of the software timer. Value range: [0, 2]</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row853450895642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p2020651095642"><a name="en-us_topic_0175230570_p2020651095642"></a><a name="en-us_topic_0175230570_p2020651095642"></a>4</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p2611462995642"><a name="en-us_topic_0175230570_p2611462995642"></a><a name="en-us_topic_0175230570_p2611462995642"></a>LOS_ERRNO_SWTMR_RET_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p3491023395642"><a name="en-us_topic_0175230570_p3491023395642"></a><a name="en-us_topic_0175230570_p3491023395642"></a>0x02000303</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p915662595642"><a name="en-us_topic_0175230570_p915662595642"></a><a name="en-us_topic_0175230570_p915662595642"></a>The passed-in pointer to the software timer ID is null.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p1423211810026"><a name="en-us_topic_0175230570_p1423211810026"></a><a name="en-us_topic_0175230570_p1423211810026"></a>Define the ID variable and pass in a valid pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row1530076695642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p3140256895642"><a name="en-us_topic_0175230570_p3140256895642"></a><a name="en-us_topic_0175230570_p3140256895642"></a>5</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p6058010795642"><a name="en-us_topic_0175230570_p6058010795642"></a><a name="en-us_topic_0175230570_p6058010795642"></a>LOS_ERRNO_SWTMR_MAXSIZE</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p804161495642"><a name="en-us_topic_0175230570_p804161495642"></a><a name="en-us_topic_0175230570_p804161495642"></a>0x02000304</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p4739096995642"><a name="en-us_topic_0175230570_p4739096995642"></a><a name="en-us_topic_0175230570_p4739096995642"></a>The number of software timers exceeds the maximum value.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p1195087610026"><a name="en-us_topic_0175230570_p1195087610026"></a><a name="en-us_topic_0175230570_p1195087610026"></a>Redefine the maximum number of software timers, or wait until a software timer releases resources.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row2386554195642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p5406070595642"><a name="en-us_topic_0175230570_p5406070595642"></a><a name="en-us_topic_0175230570_p5406070595642"></a>6</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p1684095295642"><a name="en-us_topic_0175230570_p1684095295642"></a><a name="en-us_topic_0175230570_p1684095295642"></a>LOS_ERRNO_SWTMR_ID_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p2193989295642"><a name="en-us_topic_0175230570_p2193989295642"></a><a name="en-us_topic_0175230570_p2193989295642"></a>0x02000305</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p3230086195642"><a name="en-us_topic_0175230570_p3230086195642"></a><a name="en-us_topic_0175230570_p3230086195642"></a>The passed-in software timer ID is incorrect.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p2849689910026"><a name="en-us_topic_0175230570_p2849689910026"></a><a name="en-us_topic_0175230570_p2849689910026"></a>Pass in a valid software timer ID.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row2227229895642"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p5922568095642"><a name="en-us_topic_0175230570_p5922568095642"></a><a name="en-us_topic_0175230570_p5922568095642"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p3255077595642"><a name="en-us_topic_0175230570_p3255077595642"></a><a name="en-us_topic_0175230570_p3255077595642"></a>LOS_ERRNO_SWTMR_NOT_CREATED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p1936711095642"><a name="en-us_topic_0175230570_p1936711095642"></a><a name="en-us_topic_0175230570_p1936711095642"></a>0x02000306</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p2523210295642"><a name="en-us_topic_0175230570_p2523210295642"></a><a name="en-us_topic_0175230570_p2523210295642"></a>No software timer is created.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p62917066101048"><a name="en-us_topic_0175230570_p62917066101048"></a><a name="en-us_topic_0175230570_p62917066101048"></a>Create a software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row43458669101114"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p30491270101114"><a name="en-us_topic_0175230570_p30491270101114"></a><a name="en-us_topic_0175230570_p30491270101114"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p53873767101114"><a name="en-us_topic_0175230570_p53873767101114"></a><a name="en-us_topic_0175230570_p53873767101114"></a>LOS_ERRNO_SWTMR_NO_MEMORY</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p1699001101114"><a name="en-us_topic_0175230570_p1699001101114"></a><a name="en-us_topic_0175230570_p1699001101114"></a>0x02000307</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p3955882919926"><a name="en-us_topic_0175230570_p3955882919926"></a><a name="en-us_topic_0175230570_p3955882919926"></a>The memory is insufficient when the software timer module is initialized.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p60375397191252"><a name="en-us_topic_0175230570_p60375397191252"></a><a name="en-us_topic_0175230570_p60375397191252"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for the software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row12143904101125"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p44132204101125"><a name="en-us_topic_0175230570_p44132204101125"></a><a name="en-us_topic_0175230570_p44132204101125"></a>9</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p17938739101125"><a name="en-us_topic_0175230570_p17938739101125"></a><a name="en-us_topic_0175230570_p17938739101125"></a>LOS_ERRNO_SWTMR_MAXSIZE_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p43751775101125"><a name="en-us_topic_0175230570_p43751775101125"></a><a name="en-us_topic_0175230570_p43751775101125"></a>0x02000308</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p54232920101125"><a name="en-us_topic_0175230570_p54232920101125"></a><a name="en-us_topic_0175230570_p54232920101125"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p60601407191429"><a name="en-us_topic_0175230570_p60601407191429"></a><a name="en-us_topic_0175230570_p60601407191429"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row44185939101121"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p22291345101121"><a name="en-us_topic_0175230570_p22291345101121"></a><a name="en-us_topic_0175230570_p22291345101121"></a>10</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p60768531101121"><a name="en-us_topic_0175230570_p60768531101121"></a><a name="en-us_topic_0175230570_p60768531101121"></a>LOS_ERRNO_SWTMR_HWI_ACTIVE</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p23304002101121"><a name="en-us_topic_0175230570_p23304002101121"></a><a name="en-us_topic_0175230570_p23304002101121"></a>0x02000309</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p8576005101121"><a name="en-us_topic_0175230570_p8576005101121"></a><a name="en-us_topic_0175230570_p8576005101121"></a>A timer is used when the CPU is processing interrupts.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p23567813101121"><a name="en-us_topic_0175230570_p23567813101121"></a><a name="en-us_topic_0175230570_p23567813101121"></a>Modify the source code to ensure that no timer is used when the CPU is processing interrupts.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row25611405101131"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p61257961101131"><a name="en-us_topic_0175230570_p61257961101131"></a><a name="en-us_topic_0175230570_p61257961101131"></a>11</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p62947776101131"><a name="en-us_topic_0175230570_p62947776101131"></a><a name="en-us_topic_0175230570_p62947776101131"></a>LOS_ERRNO_SWTMR_HANDLER_POOL_NO_MEM</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p65605116101131"><a name="en-us_topic_0175230570_p65605116101131"></a><a name="en-us_topic_0175230570_p65605116101131"></a>0x0200030a</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p20573694191652"><a name="en-us_topic_0175230570_p20573694191652"></a><a name="en-us_topic_0175230570_p20573694191652"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p66027202101131"><a name="en-us_topic_0175230570_p66027202101131"></a><a name="en-us_topic_0175230570_p66027202101131"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row61184987101143"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p57036951101143"><a name="en-us_topic_0175230570_p57036951101143"></a><a name="en-us_topic_0175230570_p57036951101143"></a>12</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p56590286101143"><a name="en-us_topic_0175230570_p56590286101143"></a><a name="en-us_topic_0175230570_p56590286101143"></a>LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p20410474101143"><a name="en-us_topic_0175230570_p20410474101143"></a><a name="en-us_topic_0175230570_p20410474101143"></a>0x0200030b</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p42635731101143"><a name="en-us_topic_0175230570_p42635731101143"></a><a name="en-us_topic_0175230570_p42635731101143"></a>The software timer queue fails to be created during the initialization of the software timer.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p17562130104812"><a name="en-us_topic_0175230570_p17562130104812"></a><a name="en-us_topic_0175230570_p17562130104812"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for the software timer queue creation.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row55331775101146"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p52688791101146"><a name="en-us_topic_0175230570_p52688791101146"></a><a name="en-us_topic_0175230570_p52688791101146"></a>13</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p39933692101146"><a name="en-us_topic_0175230570_p39933692101146"></a><a name="en-us_topic_0175230570_p39933692101146"></a>LOS_ERRNO_SWTMR_TASK_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p39289074192041"><a name="en-us_topic_0175230570_p39289074192041"></a><a name="en-us_topic_0175230570_p39289074192041"></a>0x0200030c</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p11950219101146"><a name="en-us_topic_0175230570_p11950219101146"></a><a name="en-us_topic_0175230570_p11950219101146"></a>The software timer task fails to be created during the initialization of the software timer.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p47151913492"><a name="en-us_topic_0175230570_p47151913492"></a><a name="en-us_topic_0175230570_p47151913492"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for the software timer task creation.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row2857798210120"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p3311521410120"><a name="en-us_topic_0175230570_p3311521410120"></a><a name="en-us_topic_0175230570_p3311521410120"></a>14</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p6508665810120"><a name="en-us_topic_0175230570_p6508665810120"></a><a name="en-us_topic_0175230570_p6508665810120"></a>LOS_ERRNO_SWTMR_NOT_STARTED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p3752793910120"><a name="en-us_topic_0175230570_p3752793910120"></a><a name="en-us_topic_0175230570_p3752793910120"></a>0x0200030d</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p59973912192254"><a name="en-us_topic_0175230570_p59973912192254"></a><a name="en-us_topic_0175230570_p59973912192254"></a>The software timer is not started.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p5265929219236"><a name="en-us_topic_0175230570_p5265929219236"></a><a name="en-us_topic_0175230570_p5265929219236"></a>Start a software timer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row4447897810129"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p4602745410129"><a name="en-us_topic_0175230570_p4602745410129"></a><a name="en-us_topic_0175230570_p4602745410129"></a>15</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p3723625410129"><a name="en-us_topic_0175230570_p3723625410129"></a><a name="en-us_topic_0175230570_p3723625410129"></a>LOS_ERRNO_SWTMR_STATUS_INVALID</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p6334657010129"><a name="en-us_topic_0175230570_p6334657010129"></a><a name="en-us_topic_0175230570_p6334657010129"></a>0x0200030e</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p6695266119244"><a name="en-us_topic_0175230570_p6695266119244"></a><a name="en-us_topic_0175230570_p6695266119244"></a>The software timer status is incorrect.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p61350282192418"><a name="en-us_topic_0175230570_p61350282192418"></a><a name="en-us_topic_0175230570_p61350282192418"></a>Check the software timer status.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row6331348110124"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p2811835210124"><a name="en-us_topic_0175230570_p2811835210124"></a><a name="en-us_topic_0175230570_p2811835210124"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p6299406810124"><a name="en-us_topic_0175230570_p6299406810124"></a><a name="en-us_topic_0175230570_p6299406810124"></a>LOS_ERRNO_SWTMR_SORTLIST_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p224588410124"><a name="en-us_topic_0175230570_p224588410124"></a><a name="en-us_topic_0175230570_p224588410124"></a>0x0200030f</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p4769892910124"><a name="en-us_topic_0175230570_p4769892910124"></a><a name="en-us_topic_0175230570_p4769892910124"></a>The error code is not in use.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p3840802610124"><a name="en-us_topic_0175230570_p3840802610124"></a><a name="en-us_topic_0175230570_p3840802610124"></a>-</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row4160002610127"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p1415891810127"><a name="en-us_topic_0175230570_p1415891810127"></a><a name="en-us_topic_0175230570_p1415891810127"></a>17</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p602173510127"><a name="en-us_topic_0175230570_p602173510127"></a><a name="en-us_topic_0175230570_p602173510127"></a>LOS_ERRNO_SWTMR_TICK_PTR_NULL</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p1799856510127"><a name="en-us_topic_0175230570_p1799856510127"></a><a name="en-us_topic_0175230570_p1799856510127"></a>0x02000310</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p401190419321"><a name="en-us_topic_0175230570_p401190419321"></a><a name="en-us_topic_0175230570_p401190419321"></a>The passed-in pointer used for obtaining the number of remaining software timer ticks is null.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p4409730210127"><a name="en-us_topic_0175230570_p4409730210127"></a><a name="en-us_topic_0175230570_p4409730210127"></a>Define a valid variable and pass in a valid pointer.</p>
</td>
</tr>
<tr id="en-us_topic_0175230570_row4522121517596"><td class="cellrowborder" valign="top" width="5.34%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p8522915125913"><a name="en-us_topic_0175230570_p8522915125913"></a><a name="en-us_topic_0175230570_p8522915125913"></a>18</p>
</td>
<td class="cellrowborder" valign="top" width="24.82%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p4522111595915"><a name="en-us_topic_0175230570_p4522111595915"></a><a name="en-us_topic_0175230570_p4522111595915"></a>LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED</p>
</td>
<td class="cellrowborder" valign="top" width="13.3%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p452271525919"><a name="en-us_topic_0175230570_p452271525919"></a><a name="en-us_topic_0175230570_p452271525919"></a>0x02000311</p>
</td>
<td class="cellrowborder" valign="top" width="27.01%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p1522191517594"><a name="en-us_topic_0175230570_p1522191517594"></a><a name="en-us_topic_0175230570_p1522191517594"></a>The software timer linked list fails to be created during the initialization of the software timer.</p>
</td>
<td class="cellrowborder" valign="top" width="29.53%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p12161145652119"><a name="en-us_topic_0175230570_p12161145652119"></a><a name="en-us_topic_0175230570_p12161145652119"></a>Adjust OS_SYS_MEM_SIZE to ensure that sufficient memory is available for the software timer linked list creation.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:**
> 
>For details about the error code definition, see  [Error Codes](development-guidelines-14.md#en-us_topic_0175230332_section29852515161). Bits 8–15 belong to the software timer module, and the value is  **0x03**.

## Development Process<a name="en-us_topic_0175230570_section9158190144721"></a>

The typical software timer development process is as follows:

1.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Software Timer**  to configure the software timer.

    <a name="en-us_topic_0175230570_table32617244171025"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230570_row41225896171025"><th class="cellrowborder" valign="top" width="19.1%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230570_p50963290171025"><a name="en-us_topic_0175230570_p50963290171025"></a><a name="en-us_topic_0175230570_p50963290171025"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="25.66%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230570_p34385849171025"><a name="en-us_topic_0175230570_p34385849171025"></a><a name="en-us_topic_0175230570_p34385849171025"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="23.52%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230570_p19411171414538"><a name="en-us_topic_0175230570_p19411171414538"></a><a name="en-us_topic_0175230570_p19411171414538"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="12.22%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230570_p52666268171025"><a name="en-us_topic_0175230570_p52666268171025"></a><a name="en-us_topic_0175230570_p52666268171025"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="19.5%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230570_p38109296171025"><a name="en-us_topic_0175230570_p38109296171025"></a><a name="en-us_topic_0175230570_p38109296171025"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230570_row54575334171025"><td class="cellrowborder" valign="top" width="19.1%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p58525911171025"><a name="en-us_topic_0175230570_p58525911171025"></a><a name="en-us_topic_0175230570_p58525911171025"></a>LOSCFG_BASE_CORE_SWTMR</p>
    </td>
    <td class="cellrowborder" valign="top" width="25.66%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p11216088171057"><a name="en-us_topic_0175230570_p11216088171057"></a><a name="en-us_topic_0175230570_p11216088171057"></a>Whether to enable the tailoring function of the software timer module.</p>
    </td>
    <td class="cellrowborder" valign="top" width="23.52%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p1319010382532"><a name="en-us_topic_0175230570_p1319010382532"></a><a name="en-us_topic_0175230570_p1319010382532"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.22%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p56826855171025"><a name="en-us_topic_0175230570_p56826855171025"></a><a name="en-us_topic_0175230570_p56826855171025"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p39572571171025"><a name="en-us_topic_0175230570_p39572571171025"></a><a name="en-us_topic_0175230570_p39572571171025"></a>LOSCFG_BASE_IPC_QUEUE</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230570_row58701968171025"><td class="cellrowborder" valign="top" width="19.1%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p57239002171025"><a name="en-us_topic_0175230570_p57239002171025"></a><a name="en-us_topic_0175230570_p57239002171025"></a>LOSCFG_BASE_CORE_SWTMR_LIMIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="25.66%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p5847613171025"><a name="en-us_topic_0175230570_p5847613171025"></a><a name="en-us_topic_0175230570_p5847613171025"></a>Maximum number of supported software timers.</p>
    </td>
    <td class="cellrowborder" valign="top" width="23.52%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p13411111415537"><a name="en-us_topic_0175230570_p13411111415537"></a><a name="en-us_topic_0175230570_p13411111415537"></a>&lt; 65535</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.22%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p47029099171025"><a name="en-us_topic_0175230570_p47029099171025"></a><a name="en-us_topic_0175230570_p47029099171025"></a>1024</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p51260679171025"><a name="en-us_topic_0175230570_p51260679171025"></a><a name="en-us_topic_0175230570_p51260679171025"></a>LOSCFG_BASE_CORE_SWTMR</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230570_row83501594558"><td class="cellrowborder" valign="top" width="19.1%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p33501690553"><a name="en-us_topic_0175230570_p33501690553"></a><a name="en-us_topic_0175230570_p33501690553"></a>LOSCFG_BASE_CORE_SWTMR_IN_ISR</p>
    </td>
    <td class="cellrowborder" valign="top" width="25.66%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p163506915514"><a name="en-us_topic_0175230570_p163506915514"></a><a name="en-us_topic_0175230570_p163506915514"></a>Whether to directly execute the callback function in interrupts.</p>
    </td>
    <td class="cellrowborder" valign="top" width="23.52%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p1435016955514"><a name="en-us_topic_0175230570_p1435016955514"></a><a name="en-us_topic_0175230570_p1435016955514"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.22%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p5350139115520"><a name="en-us_topic_0175230570_p5350139115520"></a><a name="en-us_topic_0175230570_p5350139115520"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p63502095557"><a name="en-us_topic_0175230570_p63502095557"></a><a name="en-us_topic_0175230570_p63502095557"></a>LOSCFG_BASE_CORE_SWTMR</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230570_row2631049132410"><td class="cellrowborder" valign="top" width="19.1%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230570_p9632749192412"><a name="en-us_topic_0175230570_p9632749192412"></a><a name="en-us_topic_0175230570_p9632749192412"></a>LOSCFG_BASE_CORE_TSK_SWTMR_STACK_SIZE</p>
    <p id="en-us_topic_0175230570_p1713150204114"><a name="en-us_topic_0175230570_p1713150204114"></a><a name="en-us_topic_0175230570_p1713150204114"></a>(Location: <strong id="en-us_topic_0175230570_b1622893217410"><a name="en-us_topic_0175230570_b1622893217410"></a><a name="en-us_topic_0175230570_b1622893217410"></a>Kernel</strong> &gt; <strong id="en-us_topic_0175230570_b177561137041"><a name="en-us_topic_0175230570_b177561137041"></a><a name="en-us_topic_0175230570_b177561137041"></a>Basic Config</strong> &gt; <strong id="en-us_topic_0175230570_b36441441141"><a name="en-us_topic_0175230570_b36441441141"></a><a name="en-us_topic_0175230570_b36441441141"></a>Task</strong>)</p>
    </td>
    <td class="cellrowborder" valign="top" width="25.66%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230570_p1263216496241"><a name="en-us_topic_0175230570_p1263216496241"></a><a name="en-us_topic_0175230570_p1263216496241"></a>Size of the software timer task stack.</p>
    </td>
    <td class="cellrowborder" valign="top" width="23.52%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230570_p15632154922412"><a name="en-us_topic_0175230570_p15632154922412"></a><a name="en-us_topic_0175230570_p15632154922412"></a>[LOSCFG_TASK_MIN_STACK_SIZE, OS_SYS_MEM_SIZE)</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.22%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230570_p4632164962419"><a name="en-us_topic_0175230570_p4632164962419"></a><a name="en-us_topic_0175230570_p4632164962419"></a>24576</p>
    </td>
    <td class="cellrowborder" valign="top" width="19.5%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230570_p1063219494249"><a name="en-us_topic_0175230570_p1063219494249"></a><a name="en-us_topic_0175230570_p1063219494249"></a>LOSCFG_BASE_CORE_SWTMR</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Call the LOS\_SwtmrCreate API to create a timer, and set the time interval, timer mode, and callback function after timeout.
3.  Call the LOS\_SwtmrStart API to start a software timer.
4.  Call the LOS\_SwtmrTimeGet API to obtain the remaining number of ticks of a software timer.
5.  Call the LOS\_SwtmrStop API to stop a software timer.
6.  Call the LOS\_SwtmrDelete API to delete a software timer.


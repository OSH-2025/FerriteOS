# Development Guidelines<a name="EN-US_TOPIC_0311018447"></a>

## Usage Scenarios<a name="en-us_topic_0175230453_section2421510815955"></a>

When multiple tasks are performing increasing, decreasing, and exchanging operations on the same memory data, the use of atomic operations will ensure that operation results are predictable.

## Functions<a name="en-us_topic_0175230453_section585006715106"></a>

Atomic data of Huawei LiteOS includes two types: Atomic \(32-bit signed\) and Atomic64 \(64-bit signed\). The atomic operation module of Huawei LiteOS provides the following functions:

**Table  1**  Atomic operation functions

<a name="en-us_topic_0175230453_table423153820811"></a>
<table><thead align="left"><tr id="en-us_topic_0175230453_row1872972720811"><th class="cellrowborder" valign="top" width="17.588241175882413%" id="mcps1.2.4.1.1"><p id="en-us_topic_0175230453_p2197741720811"><a name="en-us_topic_0175230453_p2197741720811"></a><a name="en-us_topic_0175230453_p2197741720811"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="22.827717228277173%" id="mcps1.2.4.1.2"><p id="en-us_topic_0175230453_p1280752020811"><a name="en-us_topic_0175230453_p1280752020811"></a><a name="en-us_topic_0175230453_p1280752020811"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="59.58404159584042%" id="mcps1.2.4.1.3"><p id="en-us_topic_0175230453_p3595409320811"><a name="en-us_topic_0175230453_p3595409320811"></a><a name="en-us_topic_0175230453_p3595409320811"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230453_row8821422144017"><td class="cellrowborder" valign="top" width="17.588241175882413%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1082116226408"><a name="en-us_topic_0175230453_p1082116226408"></a><a name="en-us_topic_0175230453_p1082116226408"></a>Read</p>
</td>
<td class="cellrowborder" valign="top" width="22.827717228277173%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p118211822134019"><a name="en-us_topic_0175230453_p118211822134019"></a><a name="en-us_topic_0175230453_p118211822134019"></a>LOS_AtomicRead</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p3821172210403"><a name="en-us_topic_0175230453_p3821172210403"></a><a name="en-us_topic_0175230453_p3821172210403"></a>Reads memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row12907201494015"><td class="cellrowborder" valign="top" width="17.588241175882413%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1790851494014"><a name="en-us_topic_0175230453_p1790851494014"></a><a name="en-us_topic_0175230453_p1790851494014"></a>Write</p>
</td>
<td class="cellrowborder" valign="top" width="22.827717228277173%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p79081214184016"><a name="en-us_topic_0175230453_p79081214184016"></a><a name="en-us_topic_0175230453_p79081214184016"></a>LOS_AtomicSet</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p1908314114017"><a name="en-us_topic_0175230453_p1908314114017"></a><a name="en-us_topic_0175230453_p1908314114017"></a>Writes memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row3711131920811"><td class="cellrowborder" rowspan="4" valign="top" width="17.588241175882413%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1464320720811"><a name="en-us_topic_0175230453_p1464320720811"></a><a name="en-us_topic_0175230453_p1464320720811"></a>Add</p>
</td>
<td class="cellrowborder" valign="top" width="22.827717228277173%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p5788323720811"><a name="en-us_topic_0175230453_p5788323720811"></a><a name="en-us_topic_0175230453_p5788323720811"></a>LOS_AtomicAdd</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p6073589620811"><a name="en-us_topic_0175230453_p6073589620811"></a><a name="en-us_topic_0175230453_p6073589620811"></a>Adds data to the memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row15982181955512"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p159821019205512"><a name="en-us_topic_0175230453_p159821019205512"></a><a name="en-us_topic_0175230453_p159821019205512"></a>LOS_AtomicSub</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p189821319125516"><a name="en-us_topic_0175230453_p189821319125516"></a><a name="en-us_topic_0175230453_p189821319125516"></a>Subtracts the memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row3156887520811"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p6009666520811"><a name="en-us_topic_0175230453_p6009666520811"></a><a name="en-us_topic_0175230453_p6009666520811"></a>LOS_AtomicInc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p6683076820811"><a name="en-us_topic_0175230453_p6683076820811"></a><a name="en-us_topic_0175230453_p6683076820811"></a>Adds one to the memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row152478191611"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p487389551611"><a name="en-us_topic_0175230453_p487389551611"></a><a name="en-us_topic_0175230453_p487389551611"></a>LOS_AtomicIncRet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p4737212173750"><a name="en-us_topic_0175230453_p4737212173750"></a><a name="en-us_topic_0175230453_p4737212173750"></a>Adds one to the memory data and returns the result.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row551826141617"><td class="cellrowborder" rowspan="2" valign="top" width="17.588241175882413%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1025291220215"><a name="en-us_topic_0175230453_p1025291220215"></a><a name="en-us_topic_0175230453_p1025291220215"></a>Subtract</p>
</td>
<td class="cellrowborder" valign="top" width="22.827717228277173%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p8139021617"><a name="en-us_topic_0175230453_p8139021617"></a><a name="en-us_topic_0175230453_p8139021617"></a>LOS_AtomicDec</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p659261051617"><a name="en-us_topic_0175230453_p659261051617"></a><a name="en-us_topic_0175230453_p659261051617"></a>Subtracts one from the memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row5082721316112"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1340185216112"><a name="en-us_topic_0175230453_p1340185216112"></a><a name="en-us_topic_0175230453_p1340185216112"></a>LOS_AtomicDecRet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p1180824516112"><a name="en-us_topic_0175230453_p1180824516112"></a><a name="en-us_topic_0175230453_p1180824516112"></a>Subtracts one from the memory data and return the result.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row11187400233110"><td class="cellrowborder" rowspan="2" valign="top" width="17.588241175882413%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1353275352817"><a name="en-us_topic_0175230453_p1353275352817"></a><a name="en-us_topic_0175230453_p1353275352817"></a>Swap</p>
</td>
<td class="cellrowborder" valign="top" width="22.827717228277173%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p50546033233110"><a name="en-us_topic_0175230453_p50546033233110"></a><a name="en-us_topic_0175230453_p50546033233110"></a>LOS_AtomicXchg32bits</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p588031233110"><a name="en-us_topic_0175230453_p588031233110"></a><a name="en-us_topic_0175230453_p588031233110"></a>Swaps the memory data. The original value in the memory is returned as a return value.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row4980729516444"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p3260399916444"><a name="en-us_topic_0175230453_p3260399916444"></a><a name="en-us_topic_0175230453_p3260399916444"></a>LOS_AtomicCmpXchg32bits</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p2367825216444"><a name="en-us_topic_0175230453_p2367825216444"></a><a name="en-us_topic_0175230453_p2367825216444"></a>Compares and swaps memory data, and returns the comparison result.</p>
</td>
</tr>
</tbody>
</table>

**Table  2**  Atomic64 atomic operations

<a name="en-us_topic_0175230453_table14459125365612"></a>
<table><thead align="left"><tr id="en-us_topic_0175230453_row11459105325613"><th class="cellrowborder" valign="top" width="17.488251174882514%" id="mcps1.2.4.1.1"><p id="en-us_topic_0175230453_p15459185355610"><a name="en-us_topic_0175230453_p15459185355610"></a><a name="en-us_topic_0175230453_p15459185355610"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="22.927707229277072%" id="mcps1.2.4.1.2"><p id="en-us_topic_0175230453_p44591453115611"><a name="en-us_topic_0175230453_p44591453115611"></a><a name="en-us_topic_0175230453_p44591453115611"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="59.58404159584042%" id="mcps1.2.4.1.3"><p id="en-us_topic_0175230453_p24591253165610"><a name="en-us_topic_0175230453_p24591253165610"></a><a name="en-us_topic_0175230453_p24591253165610"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230453_row184591853145610"><td class="cellrowborder" valign="top" width="17.488251174882514%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p17459195315612"><a name="en-us_topic_0175230453_p17459195315612"></a><a name="en-us_topic_0175230453_p17459195315612"></a>Read</p>
</td>
<td class="cellrowborder" valign="top" width="22.927707229277072%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p134591653155613"><a name="en-us_topic_0175230453_p134591653155613"></a><a name="en-us_topic_0175230453_p134591653155613"></a>LOS_Atomic64Read</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p545915531560"><a name="en-us_topic_0175230453_p545915531560"></a><a name="en-us_topic_0175230453_p545915531560"></a>Reads the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row2459195325618"><td class="cellrowborder" valign="top" width="17.488251174882514%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p44592053105614"><a name="en-us_topic_0175230453_p44592053105614"></a><a name="en-us_topic_0175230453_p44592053105614"></a>Write</p>
</td>
<td class="cellrowborder" valign="top" width="22.927707229277072%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p594319452578"><a name="en-us_topic_0175230453_p594319452578"></a><a name="en-us_topic_0175230453_p594319452578"></a>LOS_Atomic64Set</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p14596533569"><a name="en-us_topic_0175230453_p14596533569"></a><a name="en-us_topic_0175230453_p14596533569"></a>Writes the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row8459195313562"><td class="cellrowborder" rowspan="4" valign="top" width="17.488251174882514%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1459753145613"><a name="en-us_topic_0175230453_p1459753145613"></a><a name="en-us_topic_0175230453_p1459753145613"></a>Add</p>
</td>
<td class="cellrowborder" valign="top" width="22.927707229277072%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p134591253195618"><a name="en-us_topic_0175230453_p134591253195618"></a><a name="en-us_topic_0175230453_p134591253195618"></a>LOS_Atomic64Add</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p5459165318568"><a name="en-us_topic_0175230453_p5459165318568"></a><a name="en-us_topic_0175230453_p5459165318568"></a>Adds data to the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row345965325611"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p17459753165610"><a name="en-us_topic_0175230453_p17459753165610"></a><a name="en-us_topic_0175230453_p17459753165610"></a>LOS_Atomic64Sub</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p194591053165610"><a name="en-us_topic_0175230453_p194591053165610"></a><a name="en-us_topic_0175230453_p194591053165610"></a>Subtracts the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row144591153135611"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p2459125317562"><a name="en-us_topic_0175230453_p2459125317562"></a><a name="en-us_topic_0175230453_p2459125317562"></a>LOS_Atomic64Inc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p04591153165619"><a name="en-us_topic_0175230453_p04591153165619"></a><a name="en-us_topic_0175230453_p04591153165619"></a>Adds one to the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row114591653145617"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p645955317563"><a name="en-us_topic_0175230453_p645955317563"></a><a name="en-us_topic_0175230453_p645955317563"></a>LOS_Atomic64IncRet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p16459953185614"><a name="en-us_topic_0175230453_p16459953185614"></a><a name="en-us_topic_0175230453_p16459953185614"></a>Adds one to the 64-bit memory data and returns the operation result.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row745995310561"><td class="cellrowborder" rowspan="2" valign="top" width="17.488251174882514%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p1645935315612"><a name="en-us_topic_0175230453_p1645935315612"></a><a name="en-us_topic_0175230453_p1645935315612"></a>Subtract</p>
</td>
<td class="cellrowborder" valign="top" width="22.927707229277072%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p1045955365615"><a name="en-us_topic_0175230453_p1045955365615"></a><a name="en-us_topic_0175230453_p1045955365615"></a>LOS_Atomic64Dec</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p84596533569"><a name="en-us_topic_0175230453_p84596533569"></a><a name="en-us_topic_0175230453_p84596533569"></a>Subtracts one from the 64-bit memory data.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row18459195365617"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p645975315561"><a name="en-us_topic_0175230453_p645975315561"></a><a name="en-us_topic_0175230453_p645975315561"></a>LOS_Atomic64DecRet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p124599539568"><a name="en-us_topic_0175230453_p124599539568"></a><a name="en-us_topic_0175230453_p124599539568"></a>Subtracts one to the 64-bit memory data and returns the operation result.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row1645905355610"><td class="cellrowborder" rowspan="2" valign="top" width="17.488251174882514%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p945914530567"><a name="en-us_topic_0175230453_p945914530567"></a><a name="en-us_topic_0175230453_p945914530567"></a>Swap</p>
</td>
<td class="cellrowborder" valign="top" width="22.927707229277072%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p194915482214"><a name="en-us_topic_0175230453_p194915482214"></a><a name="en-us_topic_0175230453_p194915482214"></a>LOS_AtomicXchg64bits</p>
</td>
<td class="cellrowborder" valign="top" width="59.58404159584042%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0175230453_p13459175315562"><a name="en-us_topic_0175230453_p13459175315562"></a><a name="en-us_topic_0175230453_p13459175315562"></a>Swaps the 64-bit memory data. The original value in the memory is returned as a return value.</p>
</td>
</tr>
<tr id="en-us_topic_0175230453_row17459115345618"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0175230453_p10459195312568"><a name="en-us_topic_0175230453_p10459195312568"></a><a name="en-us_topic_0175230453_p10459195312568"></a>LOS_AtomicCmpXchg64bits</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0175230453_p1445925315614"><a name="en-us_topic_0175230453_p1445925315614"></a><a name="en-us_topic_0175230453_p1445925315614"></a>Compares and swaps the 64-bit memory data and returns the comparison result.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>In an atomic operation, the number of operands and their results cannot exceed the maximum number of bits supported by the function.

## Platform Differences<a name="en-us_topic_0175230453_section22034412151019"></a>

None


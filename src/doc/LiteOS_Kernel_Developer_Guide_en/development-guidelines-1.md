# Development Guidelines<a name="EN-US_TOPIC_0311018388"></a>

## Usage Scenarios<a name="en-us_topic_0175230634_section64018741114726"></a>

The main task of memory management is to dynamically partition and manage user allocated memory intervals.

Dynamic memory management is used when users have different demands on memory blocks. When a user needs memory, the system calls the LOS\_MemAlloc API to allocate the requested amount of memory. When the user no longer needs the memory, the system calls the LOS\_MemFree API to free it up.

## Functions<a name="en-us_topic_0175230634_section10503426163114"></a>

The dynamic memory management module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175230634_table6587685163114"></a>
<table><thead align="left"><tr id="en-us_topic_0175230634_row31169589163114"><th class="cellrowborder" valign="top" width="22.720000000000002%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230634_p41708771163114"><a name="en-us_topic_0175230634_p41708771163114"></a><a name="en-us_topic_0175230634_p41708771163114"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="23.29%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230634_p22967294163114"><a name="en-us_topic_0175230634_p22967294163114"></a><a name="en-us_topic_0175230634_p22967294163114"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="53.99%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230634_p48411501163114"><a name="en-us_topic_0175230634_p48411501163114"></a><a name="en-us_topic_0175230634_p48411501163114"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230634_row29017497163114"><td class="cellrowborder" rowspan="2" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p1607017163114"><a name="en-us_topic_0175230634_p1607017163114"></a><a name="en-us_topic_0175230634_p1607017163114"></a>Memory pool initialization and deletion</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p63059525163114"><a name="en-us_topic_0175230634_p63059525163114"></a><a name="en-us_topic_0175230634_p63059525163114"></a>LOS_MemInit</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p7547926163114"><a name="en-us_topic_0175230634_p7547926163114"></a><a name="en-us_topic_0175230634_p7547926163114"></a>Initializes a dynamic memory pool of the required size.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row566665491512"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p61270991512"><a name="en-us_topic_0175230634_p61270991512"></a><a name="en-us_topic_0175230634_p61270991512"></a>LOS_MemDeInit</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p4962945391512"><a name="en-us_topic_0175230634_p4962945391512"></a><a name="en-us_topic_0175230634_p4962945391512"></a>Deletes a memory pool. This function takes effect only when <strong id="en-us_topic_0175230634_b157232074103"><a name="en-us_topic_0175230634_b157232074103"></a><a name="en-us_topic_0175230634_b157232074103"></a>LOSCFG_MEM_MUL_POOL</strong> is enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row822478163114"><td class="cellrowborder" rowspan="4" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p66620777163114"><a name="en-us_topic_0175230634_p66620777163114"></a><a name="en-us_topic_0175230634_p66620777163114"></a>Dynamic memory application and free-up</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p27573869163114"><a name="en-us_topic_0175230634_p27573869163114"></a><a name="en-us_topic_0175230634_p27573869163114"></a>LOS_MemAlloc</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p18890880163114"><a name="en-us_topic_0175230634_p18890880163114"></a><a name="en-us_topic_0175230634_p18890880163114"></a>Applies for a memory block of the required size from a dynamic memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row35800200163114"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p4092108163114"><a name="en-us_topic_0175230634_p4092108163114"></a><a name="en-us_topic_0175230634_p4092108163114"></a>LOS_MemFree</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p63025325163114"><a name="en-us_topic_0175230634_p63025325163114"></a><a name="en-us_topic_0175230634_p63025325163114"></a>Frees up the applied memory.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row30357013163114"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p146891489236"><a name="en-us_topic_0175230634_p146891489236"></a><a name="en-us_topic_0175230634_p146891489236"></a>LOS_MemRealloc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p57857274163114"><a name="en-us_topic_0175230634_p57857274163114"></a><a name="en-us_topic_0175230634_p57857274163114"></a>Re-allocates a memory block of the required size and copies content from the original block. If the new memory block is successfully applied for, the original one will be freed up.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row10493717105921"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p66618278105921"><a name="en-us_topic_0175230634_p66618278105921"></a><a name="en-us_topic_0175230634_p66618278105921"></a>LOS_MemAllocAlign</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p27371466105921"><a name="en-us_topic_0175230634_p27371466105921"></a><a name="en-us_topic_0175230634_p27371466105921"></a>Applies for a memory block of the required size whose address is aligned based on <strong id="en-us_topic_0175230634_b5324164391717"><a name="en-us_topic_0175230634_b5324164391717"></a><a name="en-us_topic_0175230634_b5324164391717"></a>boundary</strong>, from a dynamic memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row65919960151855"><td class="cellrowborder" rowspan="4" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p32965141173528"><a name="en-us_topic_0175230634_p32965141173528"></a><a name="en-us_topic_0175230634_p32965141173528"></a>Memory pool information obtainment</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p653309781946"><a name="en-us_topic_0175230634_p653309781946"></a><a name="en-us_topic_0175230634_p653309781946"></a>LOS_MemPoolSizeGet</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p573178561946"><a name="en-us_topic_0175230634_p573178561946"></a><a name="en-us_topic_0175230634_p573178561946"></a>Obtains the total size of a dynamic memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row26447908151914"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p236106141946"><a name="en-us_topic_0175230634_p236106141946"></a><a name="en-us_topic_0175230634_p236106141946"></a>LOS_MemTotalUsedGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p334115431946"><a name="en-us_topic_0175230634_p334115431946"></a><a name="en-us_topic_0175230634_p334115431946"></a>Obtains the total usage of a dynamic memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row1024153745918"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p426412397596"><a name="en-us_topic_0175230634_p426412397596"></a><a name="en-us_topic_0175230634_p426412397596"></a>LOS_MemInfoGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p11264133955914"><a name="en-us_topic_0175230634_p11264133955914"></a><a name="en-us_topic_0175230634_p11264133955914"></a>Obtains the structure of a memory pool, including the size of idle memory, size of used memory, number of idle memory blocks, number of used memory blocks, and maximum size of idle memory blocks.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row52711619016"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p1346817710012"><a name="en-us_topic_0175230634_p1346817710012"></a><a name="en-us_topic_0175230634_p1346817710012"></a>LOS_MemPoolList</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p54681775011"><a name="en-us_topic_0175230634_p54681775011"></a><a name="en-us_topic_0175230634_p54681775011"></a>Prints information about all initialized memory pools in the system, including the start addresses of the memory pools, size of the memory pools, total size of idle memory, total size of used memory, maximum size of idle memory blocks, number of idle memory blocks, and number of used memory blocks. This function takes effect only when <strong id="en-us_topic_0175230634_b188035240227"><a name="en-us_topic_0175230634_b188035240227"></a><a name="en-us_topic_0175230634_b188035240227"></a>LOSCFG_MEM_MUL_POOL</strong> is enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row27533383151920"><td class="cellrowborder" rowspan="6" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p57507755173528"><a name="en-us_topic_0175230634_p57507755173528"></a><a name="en-us_topic_0175230634_p57507755173528"></a>Memory block information obtainment</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p385951231946"><a name="en-us_topic_0175230634_p385951231946"></a><a name="en-us_topic_0175230634_p385951231946"></a>LOS_MemFreeBlksGet</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p391972821946"><a name="en-us_topic_0175230634_p391972821946"></a><a name="en-us_topic_0175230634_p391972821946"></a>Obtains the number of idle memory blocks in a memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row62236556151924"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p377899081946"><a name="en-us_topic_0175230634_p377899081946"></a><a name="en-us_topic_0175230634_p377899081946"></a>LOS_MemUsedBlksGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p410837221946"><a name="en-us_topic_0175230634_p410837221946"></a><a name="en-us_topic_0175230634_p410837221946"></a>Obtains the number of used memory blocks in a memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row54209345151927"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p357380731946"><a name="en-us_topic_0175230634_p357380731946"></a><a name="en-us_topic_0175230634_p357380731946"></a>LOS_MemTaskIdGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p91028161946"><a name="en-us_topic_0175230634_p91028161946"></a><a name="en-us_topic_0175230634_p91028161946"></a>Obtains the ID of the task to which a memory block is allocated.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row66231348151931"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p11791118202635"><a name="en-us_topic_0175230634_p11791118202635"></a><a name="en-us_topic_0175230634_p11791118202635"></a>LOS_MemLastUsedGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p413438061946"><a name="en-us_topic_0175230634_p413438061946"></a><a name="en-us_topic_0175230634_p413438061946"></a>Obtains the end address of the last used memory block in a memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row152596298113"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p846713011116"><a name="en-us_topic_0175230634_p846713011116"></a><a name="en-us_topic_0175230634_p846713011116"></a>LOS_MemNodeSizeCheck</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p104671930316"><a name="en-us_topic_0175230634_p104671930316"></a><a name="en-us_topic_0175230634_p104671930316"></a>Obtains the total size of a memory block and the size of idle memory in it. This function takes effect only when <strong id="en-us_topic_0175230634_b8571192712247"><a name="en-us_topic_0175230634_b8571192712247"></a><a name="en-us_topic_0175230634_b8571192712247"></a>LOSCFG_BASE_MEM_NODE_SIZE_CHECK</strong> is enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row2341113518110"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p9434155711113"><a name="en-us_topic_0175230634_p9434155711113"></a><a name="en-us_topic_0175230634_p9434155711113"></a>LOS_MemFreeNodeShow</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p10434125712110"><a name="en-us_topic_0175230634_p10434125712110"></a><a name="en-us_topic_0175230634_p10434125712110"></a>Prints the size and number of idle memory blocks in a memory pool.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row44023001151937"><td class="cellrowborder" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p13193167173528"><a name="en-us_topic_0175230634_p13193167173528"></a><a name="en-us_topic_0175230634_p13193167173528"></a>Memory pool integrity check</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p200304671946"><a name="en-us_topic_0175230634_p200304671946"></a><a name="en-us_topic_0175230634_p200304671946"></a>LOS_MemIntegrityCheck</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p118551301946"><a name="en-us_topic_0175230634_p118551301946"></a><a name="en-us_topic_0175230634_p118551301946"></a>Checks the integrity of a memory pool. This function takes effect only when <strong id="en-us_topic_0175230634_b7571340202615"><a name="en-us_topic_0175230634_b7571340202615"></a><a name="en-us_topic_0175230634_b7571340202615"></a>LOSCFG_BASE_MEM_NODE_INTEGRITY_CHECK</strong> is enabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row10513314151950"><td class="cellrowborder" rowspan="2" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p57271543173528"><a name="en-us_topic_0175230634_p57271543173528"></a><a name="en-us_topic_0175230634_p57271543173528"></a>Sets and obtains the memory check level. This function takes effect only when <strong id="en-us_topic_0175230634_b20333359192617"><a name="en-us_topic_0175230634_b20333359192617"></a><a name="en-us_topic_0175230634_b20333359192617"></a>LOSCFG_BASE_MEM_NODE_SIZE_CHECK</strong> is enabled.</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p663627871946"><a name="en-us_topic_0175230634_p663627871946"></a><a name="en-us_topic_0175230634_p663627871946"></a>LOS_MemCheckLevelSet</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p66766791946"><a name="en-us_topic_0175230634_p66766791946"></a><a name="en-us_topic_0175230634_p66766791946"></a>Sets the memory check level.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row57717782151954"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p591081821946"><a name="en-us_topic_0175230634_p591081821946"></a><a name="en-us_topic_0175230634_p591081821946"></a>LOS_MemCheckLevelGet</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p230334311946"><a name="en-us_topic_0175230634_p230334311946"></a><a name="en-us_topic_0175230634_p230334311946"></a>Obtains the memory check level.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row20385185145522"><td class="cellrowborder" rowspan="4" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p40587290145522"><a name="en-us_topic_0175230634_p40587290145522"></a><a name="en-us_topic_0175230634_p40587290145522"></a>Dynamic memory application and free-up for a module (effective only when <strong id="en-us_topic_0175230634_b135501496276"><a name="en-us_topic_0175230634_b135501496276"></a><a name="en-us_topic_0175230634_b135501496276"></a>LOSCFG_MEM_MUL_MODULE</strong> is enabled)</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p66345092145522"><a name="en-us_topic_0175230634_p66345092145522"></a><a name="en-us_topic_0175230634_p66345092145522"></a>LOS_MemMalloc</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p5243399145522"><a name="en-us_topic_0175230634_p5243399145522"></a><a name="en-us_topic_0175230634_p5243399145522"></a>Takes memory out of a specified dynamic memory pool and allocates the memory to a specified module, and records the amount of memory allocated to the module.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row28471100145559"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p34923726145559"><a name="en-us_topic_0175230634_p34923726145559"></a><a name="en-us_topic_0175230634_p34923726145559"></a>LOS_MemMfree</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p10249566145559"><a name="en-us_topic_0175230634_p10249566145559"></a><a name="en-us_topic_0175230634_p10249566145559"></a>Frees up a memory block that has been allocated to a specified module and records the amount of memory that is freed up.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row49838696145554"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p37301726145554"><a name="en-us_topic_0175230634_p37301726145554"></a><a name="en-us_topic_0175230634_p37301726145554"></a>LOS_MemMallocAlign</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p1540943145554"><a name="en-us_topic_0175230634_p1540943145554"></a><a name="en-us_topic_0175230634_p1540943145554"></a>Applies for a memory block of the required size whose address is aligned based on <strong id="en-us_topic_0175230634_b151668083319"><a name="en-us_topic_0175230634_b151668083319"></a><a name="en-us_topic_0175230634_b151668083319"></a>boundary</strong>, from a dynamic memory pool, and records the amount of the applied memory.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row1323406145546"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p25829371145546"><a name="en-us_topic_0175230634_p25829371145546"></a><a name="en-us_topic_0175230634_p25829371145546"></a>LOS_MemMrealloc</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p713816242294"><a name="en-us_topic_0175230634_p713816242294"></a><a name="en-us_topic_0175230634_p713816242294"></a>Re-allocates a memory block of the required size and copies content from the original block, and records the amount of the re-allocated memory. If the new memory block is successfully applied for, the original one will be freed up.</p>
</td>
</tr>
<tr id="en-us_topic_0175230634_row4315071491343"><td class="cellrowborder" valign="top" width="22.720000000000002%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230634_p554693191343"><a name="en-us_topic_0175230634_p554693191343"></a><a name="en-us_topic_0175230634_p554693191343"></a>Module memory usage obtainment</p>
</td>
<td class="cellrowborder" valign="top" width="23.29%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230634_p4664826591343"><a name="en-us_topic_0175230634_p4664826591343"></a><a name="en-us_topic_0175230634_p4664826591343"></a>LOS_MemMusedGet</p>
</td>
<td class="cellrowborder" valign="top" width="53.99%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230634_p2041309191343"><a name="en-us_topic_0175230634_p2041309191343"></a><a name="en-us_topic_0175230634_p2041309191343"></a>Obtains the memory usage of a module. This function takes effect only when <strong id="en-us_topic_0175230634_b133962133518"><a name="en-us_topic_0175230634_b133962133518"></a><a name="en-us_topic_0175230634_b133962133518"></a>LOSCFG_MEM_MUL_MODULE</strong> is enabled.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>-   Debugging functions are provided for dynamic memory. For details, see  [Memory Debugging](../LiteOS_Maintenance_Guide_en/memory-debugging.md).
>-   For the bestfit\_little algorithm, only  [Multi-Memory Pool Mechanism](../LiteOS_Maintenance_Guide_en/multi-memory-pool-mechanism.md)  and  [Memory Validity Check](../LiteOS_Maintenance_Guide_en/memory-validity-check.md)  are supported. Other memory debugging functions are not supported.
>-   Among the preceding APIs, the ones controlled through macro are all related to memory debugging.
>-   After the LOS\_MemRealloc or LOS\_MemMrealloc operation is performed on the memory applied using LOS\_MemAllocAlign or LOS\_MemMallocAlign, the new start address of memory may become unaligned.
>-   For the bestfit\_little algorithm, the LOS\_MemRealloc operation cannot be performed on the memory applied using LOS\_MemAllocAlign. Otherwise, failure will be returned.

## Development Process<a name="en-us_topic_0175230634_section39695240155440"></a>

This section describes the development process of dynamic memory in typical scenarios.

1.  Configure the start address and size of a dynamic memory pool in the  **los\_config.h**  file.

    <a name="en-us_topic_0175230634_table1124543155750"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230634_row32017475155750"><th class="cellrowborder" valign="top" width="19.87198719871987%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230634_p15912448155750"><a name="en-us_topic_0175230634_p15912448155750"></a><a name="en-us_topic_0175230634_p15912448155750"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="27.512751275127513%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230634_p13839897155750"><a name="en-us_topic_0175230634_p13839897155750"></a><a name="en-us_topic_0175230634_p13839897155750"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="11.821182118211821%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230634_p47289871155750"><a name="en-us_topic_0175230634_p47289871155750"></a><a name="en-us_topic_0175230634_p47289871155750"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="27.212721272127215%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230634_p5274313155750"><a name="en-us_topic_0175230634_p5274313155750"></a><a name="en-us_topic_0175230634_p5274313155750"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.58135813581358%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230634_p24566191155750"><a name="en-us_topic_0175230634_p24566191155750"></a><a name="en-us_topic_0175230634_p24566191155750"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230634_row57796099155750"><td class="cellrowborder" valign="top" width="19.87198719871987%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p50972472155750"><a name="en-us_topic_0175230634_p50972472155750"></a><a name="en-us_topic_0175230634_p50972472155750"></a>OS_SYS_MEM_ADDR</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.512751275127513%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p35129590155750"><a name="en-us_topic_0175230634_p35129590155750"></a><a name="en-us_topic_0175230634_p35129590155750"></a>Start address of the system dynamic memory pool.</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.821182118211821%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p26924516155750"><a name="en-us_topic_0175230634_p26924516155750"></a><a name="en-us_topic_0175230634_p26924516155750"></a>[0, n)</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.212721272127215%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p33402154155750"><a name="en-us_topic_0175230634_p33402154155750"></a><a name="en-us_topic_0175230634_p33402154155750"></a>&amp;m_aucSysMem1[0]</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.58135813581358%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p21219953155750"><a name="en-us_topic_0175230634_p21219953155750"></a><a name="en-us_topic_0175230634_p21219953155750"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230634_row58661230155951"><td class="cellrowborder" valign="top" width="19.87198719871987%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p53939171155951"><a name="en-us_topic_0175230634_p53939171155951"></a><a name="en-us_topic_0175230634_p53939171155951"></a>OS_SYS_MEM_SIZE</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.512751275127513%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p6996745155951"><a name="en-us_topic_0175230634_p6996745155951"></a><a name="en-us_topic_0175230634_p6996745155951"></a>Size (in bytes) of the system dynamic memory pool (DDR adaptive).</p>
    </td>
    <td class="cellrowborder" valign="top" width="11.821182118211821%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p7058646112722"><a name="en-us_topic_0175230634_p7058646112722"></a><a name="en-us_topic_0175230634_p7058646112722"></a>[0, n)</p>
    </td>
    <td class="cellrowborder" valign="top" width="27.212721272127215%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p3184774155951"><a name="en-us_topic_0175230634_p3184774155951"></a><a name="en-us_topic_0175230634_p3184774155951"></a>From the end of the BSS segment to the end of the system DDR</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.58135813581358%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p56640174155951"><a name="en-us_topic_0175230634_p56640174155951"></a><a name="en-us_topic_0175230634_p56640174155951"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

    -   **OS\_SYS\_MEM\_ADDR**: In most cases, retain the default value.
    -   **OS\_SYS\_MEM\_SIZE**: In most cases, retain the default value.

2.  Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Memory Management**  to configure the dynamic memory management module.

    <a name="en-us_topic_0175230634_table12268254121010"></a>
    <table><thead align="left"><tr id="en-us_topic_0175230634_row1926718548109"><th class="cellrowborder" valign="top" width="20.632063206320634%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175230634_p3267754171012"><a name="en-us_topic_0175230634_p3267754171012"></a><a name="en-us_topic_0175230634_p3267754171012"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="26.972697269726968%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175230634_p226745417105"><a name="en-us_topic_0175230634_p226745417105"></a><a name="en-us_topic_0175230634_p226745417105"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="13.651365136513652%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175230634_p12267554131019"><a name="en-us_topic_0175230634_p12267554131019"></a><a name="en-us_topic_0175230634_p12267554131019"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="16.051605160516054%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175230634_p92671554121018"><a name="en-us_topic_0175230634_p92671554121018"></a><a name="en-us_topic_0175230634_p92671554121018"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="22.69226922692269%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175230634_p92671554151017"><a name="en-us_topic_0175230634_p92671554151017"></a><a name="en-us_topic_0175230634_p92671554151017"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175230634_row10268754131012"><td class="cellrowborder" valign="top" width="20.632063206320634%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p5268135401015"><a name="en-us_topic_0175230634_p5268135401015"></a><a name="en-us_topic_0175230634_p5268135401015"></a>LOSCFG_KERNEL_MEM_BESTFIT</p>
    </td>
    <td class="cellrowborder" valign="top" width="26.972697269726968%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p142687549103"><a name="en-us_topic_0175230634_p142687549103"></a><a name="en-us_topic_0175230634_p142687549103"></a>Whether to use the bestfit algorithm for memory management.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p226810547108"><a name="en-us_topic_0175230634_p226810547108"></a><a name="en-us_topic_0175230634_p226810547108"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.051605160516054%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p3268135410105"><a name="en-us_topic_0175230634_p3268135410105"></a><a name="en-us_topic_0175230634_p3268135410105"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="22.69226922692269%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p1526845415108"><a name="en-us_topic_0175230634_p1526845415108"></a><a name="en-us_topic_0175230634_p1526845415108"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230634_row1126815549108"><td class="cellrowborder" valign="top" width="20.632063206320634%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p2268195461016"><a name="en-us_topic_0175230634_p2268195461016"></a><a name="en-us_topic_0175230634_p2268195461016"></a>LOSCFG_KERNEL_MEM_BESTFIT_LITTLE</p>
    </td>
    <td class="cellrowborder" valign="top" width="26.972697269726968%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p1268554111013"><a name="en-us_topic_0175230634_p1268554111013"></a><a name="en-us_topic_0175230634_p1268554111013"></a>Whether to use the bestfit_little algorithm for memory management.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p1126812542103"><a name="en-us_topic_0175230634_p1126812542103"></a><a name="en-us_topic_0175230634_p1126812542103"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.051605160516054%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p1626855411010"><a name="en-us_topic_0175230634_p1626855411010"></a><a name="en-us_topic_0175230634_p1626855411010"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="22.69226922692269%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p6268145415103"><a name="en-us_topic_0175230634_p6268145415103"></a><a name="en-us_topic_0175230634_p6268145415103"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230634_row9268205471014"><td class="cellrowborder" valign="top" width="20.632063206320634%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p326855401013"><a name="en-us_topic_0175230634_p326855401013"></a><a name="en-us_topic_0175230634_p326855401013"></a>LOSCFG_KERNEL_MEM_SLAB_EXTENTION</p>
    </td>
    <td class="cellrowborder" valign="top" width="26.972697269726968%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p19268125417102"><a name="en-us_topic_0175230634_p19268125417102"></a><a name="en-us_topic_0175230634_p19268125417102"></a>Whether to enable the slab function to reduce memory fragments at runtime.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p1726865418101"><a name="en-us_topic_0175230634_p1726865418101"></a><a name="en-us_topic_0175230634_p1726865418101"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.051605160516054%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p1826820548108"><a name="en-us_topic_0175230634_p1826820548108"></a><a name="en-us_topic_0175230634_p1826820548108"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="22.69226922692269%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p1726835417101"><a name="en-us_topic_0175230634_p1726835417101"></a><a name="en-us_topic_0175230634_p1726835417101"></a>None</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230634_row172681154171019"><td class="cellrowborder" valign="top" width="20.632063206320634%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p16268954191018"><a name="en-us_topic_0175230634_p16268954191018"></a><a name="en-us_topic_0175230634_p16268954191018"></a>LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE</p>
    </td>
    <td class="cellrowborder" valign="top" width="26.972697269726968%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p17268185411012"><a name="en-us_topic_0175230634_p17268185411012"></a><a name="en-us_topic_0175230634_p17268185411012"></a>Whether to automatically expand slabs by applying for new space from the system memory pool, when their memory space is insufficient.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p22681654191019"><a name="en-us_topic_0175230634_p22681654191019"></a><a name="en-us_topic_0175230634_p22681654191019"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.051605160516054%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p1726855413106"><a name="en-us_topic_0175230634_p1726855413106"></a><a name="en-us_topic_0175230634_p1726855413106"></a>NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="22.69226922692269%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p1826814543103"><a name="en-us_topic_0175230634_p1826814543103"></a><a name="en-us_topic_0175230634_p1826814543103"></a>LOSCFG_KERNEL_MEM_SLAB_EXTENTION</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175230634_row62681554191015"><td class="cellrowborder" valign="top" width="20.632063206320634%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175230634_p2026845441012"><a name="en-us_topic_0175230634_p2026845441012"></a><a name="en-us_topic_0175230634_p2026845441012"></a>LOSCFG_MEM_TASK_STAT</p>
    </td>
    <td class="cellrowborder" valign="top" width="26.972697269726968%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175230634_p626865441010"><a name="en-us_topic_0175230634_p626865441010"></a><a name="en-us_topic_0175230634_p626865441010"></a>Whether to enable task memory statistics.</p>
    </td>
    <td class="cellrowborder" valign="top" width="13.651365136513652%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175230634_p1268205481014"><a name="en-us_topic_0175230634_p1268205481014"></a><a name="en-us_topic_0175230634_p1268205481014"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="16.051605160516054%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175230634_p16268115420102"><a name="en-us_topic_0175230634_p16268115420102"></a><a name="en-us_topic_0175230634_p16268115420102"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="22.69226922692269%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175230634_p52682545103"><a name="en-us_topic_0175230634_p52682545103"></a><a name="en-us_topic_0175230634_p52682545103"></a>LOSCFG_KERNEL_MEM_BESTFIT or LOSCFG_KERNEL_MEM_BESTFIT_LITTLE</p>
    </td>
    </tr>
    </tbody>
    </table>

3.  Call the LOS\_MemInit API to initialize a memory pool.

    As shown in the figure below, an EndNode is generated after the initialization, and all the remaining memory is marked as a FreeNode. EndNode is the last node in the memory pool, and its size is 0.

    ![](figures/dynamic_memory_init.jpg)

4.  Call the LOS\_MemAlloc API to apply for dynamic memory of any size.

    The system determines whether the dynamic memory pool has sufficient memory. If yes, it takes the required size out of the memory pool and returns a pointer. If no, it returns "NULL" instead.

    The LOS\_MemAlloc function can be called three times to create three nodes, namely, UsedA, UsedB, and UsedC with sizeA, sizeB, and sizeC, respectively. There is only one large FreeNode when the memory pool is initialized, so these memory blocks are divided from the FreeNode.

    ![](figures/dynamic_memory_alloc.jpg)

    If there are multiple FreeNodes in the memory pool, the one of the most appropriate size will be used to create a memory block during malloc, which can reduce memory fragments. If the created memory block is not the size of the selected FreeNode, the remaining memory will be marked as a new FreeNode.

5.  Call the LOS\_MemFree API to free up dynamic memory.

    The reclaimed memory block can be used next time.

    Assume that LOS\_MemFree is called to free up UsedB. The reclaimed UsedB block will be marked as a FreeNode. When memory blocks are reclaimed, adjacent FreeNodes are automatically merged.

    ![](figures/free_dynamic_memory.jpg)


## Platform Differences<a name="en-us_topic_0175230634_section6221139012631"></a>

None


# Development Guide<a name="EN-US_TOPIC_0302389228"></a>

## Functions<a name="section4529911120325"></a>

<a name="en-us_topic_0175230498_table5999158320325"></a>
<table><thead align="left"><tr id="en-us_topic_0175230498_row2323990220325"><th class="cellrowborder" valign="top" width="24.76247624762476%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175230498_p1748969020325"><a name="en-us_topic_0175230498_p1748969020325"></a><a name="en-us_topic_0175230498_p1748969020325"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="23.002300230023003%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175230498_p5025139720325"><a name="en-us_topic_0175230498_p5025139720325"></a><a name="en-us_topic_0175230498_p5025139720325"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="52.235223522352236%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175230498_p2204176620325"><a name="en-us_topic_0175230498_p2204176620325"></a><a name="en-us_topic_0175230498_p2204176620325"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175230498_row1839119416414"><td class="cellrowborder" valign="top" width="24.76247624762476%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175230498_p5391141124113"><a name="en-us_topic_0175230498_p5391141124113"></a><a name="en-us_topic_0175230498_p5391141124113"></a>Initialize C++ constructors</p>
</td>
<td class="cellrowborder" valign="top" width="23.002300230023003%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175230498_p11391441164119"><a name="en-us_topic_0175230498_p11391441164119"></a><a name="en-us_topic_0175230498_p11391441164119"></a>LOS_CppSystemInit</p>
</td>
<td class="cellrowborder" valign="top" width="52.235223522352236%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175230498_p17391204116414"><a name="en-us_topic_0175230498_p17391204116414"></a><a name="en-us_topic_0175230498_p17391204116414"></a>Initializes C++ constructors.</p>
</td>
</tr>
</tbody>
</table>

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>This function expects three parameters:
>-   The first parameter specifies the start address of the  **init\_array**  segment.
>-   The first parameter specifies the end address of the  **init\_array**  segment.
>-   The third parameter identifies the scenario where C++ is called, including  **BEFORE\_SCATTER**  \(calling C++ at fast startup of scatter loading\),  **AFTER\_SCATTER**  \(calling C++ at non-fast startup of scatter loading\), and  **NO\_SCATTER**  \(calling C++ in non-scatter loading or not calling C++ in scatter loading.\)

## Development Process<a name="section41972154195820"></a>

1.  Run the  **make menuconfig**  command to enable C++.

    <a name="table06655375130"></a>
    <table><thead align="left"><tr id="row8665203751318"><th class="cellrowborder" valign="top" width="24.36%" id="mcps1.1.6.1.1"><p id="p61687296155221"><a name="p61687296155221"></a><a name="p61687296155221"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="32.09%" id="mcps1.1.6.1.2"><p id="p25007692155221"><a name="p25007692155221"></a><a name="p25007692155221"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="10.27%" id="mcps1.1.6.1.3"><p id="p47081329204415"><a name="p47081329204415"></a><a name="p47081329204415"></a>Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="8.64%" id="mcps1.1.6.1.4"><p id="p7383544155221"><a name="p7383544155221"></a><a name="p7383544155221"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="24.64%" id="mcps1.1.6.1.5"><p id="p34917797155221"><a name="p34917797155221"></a><a name="p34917797155221"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="row9665837161320"><td class="cellrowborder" valign="top" width="24.36%" headers="mcps1.1.6.1.1 "><p id="p3665143710130"><a name="p3665143710130"></a><a name="p3665143710130"></a>LOSCFG_KERNEL_CPPSUPPORT</p>
    </td>
    <td class="cellrowborder" valign="top" width="32.09%" headers="mcps1.1.6.1.2 "><p id="p13665737191318"><a name="p13665737191318"></a><a name="p13665737191318"></a>Whether to tailor the C++ feature.</p>
    </td>
    <td class="cellrowborder" valign="top" width="10.27%" headers="mcps1.1.6.1.3 "><p id="p19665193721319"><a name="p19665193721319"></a><a name="p19665193721319"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="8.64%" headers="mcps1.1.6.1.4 "><p id="p1866573713131"><a name="p1866573713131"></a><a name="p1866573713131"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="24.64%" headers="mcps1.1.6.1.5 "><p id="p1266519378138"><a name="p1266519378138"></a><a name="p1266519378138"></a>LOSCFG_KERNEL_EXTKERNEL</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  Before using the C++ feature, call the LOS\_CppSystemInit function to initialize the C++ constructor.
3.  Call both C functions and C++ functions.

    To call C functions in C++, add the macros contained in C++ to the code as follows:

    ```
    #ifdef __cplusplus
    #if __cplusplus
    extern "C" {
    #endif /* __cplusplus */
    #endif /* __cplusplus */
    /* code */
    ...
    #ifdef __cplusplus
    #if __cplusplus
    }
    #endif /* __cplusplus */
    #endif /* __cplusplus */
    ```



# Development Guidelines<a name="EN-US_TOPIC_0311018431"></a>

## Usage Scenarios<a name="en-us_topic_0175921104_section10746131142833"></a>

A spinlock provides a mutual exclusion mechanism to prevent two tasks from accessing the same shared resource at the same time.

## Functions<a name="en-us_topic_0175921104_section335816715456"></a>

The spinlock module of Huawei LiteOS provides the following functions. For details about the APIs, see the API reference.

<a name="en-us_topic_0175921104_table14234001162410"></a>
<table><thead align="left"><tr id="en-us_topic_0175921104_row20709259162410"><th class="cellrowborder" valign="top" width="30.05300530053005%" id="mcps1.1.4.1.1"><p id="en-us_topic_0175921104_p66837260162410"><a name="en-us_topic_0175921104_p66837260162410"></a><a name="en-us_topic_0175921104_p66837260162410"></a>Function Category</p>
</th>
<th class="cellrowborder" valign="top" width="23.77237723772377%" id="mcps1.1.4.1.2"><p id="en-us_topic_0175921104_p45109011162410"><a name="en-us_topic_0175921104_p45109011162410"></a><a name="en-us_topic_0175921104_p45109011162410"></a>API</p>
</th>
<th class="cellrowborder" valign="top" width="46.17461746174617%" id="mcps1.1.4.1.3"><p id="en-us_topic_0175921104_p29951237162410"><a name="en-us_topic_0175921104_p29951237162410"></a><a name="en-us_topic_0175921104_p29951237162410"></a>Description</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0175921104_row10131148162410"><td class="cellrowborder" rowspan="2" valign="top" width="30.05300530053005%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p15316688162410"><a name="en-us_topic_0175921104_p15316688162410"></a><a name="en-us_topic_0175921104_p15316688162410"></a>Spinlock initialization</p>
</td>
<td class="cellrowborder" valign="top" width="23.77237723772377%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p32692216162410"><a name="en-us_topic_0175921104_p32692216162410"></a><a name="en-us_topic_0175921104_p32692216162410"></a>LOS_SpinInit</p>
</td>
<td class="cellrowborder" valign="top" width="46.17461746174617%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175921104_p30823853162410"><a name="en-us_topic_0175921104_p30823853162410"></a><a name="en-us_topic_0175921104_p30823853162410"></a>Dynamically initializes a spinlock.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row185813332337"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p9582173353317"><a name="en-us_topic_0175921104_p9582173353317"></a><a name="en-us_topic_0175921104_p9582173353317"></a>SPIN_LOCK_INIT</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p15582103317339"><a name="en-us_topic_0175921104_p15582103317339"></a><a name="en-us_topic_0175921104_p15582103317339"></a>Statically initializes a spinlock.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row54162342162410"><td class="cellrowborder" rowspan="3" valign="top" width="30.05300530053005%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p25073564162410"><a name="en-us_topic_0175921104_p25073564162410"></a><a name="en-us_topic_0175921104_p25073564162410"></a>Spinlock request and release</p>
</td>
<td class="cellrowborder" valign="top" width="23.77237723772377%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p17692840162410"><a name="en-us_topic_0175921104_p17692840162410"></a><a name="en-us_topic_0175921104_p17692840162410"></a>LOS_SpinLock</p>
</td>
<td class="cellrowborder" valign="top" width="46.17461746174617%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175921104_p23833903162410"><a name="en-us_topic_0175921104_p23833903162410"></a><a name="en-us_topic_0175921104_p23833903162410"></a>Requests a specified spinlock. If the spinlock cannot be obtained, the system waits in a loop.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row6130742123610"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p41301442183610"><a name="en-us_topic_0175921104_p41301442183610"></a><a name="en-us_topic_0175921104_p41301442183610"></a>LOS_SpinTrylock</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p16130442103610"><a name="en-us_topic_0175921104_p16130442103610"></a><a name="en-us_topic_0175921104_p16130442103610"></a>Attempts to request a specified spinlock. If the spinlock cannot be obtained, a failure message is returned, and the system does not wait in a loop.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row13178541162410"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p60828923162410"><a name="en-us_topic_0175921104_p60828923162410"></a><a name="en-us_topic_0175921104_p60828923162410"></a>LOS_SpinUnlock</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p28195705162410"><a name="en-us_topic_0175921104_p28195705162410"></a><a name="en-us_topic_0175921104_p28195705162410"></a>Releases a spinlock.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row829833285118"><td class="cellrowborder" rowspan="2" valign="top" width="30.05300530053005%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p1826134717516"><a name="en-us_topic_0175921104_p1826134717516"></a><a name="en-us_topic_0175921104_p1826134717516"></a>Spinlock request and release (with disabled interrupt)</p>
</td>
<td class="cellrowborder" valign="top" width="23.77237723772377%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p6298132165119"><a name="en-us_topic_0175921104_p6298132165119"></a><a name="en-us_topic_0175921104_p6298132165119"></a>LOS_SpinLockSave</p>
</td>
<td class="cellrowborder" valign="top" width="46.17461746174617%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175921104_p329873275110"><a name="en-us_topic_0175921104_p329873275110"></a><a name="en-us_topic_0175921104_p329873275110"></a>Requests a spinlock after the interrupt is disabled.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row2474173655120"><td class="cellrowborder" valign="top" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p3474123617518"><a name="en-us_topic_0175921104_p3474123617518"></a><a name="en-us_topic_0175921104_p3474123617518"></a>LOS_SpinUnlockRestore</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p1347433685113"><a name="en-us_topic_0175921104_p1347433685113"></a><a name="en-us_topic_0175921104_p1347433685113"></a>Releases a spinlock and restores the interrupt.</p>
</td>
</tr>
<tr id="en-us_topic_0175921104_row13315131215318"><td class="cellrowborder" valign="top" width="30.05300530053005%" headers="mcps1.1.4.1.1 "><p id="en-us_topic_0175921104_p0315812115318"><a name="en-us_topic_0175921104_p0315812115318"></a><a name="en-us_topic_0175921104_p0315812115318"></a>Spinlock holding status</p>
</td>
<td class="cellrowborder" valign="top" width="23.77237723772377%" headers="mcps1.1.4.1.2 "><p id="en-us_topic_0175921104_p133153129534"><a name="en-us_topic_0175921104_p133153129534"></a><a name="en-us_topic_0175921104_p133153129534"></a>LOS_SpinHeld</p>
</td>
<td class="cellrowborder" valign="top" width="46.17461746174617%" headers="mcps1.1.4.1.3 "><p id="en-us_topic_0175921104_p123154125539"><a name="en-us_topic_0175921104_p123154125539"></a><a name="en-us_topic_0175921104_p123154125539"></a>Checks whether a spinlock is held.</p>
</td>
</tr>
</tbody>
</table>

## Development Process<a name="en-us_topic_0175921104_section1137085411376"></a>

The typical spinlock development process is as follows:

1.  Spinlocks depend on the SMP. Run the  **make menuconfig**  command and choose  **Kernel**  \>  **Enable Kernel SMP**  to configure the spinlock.

    <a name="en-us_topic_0175921104_table34659900162615"></a>
    <table><thead align="left"><tr id="en-us_topic_0175921104_row3414390162615"><th class="cellrowborder" valign="top" width="21.3978602139786%" id="mcps1.1.6.1.1"><p id="en-us_topic_0175921104_p8130151162615"><a name="en-us_topic_0175921104_p8130151162615"></a><a name="en-us_topic_0175921104_p8130151162615"></a>Configuration Item</p>
    </th>
    <th class="cellrowborder" valign="top" width="35.35646435356465%" id="mcps1.1.6.1.2"><p id="en-us_topic_0175921104_p54562526162615"><a name="en-us_topic_0175921104_p54562526162615"></a><a name="en-us_topic_0175921104_p54562526162615"></a>Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="15.608439156084392%" id="mcps1.1.6.1.3"><p id="en-us_topic_0175921104_p57488448162615"><a name="en-us_topic_0175921104_p57488448162615"></a><a name="en-us_topic_0175921104_p57488448162615"></a>Value Range</p>
    </th>
    <th class="cellrowborder" valign="top" width="12.768723127687231%" id="mcps1.1.6.1.4"><p id="en-us_topic_0175921104_p26052727162615"><a name="en-us_topic_0175921104_p26052727162615"></a><a name="en-us_topic_0175921104_p26052727162615"></a>Default Value</p>
    </th>
    <th class="cellrowborder" valign="top" width="14.86851314868513%" id="mcps1.1.6.1.5"><p id="en-us_topic_0175921104_p29896171162615"><a name="en-us_topic_0175921104_p29896171162615"></a><a name="en-us_topic_0175921104_p29896171162615"></a>Dependency</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0175921104_row5670787162615"><td class="cellrowborder" valign="top" width="21.3978602139786%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175921104_p767075542311"><a name="en-us_topic_0175921104_p767075542311"></a><a name="en-us_topic_0175921104_p767075542311"></a>LOSCFG_KERNEL_SMP</p>
    </td>
    <td class="cellrowborder" valign="top" width="35.35646435356465%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175921104_p27727169162615"><a name="en-us_topic_0175921104_p27727169162615"></a><a name="en-us_topic_0175921104_p27727169162615"></a>Whether to enable the SMP.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.608439156084392%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175921104_p31308215162615"><a name="en-us_topic_0175921104_p31308215162615"></a><a name="en-us_topic_0175921104_p31308215162615"></a>YES/NO</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.768723127687231%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175921104_p52937457162615"><a name="en-us_topic_0175921104_p52937457162615"></a><a name="en-us_topic_0175921104_p52937457162615"></a>YES</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.86851314868513%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175921104_p60075607162615"><a name="en-us_topic_0175921104_p60075607162615"></a><a name="en-us_topic_0175921104_p60075607162615"></a>Multi-core hardware is supported.</p>
    </td>
    </tr>
    <tr id="en-us_topic_0175921104_row3809559162615"><td class="cellrowborder" valign="top" width="21.3978602139786%" headers="mcps1.1.6.1.1 "><p id="en-us_topic_0175921104_p131131669249"><a name="en-us_topic_0175921104_p131131669249"></a><a name="en-us_topic_0175921104_p131131669249"></a>LOSCFG_KERNEL_SMP_CORE_NUM</p>
    </td>
    <td class="cellrowborder" valign="top" width="35.35646435356465%" headers="mcps1.1.6.1.2 "><p id="en-us_topic_0175921104_p30022176162615"><a name="en-us_topic_0175921104_p30022176162615"></a><a name="en-us_topic_0175921104_p30022176162615"></a>Number of cores in the multi-core scenario.</p>
    </td>
    <td class="cellrowborder" valign="top" width="15.608439156084392%" headers="mcps1.1.6.1.3 "><p id="en-us_topic_0175921104_p15877214162615"><a name="en-us_topic_0175921104_p15877214162615"></a><a name="en-us_topic_0175921104_p15877214162615"></a>Depends on the architecture.</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.768723127687231%" headers="mcps1.1.6.1.4 "><p id="en-us_topic_0175921104_p10985932162615"><a name="en-us_topic_0175921104_p10985932162615"></a><a name="en-us_topic_0175921104_p10985932162615"></a>2</p>
    </td>
    <td class="cellrowborder" valign="top" width="14.86851314868513%" headers="mcps1.1.6.1.5 "><p id="en-us_topic_0175921104_p17445266162615"><a name="en-us_topic_0175921104_p17445266162615"></a><a name="en-us_topic_0175921104_p17445266162615"></a>None</p>
    </td>
    </tr>
    </tbody>
    </table>

2.  To create a spinlock, call the LOS\_SpinInit API to initialize the spinlock, or call SPIN\_LOCK\_INIT API to initialize the spinlock of the static memory.
3.  Call the LOS\_SpinLock, LOS\_SpinTrylock, or LOS\_SpinLockSave API to request a spinlock. If the request is successful, the code for lock protection is executed. If the request fails, the system enters a busy loop till a spinlock is obtained.
4.  Call the LOS\_SpinUnlock/LOS\_SpinUnlockRestore API to release a spinlock. After the lock protection code is executed, the corresponding spinlock is released, so that other cores can request the spinlock.


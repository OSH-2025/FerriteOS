# Scheduling Statistics<a name="EN-US_TOPIC_0312244433"></a>

## Function Description<a name="en-us_topic_0291635259_section15719185416470"></a>

This function is used to collect CPU scheduling information, including the startup time, running duration, and number of scheduling times of idle tasks.

## How to Use<a name="en-us_topic_0291635259_section2071256204815"></a>

1.  Run  **make menuconfig**  and choose  **Debug**  \>  **Enable a Debug Version**  \>  **Enable Debug LiteOS Kernel Resource**  \>  **Enable Scheduler Statistics Debugging**  to enable the scheduling statistics function, that is, configure  **LOSCFG\_DEBUG\_SCHED\_STATISTICS=y**. This function is disabled by default.
2.  Register the following functions as shell commands: For details about how to register the shell commands, see  [Development Process of Adding a Shell Command](/shell/doc_en/development-guide.md#development-process-of-adding-a-shell-command).

    OsShellStatisticsStart is the function for enabling the scheduling statistics.

    OsShellStatisticsStop is the function for disabling the scheduling statistics. After the scheduling statistics is disabled, OsStatisticsShow is automatically called to output the scheduling statistics.

    OsShellCmdDumpSched is the function for displaying the CPU scheduling information.

3.  View the scheduling information.

    Call the registered command in the shell window.


## Precautions<a name="en-us_topic_0291635259_section7672188125113"></a>

Run the shell command corresponding to OsShellStatisticsStart to enable the scheduling statistics, and then view the scheduling information.

## Output<a name="en-us_topic_0291635259_section85734144813"></a>

**Figure  1**  Output for calling OsShellStatisticsStop<a name="en-us_topic_0291635259_fig104573413421"></a>  
![](figures/sched_statistics_after_stop.png "output-for-calling-osshellstatisticsstop")

In the preceding figure,  **mpstop**  is the shell command registered by OsShellStatisticsStop and is only an example. Actually, this command does not exist in the system. The following table describes the output items in the preceding figure.

<a name="en-us_topic_0291635259_table1978162419917"></a>
<table><tbody><tr id="en-us_topic_0291635259_row68145245917"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p128141624198"><a name="en-us_topic_0291635259_p128141624198"></a><a name="en-us_topic_0291635259_p128141624198"></a>Passed Time</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p19814924598"><a name="en-us_topic_0291635259_p19814924598"></a><a name="en-us_topic_0291635259_p19814924598"></a>Running duration of the scheduling function.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row16814122416918"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p581410241796"><a name="en-us_topic_0291635259_p581410241796"></a><a name="en-us_topic_0291635259_p581410241796"></a>CPU</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p17814924891"><a name="en-us_topic_0291635259_p17814924891"></a><a name="en-us_topic_0291635259_p17814924891"></a>CPU name.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row10814132418916"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p7814132410916"><a name="en-us_topic_0291635259_p7814132410916"></a><a name="en-us_topic_0291635259_p7814132410916"></a>Idle(%)</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p158141424397"><a name="en-us_topic_0291635259_p158141424397"></a><a name="en-us_topic_0291635259_p158141424397"></a>Percentage of idle task running duration.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row381415246912"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p681462413919"><a name="en-us_topic_0291635259_p681462413919"></a><a name="en-us_topic_0291635259_p681462413919"></a>ContexSwitch</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p1481422412910"><a name="en-us_topic_0291635259_p1481422412910"></a><a name="en-us_topic_0291635259_p1481422412910"></a>Number of task scheduling switching times.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row198141424191"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p15814324296"><a name="en-us_topic_0291635259_p15814324296"></a><a name="en-us_topic_0291635259_p15814324296"></a>HwiNum</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p1181416246918"><a name="en-us_topic_0291635259_p1181416246918"></a><a name="en-us_topic_0291635259_p1181416246918"></a>Number of times that interrupts are triggered.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row1681422417916"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p1814182414916"><a name="en-us_topic_0291635259_p1814182414916"></a><a name="en-us_topic_0291635259_p1814182414916"></a>Avg Pri</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p6814124395"><a name="en-us_topic_0291635259_p6814124395"></a><a name="en-us_topic_0291635259_p6814124395"></a>Average priority of switch-in tasks that are not idle tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row2814324495"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p118141824391"><a name="en-us_topic_0291635259_p118141824391"></a><a name="en-us_topic_0291635259_p118141824391"></a>HiTask(%)</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p1481513241890"><a name="en-us_topic_0291635259_p1481513241890"></a><a name="en-us_topic_0291635259_p1481513241890"></a>Percentage of the running duration of high-priority tasks. A task whose priority is less than 16 is defined as a high-priority task.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row1881518241593"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p2081513241599"><a name="en-us_topic_0291635259_p2081513241599"></a><a name="en-us_topic_0291635259_p2081513241599"></a>HiTask SwiNum</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p281522418911"><a name="en-us_topic_0291635259_p281522418911"></a><a name="en-us_topic_0291635259_p281522418911"></a>Number of switching times that a high-priority task is switched in. A task with a priority lower than 16 is high-priority.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row681517241394"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p881582416919"><a name="en-us_topic_0291635259_p881582416919"></a><a name="en-us_topic_0291635259_p881582416919"></a>HiTask P(ms)</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p158155241892"><a name="en-us_topic_0291635259_p158155241892"></a><a name="en-us_topic_0291635259_p158155241892"></a>Average running duration of high-priority tasks. A task whose priority is less than 16 is high-priority.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row178156241097"><td class="cellrowborder" valign="top" width="27.57%"><p id="en-us_topic_0291635259_p58151924297"><a name="en-us_topic_0291635259_p58151924297"></a><a name="en-us_topic_0291635259_p58151924297"></a>MP Hwi</p>
</td>
<td class="cellrowborder" valign="top" width="72.43%"><p id="en-us_topic_0291635259_p13815124995"><a name="en-us_topic_0291635259_p13815124995"></a><a name="en-us_topic_0291635259_p13815124995"></a>Number of times that inter-core interrupts are triggered. This item is used only for multi-core CPUs.</p>
</td>
</tr>
</tbody>
</table>

**Figure  2**  Output for calling OsShellCmdDumpSched<a name="en-us_topic_0291635259_fig11142245154213"></a>  
![](figures/CPU_sched_information.png "output-for-calling-osshellcmddumpsched")

In the preceding figure,  **mpstat**  is the shell command registered by OsShellCmdDumpSched and is only an example. Actually, this command does not exist in the system. The following table describes the output items in the preceding figure.

<a name="en-us_topic_0291635259_table959215014287"></a>
<table><tbody><tr id="en-us_topic_0291635259_row12613125072817"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p961375018282"><a name="en-us_topic_0291635259_p961375018282"></a><a name="en-us_topic_0291635259_p961375018282"></a>Task</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p1761365002814"><a name="en-us_topic_0291635259_p1761365002814"></a><a name="en-us_topic_0291635259_p1761365002814"></a>Task name.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row461395082818"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p561315002816"><a name="en-us_topic_0291635259_p561315002816"></a><a name="en-us_topic_0291635259_p561315002816"></a>TID</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p1961335016281"><a name="en-us_topic_0291635259_p1961335016281"></a><a name="en-us_topic_0291635259_p1961335016281"></a>Task ID.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row4613195022813"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p1661314503286"><a name="en-us_topic_0291635259_p1661314503286"></a><a name="en-us_topic_0291635259_p1661314503286"></a>Total Time</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p1561320506285"><a name="en-us_topic_0291635259_p1561320506285"></a><a name="en-us_topic_0291635259_p1561320506285"></a>Total running duration of all CPU tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row16131850132813"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p126131850162817"><a name="en-us_topic_0291635259_p126131850162817"></a><a name="en-us_topic_0291635259_p126131850162817"></a>Total CST</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p96131450112818"><a name="en-us_topic_0291635259_p96131450112818"></a><a name="en-us_topic_0291635259_p96131450112818"></a>Number of context switching times of all CPU tasks.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row136131450122819"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p1613185072816"><a name="en-us_topic_0291635259_p1613185072816"></a><a name="en-us_topic_0291635259_p1613185072816"></a>CPU</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p761385072818"><a name="en-us_topic_0291635259_p761385072818"></a><a name="en-us_topic_0291635259_p761385072818"></a>CPU name.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row1461375014284"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p19613185016284"><a name="en-us_topic_0291635259_p19613185016284"></a><a name="en-us_topic_0291635259_p19613185016284"></a>Time</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p10613450182820"><a name="en-us_topic_0291635259_p10613450182820"></a><a name="en-us_topic_0291635259_p10613450182820"></a>Running duration of a CPU task.</p>
</td>
</tr>
<tr id="en-us_topic_0291635259_row061316508285"><td class="cellrowborder" valign="top" width="28.09%"><p id="en-us_topic_0291635259_p1661385020280"><a name="en-us_topic_0291635259_p1661385020280"></a><a name="en-us_topic_0291635259_p1661385020280"></a>CST</p>
</td>
<td class="cellrowborder" valign="top" width="71.91%"><p id="en-us_topic_0291635259_p861316509286"><a name="en-us_topic_0291635259_p861316509286"></a><a name="en-us_topic_0291635259_p861316509286"></a>Number of context switching times of a CPU task.</p>
</td>
</tr>
</tbody>
</table>


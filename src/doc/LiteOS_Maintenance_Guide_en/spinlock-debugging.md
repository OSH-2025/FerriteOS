# Spinlock Debugging<a name="EN-US_TOPIC_0312244444"></a>

## Function Description<a name="en-us_topic_0291373015_section177518092"></a>

In a multi-core environment, the multi-task system uses spinlocks to achieve resource access exclusion. The spinlock debugging module \(**lockdep**\) can detect the following types of errors \(including usage errors\):

1.  The task is repeatedly locked.
2.  A deadlock occurs. The following uses ABBA as an example:

    1.  Task A owns spinlock X and permanently waits for spinlock Y.
    2.  Task B owns spinlock Y and permanently waits for spinlock X.

    In this case, task A and task B are deadlocked.

3.  The lock is released when it is not owned by any tasks.
4.  The  **lockdep**  record information overflows.

## How to Use<a name="en-us_topic_0291373015_section933612101390"></a>

The  **LOSCFG\_KERNEL\_SMP\_LOCKDEP**  marco is used to enable the spin lock detection module  **lockdep**  to enable the spin lock commissioning function. You can run the  **make menuconfig**  command to enable  **Enable Spinlock Lockdep Check**.

```
Kernel ---> Enable Kernel SMP ---> Enable Spinlock Lockdep Check
```

## Example for Locating Deadlocks<a name="en-us_topic_0291373015_section17225714192818"></a>

1.  After spinlock debugging function is enabled, deadlock information is printed when a deadlock is detected. The following is an example of the information printed during deadlock check:

    **Figure  1**  Information printed by the deadlock module<a name="en-us_topic_0291373015_f6bb63b55c1874aebb9e4a4a0cdc1f9f7"></a>  
    ![](figures/deadlock_information_in_lockdep.png)

2.  Copy the value of  **request addr**  \(**0x802a6528**  in this example\) and find the corresponding address in the .asm disassembly file of the system image. By default, the file is generated in the  **Huawei\_LiteOS/out/**_Platform name_  directory. As shown in the following figure, the location where spinlock is called and the called function \(task\_fx02 in this example\) can be located.

    **Figure  2**  Finding corresponding addresses in the disassembly file<a name="en-us_topic_0291373015_fdbc53a06347c4cfb9ba13da71c3bbe31"></a>  
    ![](figures/find_addr_for_spinlock_deadlock.png)

3.  Based on the spinlock in the second blue box of the deadlock information shown in Figure 1, find the spinlock owned by another task based on the code logic, and then adjust the spinlock calling time sequence based on the code to fix the deadlock.

## Suggestion and Summary<a name="en-us_topic_0291373015_scd8cd435b7724e46b5301f98880d0c84"></a>

The spinlock deadlock occurs when a CPU is suspended and tasks can no longer be scheduled. If the spinlock debugging is not enabled, you can use a debugging tool such as JLink to halt the CPU or trigger a watchdog exception. Then, check whether the PC value is the spinlock code to determine whether the spinlock occurs.


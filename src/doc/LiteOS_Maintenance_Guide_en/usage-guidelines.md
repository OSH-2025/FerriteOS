# Usage Guidelines<a name="EN-US_TOPIC_0312244431"></a>

## Usage Scenarios<a name="en-us_topic_0287763514_section4198546414140"></a>

-   The system memory is suspected to be faulty and the fault needs to be located.
-   During development, you need to check whether memory problems exist.

## Usage Process<a name="en-us_topic_0287763514_section15585181011559"></a>

1.  Add the LMS compilation check option -fsanitize=kernel-address to the  **Makefile**  file of the detected module.
2.  Use  **menuconfig**  to configure the compiler not to be optimized.

    LOSCFG\_COMPILER\_OPTIMIZE\_NONE=y \[Compiler \> Optimize Option \> Optimize None\]

3.  The LMS supports only the bestfit memory algorithms and does not support the SLAB algorithms. Therefore, you need to enable the following configuration items:

    LOSCFG\_KERNEL\_MEM\_BESTFIT=y \[Kernel \> Memory Management \> Dynamic Memory Management Algorithm \> bestfit\] 

    LOSCFG\_KERNEL\_MEM\_SLAB\_EXTENTION is not set \[Kernel \> Memory Management \> Enable Mem SLAB Extension\]

4.  To output the Backtrace information, enable the following configuration item:

    LOSCFG\_BACKTRACE=y \[Debug \> Enable Backtrace\]

5.  Enable the LMS configuration item.

    LOSCFG\_KERNEL\_LMS=y \[Debug \> Enable Lite Memory Sanitizer\]

6.  Recompile the code and check the serial port output. If a memory problem is detected, the detection result is displayed. The detection information displayed by the LMS is similar to that shown in the following figure.

    ![](figures/LMS_detect_information.png)

    LMS detection information includes the following types of information:

    -   Detected memory problem, for example, Use-After-Free in the preceding figure.
    -   Address of the faulty memory, for example, 0x804308cb in the preceding figure.
    -   Shadow memory corresponding to the address of the faulty memory and the value of the shadow memory. In the preceding example, the address of the shadow memory is 0x83c7b74c, and the value of the address is 01.
    -   Memory values of the faulty address before and after the problem occurs.
    -   Running task. In the preceding example, the task name is IT\_TST\_INI and the task ID is 4.
    -   Backtrace stack when the problem occurs.


## Platform Differences<a name="en-us_topic_0287763514_section15592521317"></a>

None


# Usage of Magic Keys<a name="EN-US_TOPIC_0312244446"></a>

## Usage Scenarios<a name="en-us_topic_0175230489_section10138163141413"></a>

When the system does not display crash information and does not respond, use magic keys to check whether the system responds to an interrupt. If the system has no response, check the CPU usage \(cpup\) in the task information and identify the task that occupies the CPU for a long period to cause the system crash. If high-priority tasks are using the CPU, low-priority tasks will not respond.

## Function Description<a name="en-us_topic_0175230489_section13598163943816"></a>

Magic keys can be used to identify special keys and output relevant information during the UART and USB-to-virtual serial port interrupts.

## How to Use<a name="en-us_topic_0175230489_section104865185441"></a>

1.  Run the  **make menuconfig**  command to enable the magic key function.
    -   This function depends on shell. Therefore, you need to enable shell first.

        ```
        Debug ---> Enable a Debug Version ---> Enable Shell
        ```

    -   To enable this function, set  **LOSCFG\_ENABLE\_MAGICKEY**  to  **y**. Otherwise, the magic key becomes invalid. To enable the marco, choose the following menu item:

        ```
        Debug ---> Enable MAGIC KEY
        ```

        >![](public_sys-resources/icon-note.gif) **NOTE:** 
        >
        >You can move the cursor to this function in  **menuconfig**, and enter  **?**  to view help information.
        >```
        >Answer Y to enable LiteOS Magic key.
        >  ctrl + r : Magic key check switch;
        >  ctrl + z : Show all magic op key;
        >  ctrl + t : Show task information;
        >  ctrl + p : System panic;
        >  ctrl + e : Check system memory pool.
        >```


2.  Press the combined key  **Ctrl**+**r**  to enable the detection function of a magic key.

    When the UART or USB-to-virtual serial port is connected, press the combined key  **Ctrl**+**r**. The detection function of a magic key is enabled and output "Magic key on". If you press the combined key once again, the detection function is disabled and output "Magic key off". Magic keys can achieve the following functions:

    -   **Ctrl**+**z**: Help key, which is used to display the brief introduction to related magic keys.
    -   **Ctrl**+**t**: displays task information.
    -   **Ctrl**+**p**: The system proactively enters the panic state. After the panic-related information is printed, the system is suspended.
    -   **Ctrl**+**e**: The system checks the integrity of the memory pool. If an error is detected, the system displays an error message. If no error is detected, the system displays "system memcheck over, all passed!".


## Precautions<a name="en-us_topic_0175230489_section7672188125113"></a>

When the detection function of a magic key is enabled, you need to ensure that special characters entered through UART or USB-to-virtual serial ports differ from the magic key values; otherwise, a magic key is incorrectly triggered, and existing design functions may have exceptions.


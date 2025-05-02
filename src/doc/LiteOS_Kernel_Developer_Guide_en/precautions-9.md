# Precautions<a name="EN-US_TOPIC_0145350142"></a>

-   Configure the maximum number of supported interrupts and the number of configurable interrupt priorities based on the specific hardware.
-   The interrupt sharing mechanism allows different devices to use the same interrupt number to register the same interrupt handler. However, the input parameter  **pDevId**  \(device ID\) of the interrupt handler must be unique to represent different devices. That is, interrupt handlers can be mounted only once with the same interrupt numbers and device IDs. However, the interrupt handlers can be mounted repeatedly with the same interrupt numbers and different device IDs.
-   The interrupt handler cannot take a long time. Otherwise, the CPU cannot respond to interrupts in time.
-   The function that causes scheduling cannot be executed during the response to interrupts.
-   The input parameter of the LOS\_IntRestore\(\) API must be the return value of the LOS\_IntLock\(\) API, that is, the CPSR value before the interrupt is disabled.
-   Interrupts 0–15 in the Cortex-M series processors and interrupts 0-31 in the Cortex-A7 are for internal use. Therefore, you are not advised to apply for and create them.


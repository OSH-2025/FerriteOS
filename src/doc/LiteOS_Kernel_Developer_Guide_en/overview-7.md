# Overview<a name="EN-US_TOPIC_0311018395"></a>

## Basic Concepts<a name="en-us_topic_0175230273_section6092055115413"></a>

An interrupt refers to the process in which the CPU stops to execute a new program when necessary. That is, during the running of the program, a transaction that must be immediately processed by the CPU occurs. In this case, the CPU temporarily stops the execution of the ongoing program and processes the transaction. This process is called interruption.

Peripherals can complete certain work without the CPU. In some cases, however, the CPU needs to perform certain work for the peripherals. With the interrupt mechanism, the CPU can execute other tasks when the peripherals do not require the CPU. When the peripherals require the CPU, the CPU can interrupt the ongoing task to respond to the interrupt request by generating the interrupt signal. In this way, the CPU does not need to spend a lot of time in waiting and querying the peripheral status, which greatly improves the real-time performance and execution efficiency of the system.

The interrupt mechanism of Huawei LiteOS supports:

-   Interrupt sharing, which can be configured.
-   Interrupt nesting, that is, a high-priority interrupt can preempt a low-priority interrupt, which can be configured.
-   Independent interrupt stack, which can be configured.
-   Number of supported interrupt priorities, which can be configured.
-   Number of supported interrupts, which can be configured.

## Interrupt-related Hardware<a name="en-us_topic_0175230273_section835914496338"></a>

Interrupt-related hardware can be classified into three types: device, interrupt controller, and CPU.

-   Device

    The device is the source that initiates an interrupt. When the device needs to request the CPU, an interrupt signal is generated and connected to the interrupt controller.

-   Interrupt controller

    The interrupt controller is one of the peripherals of the CPU. On the one hand, the interrupt controller receives the input of the interrupt pins of other peripherals. On the other hand, the interrupt controller sends interrupt signals to the CPU. The interrupt controller can be programmed to set the startup/stop status, priority, and trigger mode of the interrupt source. Common interrupt controllers include vector interrupt controllers \(VICs\) and general interrupt controllers \(GICs\). The nested vector interrupt controller \(NVIC\) is used in the ARM Cortex-M series. The GIC is used in ARM Cortex-A7.

-   CPU

    The CPU responds to the request of the interrupt source, interrupts the ongoing task, and executes the interrupt handler.


## Interrupt-related Concepts<a name="en-us_topic_0175230273_section53711673346"></a>

**Interrupt number**

Interrupt request signals are numbered so the computer can determine interrupt requests sent by devices.

**Interrupt request**

An emergency event needs to apply to the CPU \(by sending an electrical pulse signal\) for interruption, and requires the CPU to suspend the ongoing task and process the emergency event. This application process is called interrupt request.

**Interrupt priority**

To enable the system to respond to and handle all interrupts in time, the system classifies interrupt sources into several levels based on the importance and urgency of interrupt events.

**Interrupt handler**

After a peripheral generates an interrupt request, the CPU suspends the ongoing task and responds to the interrupt request. Each device that generates an interrupt has a corresponding interrupt handler.

**Interrupt nesting**

Interrupt nesting is also called interrupt preemption. When an interrupt handler is being executed, if another interrupt source with a higher priority raises an interrupt request, the interrupt handler of the interrupt source with a lower priority that is being executed is temporarily stopped, and the interrupt request with a higher priority is processed, after the processing is complete, the stopped interrupt handler continues to be executed.

**Interrupt triggering**

The interrupt source sends an interrupt signal to the interrupt controller. The interrupt controller arbitrates the interrupt, determines the priority, and sends the interrupt signal to the CPU. When the interrupt source generates an interrupt signal, the interrupt trigger is set to  **1**, indicating that the interrupt source generates an interrupt and requires the CPU to respond to the interrupt.

**Interrupt trigger type**

An external interrupt request is sent to the NVIC or GIC through a physical signal. The interrupt can be level-triggered or edge-triggered.

**Interrupt vector**

Entry address of the interrupt service routine \(ISR\).

**Interrupt vector table**

Region for storing interrupt vectors. Interrupt vectors map to interrupt numbers. The interrupt vectors are stored in the interrupt vector table in sequence based on the interrupt numbers.

**Interrupt sharing**

When the number of peripherals is small, one peripheral corresponds to one interrupt number. To support more hardware devices, multiple devices can share one interrupt number. The interrupt handlers that share the same interrupt number form a linked list. When an external device generates an interrupt request, the system traverses the linked list until the interrupt handler of the device is found. During the traversal, each interrupt handler may determine, by detecting the device ID, whether an interrupt is generated by the corresponding device.

**Inter-core interrupt**

For a multi-core system, the interrupt controller allows a hardware thread of a CPU to interrupt the hardware threads of other CPUs. This mode is called inter-core interrupt. Inter-core interrupts are implemented based on memory sharing among multiple CPUs. Inter-core interrupts can reduce the load of a CPU and improve system efficiency. Currently, only the GIC supports this function.

## Operation Mechanism<a name="en-us_topic_0175230273_section66198724155629"></a>

-   The interrupt mechanism of Huawei LiteOS supports interrupt sharing.

    The implementation of interrupt sharing depends on the linked list. A linked list is created for each interrupt number. The linked list node contains the registered interrupt handler function and input parameters of the function. When interrupts are created for the same interrupt number for multiple times, the interrupt handler function and input parameters of the function are added to the linked list corresponding to the interrupt number. Therefore, when the hardware generates an interrupt, the corresponding linked list is found based on the interrupt number, and the linked list is traversed until the interrupt handler function of the corresponding device is found.

-   The interrupt mechanism of Huawei LiteOS supports interrupt nesting.
    -   The interrupt nesting of the GIC and NVIC is implemented by hardware.
    -   The interrupt nesting mechanism of the RISC-V is as follows: In interrupt nesting mode, after interrupt A is triggered, the current operation is pushed to the stack. Before the interrupt handler is called, the MIE is set to  **1**  to allow new interrupts to be responded. During the execution of interrupt handler by interrupt A, if interrupt B with a higher priority is triggered, interrupt B pushes the current operation, that is, the operation related to interrupt A, into the stack, and then executes the interrupt handler of interrupt B. After the interrupt handler of interrupt B is executed, the MIE field in the mstatus register is temporarily set to  **0**, the interrupt response is disabled, the operations related to interrupt A are popped from the stack, and the MIE field is set to  **1**  to allow the processor to respond to the interrupt again. After interrupt B ends, interrupt A continues to be executed.



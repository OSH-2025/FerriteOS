# Introduction to Each Module<a name="EN-US_TOPIC_0305675049"></a>

**Task Management**

Creates, deletes, delays, suspends, and resumes tasks; locks or unlocks task scheduling. High-priority tasks preempt resources from low-priority ones. Tasks with the same priority are scheduled in a round-robin way using time slices.

**Memory Management**

-   Supports memory application and free-up; provides the static memory management algorithm BOX as well as dynamic memory management algorithms bestfit and bestfit\_little.
-   Provides memory statistics and memory overwriting detection.

**Hardware-related Functions**

Provides the following functions:

-   Interrupt management: creates, deletes, enables, and disables interrupts; clears interrupt request flags.
-   Error handling: prints the information stored in the call stack of the abnormal function or saves the current OS state when exceptions occur at runtime.
-   Tick: the basic time unit used in OS scheduling. Duration is measured by the number of ticks per second, which is configurable.

**Inter-process Communication \(IPC\)**

Provides the following functions:

-   Message queue: creates, deletes, reads from, and writes into message queues.

-   Event: reads and writes events.
-   Semaphore: creates, deletes, applies for, and releases semaphores.
-   Mutex: creates, deletes, applies for, and releases mutexes.

**Software Timer**

Creates, deletes, starts, and stops timers.

**Spin Lock**

Initializes, applies for, and releases spin locks in SMP scenarios.

**Low Power Consumption**

-   Run-stop: also known as hibernation and wakeup. This mechanism saves onsite system images so that the system can be recovered from these images when necessary.
-   Tickless: This mechanism calculates the time of the next meaningful clock interrupt to reduce unnecessary clock interrupts, thereby lowering power consumption. If the tickless function is enabled, the system will start the tickless mechanism when CPUs are idle.

**Maintenance and Test**

-   CPU usage: provides the CPU usage of the system or any specified task.
-   Tracing: obtains the context of events in real time and writes the context into the buffer. You can customize the trace buffer, including designating the modules to trace, starting or stopping tracing, and clearing or outputting data in the buffer.
-   Lite memory sanitizer \(LMS\): checks the validity of memory operations in real time. LMS can detect memory problems such as buffer overflow, use after free, double free, and wild pointer free.

-   Shell: receives users' commands through a serial port, and calls and executes applications through commands. Huawei LiteOS Shell supports basic debugging functions and allows users to add user-defined commands.

**C++ Support**

Huawei LiteOS supports some standard template library \(STL\) features, exceptions, and runtime type identification \(RTTI\) features. Other features are supported by the compiler.


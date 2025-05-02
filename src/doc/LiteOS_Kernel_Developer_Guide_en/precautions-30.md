# Precautions<a name="EN-US_TOPIC_0311018422"></a>

-   Mutexes cannot be used in the interrupt service program.
-   As a real-time OS, Huawei LiteOS needs to ensure real-time task scheduling and avoid long-time task blocking. Therefore, a mutex needs to be released as soon as possible after being obtained.
-   In Huawei LiteOS, do not call the LOS\_TaskPriSet API to change the priority of a task that holds a mutex.
-   Mutexes do not support the scenario where multiple tasks with the same priority are reversed.


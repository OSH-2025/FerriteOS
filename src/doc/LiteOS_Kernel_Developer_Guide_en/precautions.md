# Precautions<a name="EN-US_TOPIC_0311018322"></a>

-   Running idle tasks reclaims the TCBs and stacks of previously deleted tasks.
-   A task name is a pointer without memory space. Do not set a task name to the address of a local variable.
-   The task stack size is aligned by 16 bytes or  **sizeof\(UINTPTR\)**  x 2. Follow the "nothing more and nothing less" principle while determining the task stack size.
-   A running task cannot be suspended while current task scheduling is locked.
-   Idle and software timer tasks must not be suspended or deleted.
-   In the interrupt handler function or in the case of the lock task, the operation that calls the LOS\_TaskDelay API will fail.
-   Locking task scheduling does not disable interrupts. Tasks can still be interrupted while task scheduling is locked.
-   Locked task scheduling and unlocked task scheduling must be used in coordination.
-   Task scheduling may occur while a task priority is being set.
-   The maximum number of tasks able to be set by operating system is not equal to the total number of tasks available to users. For example, if the system software timer occupies one more task resource, the number of available task resources decreases by one.
-   The LOS\_CurTaskPriSet or LOS\_TaskPriSet API must not be used when interrupts are being processed.
-   If the task corresponding to the task ID sent by the LOS\_TaskPriGet API has not been created or exceeds the maximum number of tasks, 0xffff will be returned.
-   Resources such as a mutex or a semaphore allocated to a task must have been released when the task is being deleted.

-   In SMP mode, the LOS\_TaskLock API can only be used to lock the scheduler of the current core. Tasks on other cores can still be scheduled properly.
-   In SMP mode, tasks are deleted or suspended in asynchronous mode. Therefore, the return value of an operation does not indicate the final result but only indicates that the request has been sent. Additionally, there is a delay in completing the preceding operation.
-   In multi-core mode, if the cross-core deletion synchronization function \(LOSCFG\_KERNEL\_SMP\_TASK\_SYNC option\) is enabled, the result is returned only after the target task is deleted when the cross-core deletion task is executed. If the task is not deleted within the specified time, the error LOS\_ERRNO\_TSK\_MP\_SYNC\_FAILED is returned. After this function is enabled, each task increases the overhead of one semaphore.


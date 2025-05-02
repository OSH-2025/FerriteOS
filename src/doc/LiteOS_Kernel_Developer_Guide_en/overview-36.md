# Overview<a name="EN-US_TOPIC_0311018430"></a>

In a multi-core environment, multiple tasks may access the same resource due to the shared memory. An exclusive access mechanism is required to ensure that only one core performs operations at a time. Spinlocks are used as such a mechanism.

A spinlock is a lock which causes a thread trying to acquire it to simply wait in a loop \("spin"\) while repeatedly checking if the lock is available. The thread exits the loop only until the lock is acquired. Therefore, spinlocks are recommended for operations that take a short time, preventing adverse impact on system performance.

Similar to mutexes, spinlocks are designed to grant a task exclusive use of shared resources. Either a mutex or a spinlock can have only one holder at a given point in time. However, the scheduling mechanisms of the two modes are slightly different. For a mutex, if a common resource is occupied, other applicants will be blocked. A spinlock, however, does not cause caller blocking, and applicants keep checking whether the holder releases the spinlock.


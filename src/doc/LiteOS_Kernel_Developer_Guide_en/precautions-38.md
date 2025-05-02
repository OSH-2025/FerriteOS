# Precautions<a name="EN-US_TOPIC_0311018432"></a>

-   Tasks are unable to lock the same spinlock for multiple times, which causes a deadlock.
-   Spinlocks are used to lock tasks. Therefore, task scheduling can be performed only after the outermost layer is unlocked.
-   LOS\_SpinLock and LOS\_SpinUnlock can be used independently. That is, the interrupt can be left enabled, but you need to ensure that the called APIs are used either in tasks or interrupts. If the called APIs are used in both tasks and interrupts, use LOS\_SpinLockSave and LOS\_SpinUnlockRestore. In this case, using LOS\_SpinLock may cause deadlock when the interrupt is enabled.
-   Exercise caution when using spinlocks for time-consuming operations. You can use mutexes for protection.
-   In the single-core scenario with SMP disabled, the spinlock function is invalid. Only the LOS\_SpinLockSave and LOS\_SpinUnlockRestore APIs can disable the interrupt recovery function.
-   It is recommended that LOS\_SpinLock and LOS\_SpinUnlock be used together, and LOS\_SpinLockSave and LOS\_SpinUnlockRestore be used together to avoid errors.


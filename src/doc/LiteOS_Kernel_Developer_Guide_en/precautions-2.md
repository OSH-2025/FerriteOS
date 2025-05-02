# Precautions<a name="EN-US_TOPIC_0311018389"></a>

-   The dynamic memory module manages memory through control block structures that consume extra memory. Therefore, the memory space available to users is usually less than the value of the configuration item  **OS\_SYS\_MEM\_SIZE**.
-   The LOS\_MemAllocAlign and LOS\_MemMallocAlign APIs may consume extra memory for memory alignment, leading to memory loss. When the memory used for alignment is freed up, the lost memory will be reclaimed.
-   If the LOS\_MemRealloc or LOS\_MemMrealloc API successfully re-allocates memory, the system determines whether it is necessary to free up the previously applied memory and returns the re-allocated memory address. If the reallocation fails, the previously allocated memory remains unchanged, and NULL will be returned. The use of pPtr = LOS\_MemRealloc\(pool, pPtr, uwSize\) is not allowed, indicating that using the original pPtr to receive the returned value is forbidden.
-   If the LOS\_MemFree or LOS\_MemMfree API is called multiple times for the same memory block, only the first attempt succeeds. Subsequent attempts will be regarded as invalid pointers and may cause unpredictable results.
-   In the memory node control block structure LosMemDynNode managed by the dynamic memory module,  **sizeAndFlag**  is of the UINT32 type, wherein the most significant two bits indicate the flag and the remaining 30 indicate the size of the memory node. Therefore, the size of the initial memory pool cannot exceed 1 GB. Otherwise, unpredictable results may occur.


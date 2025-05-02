# Precautions<a name="EN-US_TOPIC_0311040151"></a>

-   The maximum number of queues supported by the system refers to the total number of queue resources in the entire system instead of the number of queue resources that can be used by users. For example, if the system software timer occupies one more queue resource, the number of available queue resources decreases by one.
-   The queue name and flags input during queue creation are not used currently and are reserved for future use.
-   The input parameter  **timeout**  in the queue interface function is a relative time.
-   LOS\_QueueReadCopy, LOS\_QueueWriteCopy, and LOS\_QueueWriteHeadCopy are a group of APIs; LOS\_QueueRead, LOS\_QueueWrite, and LOS\_QueueWriteHead are a group of APIs. The two groups of APIs must be used together.
-   As the LOS\_QueueWrite, LOS\_QueueWriteHead, and LOS\_QueueRead APIs are called to perform operations on data addresses, you must ensure that the memory area to which the pointer obtained by calling LOS\_QueueRead points is not modified or released abnormally when the queue is being read. Otherwise, unpredictable results may occur.
-   The LOS\_QueueWrite, LOS\_QueueWriteHead, and LOS\_QueueRead APIs are called to operate data addresses, which means that the actual write or read data length is the same as that of a pointer. Therefore, before using these APIs, ensure that the message node size is the same as the pointer length during queue creation, to avoid unnecessary waste and read failures.
-   After the queue is used, if the memory that is dynamically allocated exists, the memory needs to be released in a timely manner.


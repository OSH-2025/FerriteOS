# Precautions<a name="EN-US_TOPIC_0311018427"></a>

-   Limit the number of operations contained in the callback function of a software timer. You are not advised to use APIs or perform operations that may suspend or block tasks. Otherwise, the software timer will be inaccurate, bringing unpredictable impacts.
-   The software timer uses a queue and a task resource of the system. The priority of a task in a software timer is set to 0, which cannot be modified.
-   The number of software timers that can be configured in the system refers to the total number of software timers that can be used in the entire system rather than that can be used by users. For example, if the system software timer occupies one more software timer, the software timers that can be used by users decreases by 1.
-   A one-shot software timer that will not be automatically deleted after expiration needs to be deleted by calling the LOS\_SwtmrDelete API. Resources allocated to the timer are reclaimed to avoid resource leaks.
-   The timing precision of software timers is related to the tick clock period of the system.


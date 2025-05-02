# Overview<a name="EN-US_TOPIC_0311018446"></a>

## Basic Concepts<a name="en-us_topic_0175230399_section64131474101014"></a>

In a multi-tasking operating system \(OS\), data reading, modification, and writing are three essential steps of modifying the data in a specified memory. However, the data may be concurrently accessed by multiple tasks. If the data modification is interrupted by other tasks, unexpected modification result will occur.

Multiple tasks can be successfully executed by enabling and disabling interrupts, but this method affects OS performance.

The ARMv6 architecture introduces the LDREX and STREX instructions to support more discreet non-blocking synchronization of the shared memory. The atomic operation ensures that a "read-modify-write" operation on same data is not interrupted during execution of the operation, that is, atomicity of the operation.

## Operation Mechanism<a name="en-us_topic_0175230399_section1775887101044"></a>

Huawei LiteOS provides atomic operation APIs by encapsulating LDREX and STREX of the ARMv6 architecture.

-   **LDREX Rx, \[Ry\]**

    The following is the method of reading data in the memory and mark the exclusive access to the memory:

    -   Read the 4-byte memory data pointed to by the Ry register and store the read memory data to the Rx register.
    -   Add an exclusive access flag to the memory segment pointed by Ry.


-   **STREX Rf, Rx, \[Ry\]**

    The following describes the method of checking the memory has the exclusive access flag, and updating the memory and clearing the flag if the flag exist:

    -   If the memory has an exclusive access flag, the memory data will be updated.
        1.  Update the memory data pointed to the Ry register to the value in the Rx register.
        2.  Set the Rf flag register to 0.

    -   If the memory does not have an exclusive access flag, the memory data should be updated.
        1.  The memory data pointed to by Ry will not be updated.
        2.  Set the Rf flag register to 1.


-   Flag register
    -   If the flag register is 0, the loop exits and the atomic operation ends.
    -   If the flag register is 1, the atomic operation cycle proceeds and the atomic operation starts again.



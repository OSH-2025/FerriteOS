# Overview<a name="EN-US_TOPIC_0312244430"></a>

## Basic Concepts<a name="en-us_topic_0287763513_section7842986141442"></a>

Lite Memory Sanitizer \(LMS\) is a commissioning tool used to check the validity of memory operations in real time. The LMS can detect buffer overflow, use after free, and wild pointer in real time, and can notify the operating system immediately when an exception occurs. With the help of locating methods such as Backtrace, the LMS can locate the code line where the memory problem occurs. This greatly improves the efficiency of locating memory problems.

## Operational Mechanism<a name="en-us_topic_0287763513_section4198546414140"></a>

The LMS uses the shadow memory mapping to mark the status of the system memory. There are four states: readable and writable, unreadable and unwritable, partially readable and writable, as well as released. The shadow memory is stored in the tail node of the memory pool.

-   When a program is compiled, a red zone is inserted on both ends of local variables such as arrays and structs, and the shadow memory mapped to the red zone is set to be the unreadable and unwritable state.
-   When the memory is released from the heap, the shadow memory of the released memory is set to the released state.
-   During code compilation, a detection function is inserted before the read/write instruction in the code to check the validity of the address. Generally, the status value of the shadow memory that accesses the memory is checked. If the shadow memory is unreadable and unwritable, an overflow error is reported. If the shadow memory is released, an Use-After-Free error is reported.


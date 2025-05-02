# Overview<a name="EN-US_TOPIC_0311018436"></a>

## Basic Concepts<a name="en-us_topic_0175230573_section6609263414935"></a>

Time management provides time services to applications and uses system time as the reference time.

System time is generated when an output pulse of a timer/counter triggers an interrupt, it is an integral number or long integral number of ticks. The period of the output pulse is called a "clock tick". The system clock is also called time scale or tick.

User time is measured in seconds or milliseconds, whereas the OS clock measures time in ticks. When a user initiates an operation to the operating system, such as suspending or delaying a task, the time management module converts user time between seconds/milliseconds and ticks.

The time management module of Huawei LiteOS provides time conversion, measurement, and deferral to satisfy what users need.

## Related Concepts<a name="en-us_topic_0175230573_section12490545164852"></a>

-   Cycle

    Cycle is the minimal time unit of the operating system. The system clock speed is represented in the form of cycles per second.


-   Tick

    A tick is the basic time unit used in OS. The tick length is user configurable. Typically, it is determined by the system clock speed and represented in the form of ticks per second.



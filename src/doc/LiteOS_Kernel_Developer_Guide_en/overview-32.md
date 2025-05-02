# Overview<a name="EN-US_TOPIC_0311018425"></a>

## Basic Concepts<a name="en-us_topic_0175230194_section50571083144137"></a>

A software timer is a timer simulated by software, and works based on system tick interrupts. When a predefined number of ticks elapse, a software timer triggers a user-defined callback function. The timer length is an integral number of ticks.

Only a limited number of hardware timers can be used due to hardware constraints. To meet user requirements of more timers, Huawei LiteOS provides the software timer function and supports the following features:

-   Creates a software timer.
-   Starts a software timer.
-   Stops a software timer.
-   Deletes a software timer.
-   Obtain the number of remaining ticks of a software timer.
-   Configures the number of supported software timers.

## Operation Mechanism<a name="en-us_topic_0175230194_section2437362619634"></a>

Software timers are system resources and are allocated continuous memory at the initialization of the timer module.

Software timers are placed in a queue and a triggered in the first in first out order. The software timers with a short life cycle are placed at the beginning of queue so that they will be triggered earlier than those with a longer life cycle. The timer with a shorter time interval is always closer to the queue head than the timer with a longer time interval, and is therefore preferentially triggered.

The software timer uses ticks as the basic timing unit. When a software timer is created and started, Huawei LiteOS determines the timer expiry time based on the current system time \(in ticks\) and the specific time interval \(in ticks\) and adds the timer control structure to the global timing list.

When a tick interrupt occurs, the tick interrupt processing function scans the global timing list for expired timers. If such a timer is found, the timer is recorded. After the tick interrupt processing function finishes processing, the software timer task \(with the highest priority\) is woken up. In this task, the recorded timer callback function is called.

**Software timer states**

-   OS\_SWTMR\_STATUS\_UNUSED \(The timer is not used.\)

    While the timer module is being initialized, the operating system initializes all timer resources in the system to OS\_SWTMR\_STATUS\_UNUSED state.


-   OS\_SWTMR\_STATUS\_TICKING \(The timer is timing.\)

    If the LOS\_SwtmrStart API is called after the timer is created, the timer switches to OS\_SWTMR\_STATUS\_TICKING state.

-   OS\_SWTMR\_STATUS\_CREATED \(The timer is not started or had stopped.\)

    After a timer is created, it enters this state if it is not working.


**Software timer modes**

Depending on timer mode, software timers are classified into three types:

-   One-shot timer: The timer triggers the timer event only once after it is started. Then, the timer is automatically deleted.
-   Periodic timer: The timer triggers the timer event periodically until the timer is manually stopped.
-   One-shot timer: The timer will not be automatically deleted after it expires. You need to call the LOS\_SwtmrDelete API to delete this timer.


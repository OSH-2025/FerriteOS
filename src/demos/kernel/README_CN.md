KERNEL DEMOS使用指南
================
## 目 录
* [1.前言](#1)
* [2.基本实例介绍](#2)
    * [2.1 任务](#a)
    * [2.2 内存管理](#b)
    * [2.3 中断](#c)
    * [2.4 IPC通信](#d)
    * [2.5 时间管理](#e)
    * [2.6 双向链表](#f)
* [3.可操作的测试实例介绍](#3)
    * [3.1 测试任务实例](#g)
    * [3.2 测试队列实例](#h)
    * [3.3 测试事件实例](#i)
    * [3.4 测试互斥锁实例](#j)
    * [3.5 测试信号量实例](#k)
* [4.运行实例](#4)


>![](public_sys-resources/icon-notice.gif) **须知：**
>如果想更详细了解LiteOS各内核模块，可以访问<a href="/doc/LiteOS_Developer_Guide.md" target="_blank">LiteOS开发指南</a>。
<h2 id="1">1.前言</h2>

**使用方法**

用户可以直接参考[4.运行实例](#4)来运行demos/kernel下各个实例，也可以直接参考华为云上的沙盒实验来操作[3.可操作的测试实例介绍](#3)中的各个实例。

具体的实例介绍在[2.基本实例介绍](#2)中，下面是对各个实例的概述：

**任务**

los_api_task.c/.h实现了任务实例。

-   实现了任务的创建、删除、延迟、挂起、恢复等功能，以及锁定和解锁任务调度。

**内存管理**

los_api_dynamic_mem.c/.h实现了动态内存实例
-   实现了动态内存的初始化、申请、使用、释放功能。

los_api_static_mem.c/.h实现了静态内存实例
-   实现了静态内存的初始化、申请、使用、清除和释放功能。

**中断**

los_api_interrupt.c/.h实现了中断实例。
-   实现了中断的创建、删除、使能、禁止、请求位的清除等功能。

**IPC通信**

los_api_msgqueue.c/.h实现了消息队列实例。
-   实现了消息队列的创建、删除、发送和接收功能。

los_api_event.c/.h实现了事件实例。
-   实现了事件的创建、删除、读事件和写事件功能。

los_api_mutex.c/.h实现了互斥锁实例。
-   实现了互斥锁的创建、删除、申请和释放等功能。

los_api_sem.c/.h实现了信号量实例。
-   实现了信号量的创建、删除、申请和释放等功能。

**时间管理**

los_api_systick.c/.h实现了tick时间实例。
-   实现了获取系统Tick数和每个Tick的Cycle数的功能。

los_api_timer.c/.h实现了软件定时器实例。
-   实现了软件定时器的创建、开始、停止、删除等功能。

**双向链表**

los_api_list.c/.h实现了双向链表实例
-   实现了双向链表的初始化，链表的非空检测，节点的增加、删除等功能。

<h2 id="2">2.基本实例介绍</h2>

<h3 id="a">2.1 任务</h3>

**任务实例：<a href="./api/los_api_task.c" target="_blank">los_api_task.c</a>/<a href="./include/los_api_task.h" target="_blank">los_api_task.h</a>**

任务是竞争系统资源的最小运行单元。任务可以使用或等待CPU、使用内存空间等系统资源，并独立于其它任务运行。

本实例执行以下步骤：

1.  锁任务LOS\_TaskLock，防止创建任务时发生任务调度。
2.  通过LOS\_TaskCreate创建了2个任务，高优先级任务HiTaskEntry和低优先级任务LoTaskEntry，解锁任务LOS\_TaskUnlock。
3.  HiTaskEntry任务被调度，通过LOS\_TaskSuspend挂起任务。
4.  LoTaskEntry任务被调度，通过LOS\_TaskResume恢复挂起的HiTaskEntry任务。
5.  HiTaskEntry任务被唤醒，执行完成，LoTaskEntry任务被调度，执行完成。

**实例结果**

```
Kernel task demo start to run.
Create high priority task successfully.
Create low priority task successfully.
Enter high priority task handler.
Enter low priority task handler.
High priority task LOS_TaskDelay successfully.
High priority task LOS_TaskSuspend successfully.
High priority task LOS_TaskResume successfully.
Kernel task demo finished.
```

<h3 id="b">2.2 内存管理</h3>

Huawei LiteOS的内存管理分为静态内存管理和动态内存管理，提供内存初始化、分配、释放等功能。

**动态内存管理实例：<a href="./api/los_api_dynamic_mem.c" target="_blank">los_api_dynamic_mem.c</a>/<a href="./include/los_api_dynamic_mem.h" target="_blank">los_api_dynamic_mem.h</a>**

当用户需要使用内存时，可以通过操作系统的动态内存申请函数申请指定大小的内存，动态内存管理模块会从系统配置的一块连续内存（内存池）中为用户分配指定大小的内存块。一旦使用完毕，用户再通过动态内存释放函数归还所占用内存，使之可以重复使用。

本实例执行以下步骤：

1.  通过LOS\_MemInit初始化一个动态内存池。
2.  使用LOS\_MemAlloc从动态内存池中申请一块内存。
3.  在这个内存块中存放一个数据。
4.  打印出这个内存块中的数据。
5.  通过LOS\_MemFree释放这块内存。

**实例结果**

```
Kernel dynamic memory demo start to run.
Mempool init successfully.
Mem alloc successfully.
*mem = 828.
Mem free successfully.
Kernel dynamic memory demo finished.
```

**静态内存管理实例：<a href="./api/los_api_static_mem.c" target="_blank">los_api_static_mem.c</a>/<a href="./include/los_api_static_mem.h" target="_blank">los_api_static_mem.h</a>**

静态内存池由一个控制块和若干相同大小的内存块构成。控制块位于内存池头部，用于内存块管理。内存块大小在内存池初始化时设定，初始化后内存块的大小不可变更。所以静态内存池是由若干大小相同的内存块构成的。内存块的申请和释放以块大小为粒度。

本实例执行以下步骤：

1.  通过LOS\_MemboxInit初始化一个静态内存池。
2.  使用LOS\_MemboxAlloc从静态内存池中申请一块内存。
3.  在这个内存块中存放一个数据。
4.  打印出内存块中的数据。
5.  通过LOS\_MemboxClr清除内存块中的数据。
6.  通过LOS\_MemboxFree释放这块内存。

**实例结果**

```
Kernel static memory demo start to run.
Mem box init successfully.
Mem box alloc successfully.
*mem = 828.
Clear data ok, *mem = 0.
Mem box free successfully.
Kernel static memory demo finished.
```

<h3 id="c">2.3 中断</h3>

**中断实例：<a href="./api/los_api_interrupt.c" target="_blank">los_api_interrupt.c</a>/<a href="./include/los_api_interrupt.h" target="_blank">los_api_interrupt.h</a>**

中断是指出现需要时，CPU暂停执行当前程序，转而执行新程序的过程。即在程序运行过程中，出现了一个必须由CPU立即处理的事务。此时，CPU暂时中止当前程序的执行转而处理这个事务，这个过程就叫做中断。

本实例以软件中断为例来说明中断使用的一般步骤：

1.  通过LOS\_IntLock关中断。
2.  通过LOS\_HwiCreate创建中断。
3.  通过LOS\_HwiEnable使能中断。
4.  通过LOS\_IntRestore开中断。
5.  通过LOS\_HwiTrigger触发中断。

**实例结果**

```
Kernel interrupt demo start to run.
User IRQ test successfully.
Kernel interrupt demo successfully.
```

**说明：** 目前的中断测试代码提供了基本框架，中断硬件初始化代码请用户根据开发板硬件情况在ExampleExti0Init\(\)函数中自行实现。

以下以野火挑战者F429开发板为例来说明硬件中断使用的一般步骤：

1.  通过LOS\_IntLock关中断。
2.  通过Exti0Init初始化按键PA0。
2.  通过LOS\_HwiCreate创建中断。
3.  通过LOS\_HwiEnable使能中断。
4.  通过LOS\_IntRestore开中断。
5.  通过硬件按键PA0触发中断。
6.  在中断处理程序中通过\_\_HAL\_GPIO\_EXTI\_CLEAR\_IT清除硬件中断。

**实例结果**

按下PA0引脚对应的按键（即野火挑战者F429开发板上的对应的KEY1按键）串口输出如下：

```
Button IRQ test successfully.
```

**注意事项：** 当前LiteOS系统对于Cortex-M系列芯片，前16个中断号为系统预留，不开放给用户使用。

<h3 id="d">2.4 IPC通信</h3>

IPC通信提供消息队列、事件、互斥锁和信号量功能。

**消息队列实例：<a href="./api/los_api_msgqueue.c" target="_blank">los_api_msgqueue.c</a>/<a href="./include/los_api_msgqueue.h" target="_blank">los_api_msgqueue.h</a>**

队列又称消息队列，是一种常用于任务间通信的数据结构，能够接收来自任务或中断的不固定长度的消息，并根据不同的接口选择消息是否存放在自己空间。任务能够从队列里读取消息，当队列中的消息为空时，挂起读取任务；当队列中有新消息时，挂起的读取任务被唤醒并处理新消息。

本实例执行以下步骤：

1.  在MsgQueueDemo中锁任务调度，通过LOS\_TaskCreate创建两个任务SendTaskEntry和ReceiveTaskEntry。
2.  在MsgQueueDemo中通过LOS\_QueueCreate创建一个消息队列。
3.  解锁任务调度，任务SendTaskEntry被调度，该任务循环通过LOS\_QueueWrite以非阻塞模式向队列写入消息，写入一条消息后休眠50Tick，循环写API_MSG_NUM条消息。
4.  任务ReceiveTaskEntry被唤醒，该任务循环通过LOS\_QueueRead以非阻塞模式从队列中读取消息，读完一条消息后就休眠50Tick，一旦任务无法从队列中读取消息，就退出循环，并通过LOS\_QueueDelete删除队列。
5.  通过LOS_TaskDelete删除SendTaskEntry和ReceiveTaskEntry两个任务。

**实例结果**

```
Kernel message queue demo start to run.
Create the queue successfully.
Receive message : test is message 0.
Receive message : test is message 1.
Receive message : test is message 2.
Receive message : test is message 3.
Receive message : test is message 4.
Receive message failed, error: 200061d.
Delete the queue successfully.
Kernel message queue demo finished.
```

**事件实例：<a href="./api/los_api_event.c" target="_blank">los_api_event.c</a>/<a href="./include/los_api_event.h" target="_blank">los_api_event.h</a>**

事件是一种实现任务间通信的机制，可用于实现任务间的同步。一个任务可以等待多个事件的发生：可以是任意一个事件发生时唤醒任务进行事件处理，也可以是几个事件都发生后才唤醒任务进行事件处理。

本实例执行以下步骤：

1.  在EventDemo中初始化事件控制块，并创建任务ReadTaskEntry。任务ReadTaskEntry的优先级高于EventDemo函数所在任务。
2.  ReadTaskEntry被调度，通过LOS\_EventRead读事件0x00000001，超时时间为100 Ticks，此时无法读取到该事件，ReadTaskEntry阻塞挂起。
3.  EventDemo被调度，通过LOS\_EventWrite写事件0x00000001。
4.  ReadTaskEntry读取到事件0x00000001，被唤醒，任务结束执行。
5.  EventDemo被调度，通过LOS\_EventClear清除事件类型，任务结束执行。

**实例结果**

```
Kernel event demo start to run.
Read event task wait event 0x1.
Write event.
Read the event : 0x1.
Current event id : 1
Current event id : 0
Kernel event demo finished.
```

**互斥锁实例：<a href="./api/los_api_mutex.c" target="_blank">los_api_mutex.c</a>/<a href="./include/los_api_mutex.h" target="_blank">los_api_mutex.h</a>**

互斥锁又称互斥型信号量，是一种特殊的二值性信号量，用于实现对共享资源的独占式处理。

本实例执行以下步骤：

1.  在MutexLockDemo中创建一个互斥锁，锁任务调度，创建两个任务：低优先级任务MutexTask1Entry和高优先级任务MutexTask2Entry，解锁任务调度。
2.  MutexTask2Entry被调度，通过LOS_MuxPend以永久阻塞模式申请互斥锁，此时互斥锁没有被其他任务持有，它成功申请到锁后，开始休眠100Tick，MutexTask2Entry挂起。
3.  MutexTask1Entry被调度，申请互斥锁，其申请等待时间为10Tick，因互斥锁仍被MutexTask2Entry持有，MutexTask1Entry挂起10Tick后再次被唤醒，因仍未拿到互斥锁，MutexTask1Entry以永久阻塞模式申请互斥锁，因无法拿到锁，MutexTask1Entry挂起。
4.  100Tick后MutexTask2Entry休眠时间结束，其被唤醒后释放互斥锁，MutexTask2Entry结束执行。
5.  因MutexTask2Entry已经释放互斥锁，MutexTask1Entry被唤醒后拿到锁，接着释放锁，MutexTask1Entry结束执行。
6.  300Tick后MutexLockDemo被调度运行并删除互斥锁。

**实例结果**

```
Kernel mutex demo start to run.
Task2 try to get the mutex, wait forever.
Task2 get mutex g_demoMux01 and suspend 100 ticks.
Task1 try to get mutex, wait 10 ticks.
Task1 timeout and try to get mutex, wait forever.
Task2 resumed and post the g_demoMux01.
Task1 wait forever, got mutex g_demoMux01 successfully.
Kernel mutex demo finished.
```

**信号量实例：<a href="./api/los_api_sem.c" target="_blank">los_api_sem.c</a>/<a href="./include/los_api_sem.h" target="_blank">los_api_sem.h</a>**

信号量（Semaphore）是一种实现任务间通信的机制，实现任务之间同步或临界资源的互斥访问。常用于协助一组相互竞争的任务来访问临界资源。

本实例执行以下步骤：

1.  在SemphoreDemo中创建一个计数值为0的信号量，锁任务调度，创建两个任务：低优先级任务Task1Entry和高优先级任务Task2Entry，解锁任务调度。
2.  Task2Entry被调度，通过LOS\_SemPend以永久阻塞模式申请信号量后阻塞挂起。
3.  Task1Entry被调度，通过LOS\_SemPend以定时等待10Tick的方式申请信号量后阻塞挂起。
4.  SemphoreDemo被调度，执行LOS\_SemPost释放信号量。
5.  Task2Entry得到信号量，被唤醒，然后休眠20Tick，Task2Entry挂起。
6.  Task1Entry被调度，等待10Tick后仍未得到信号量，尝试以永久阻塞模式申请信号量，Task1Entry挂起。
7.  SemphoreDemo被唤醒，开始休眠40Tick，SemphoreDemo挂起。
8.  20Tick的休眠结束后，Task2Entry唤醒，释放信号量，Task2Entry结束执行。
8.  Task1Entry得到信号量，被唤醒后释放信号量，Task1Entry结束执行。
9.  40Tick的休眠结束后，SemphoreDemo被唤醒，并通过LOS\_SemDelete删除信号量。

**实例结果**

```
Kernel semaphore demo start to run.
Task2 try get semaphore g_demoSemId wait forever.
Task1 try get semphore g_demoSemId, timeout 10 ticks.
Task2 get semaphore g_demoSemId and then delay 20ticks.
Task1 timeout and try get semphore g_demoSemId wait forever.
Task2 post semaphore g_demoSemId.
Task1 wait_forever and got semphore g_demoSemId successfully.
Kernel semaphore demo finished.
```

<h3 id="e">2.5 时间管理</h3>

时间管理以系统时钟为基础。时间管理提供给应用程序所有和时间有关的服务。下面包括时钟管理和软件定时器两个实例。

**时钟管理实例：<a href="./api/los_api_systick.c" target="_blank">los_api_systick.c</a>/<a href="./include/los_api_systick.h" target="_blank">los_api_systick.h</a>**

本实例执行以下步骤：

1.  通过LOS\_MS2Tick将1000ms转换成tick数输出。
2.  通过LOS\_Tick2MS将1000个tick数转换成毫秒数输出。
3.  通过LOS\_CyclePerTickGet统计一个Tick的Cycle数。
4.  通过LOS\_TickCountGet获取自系统启动以来的Tick数，然后休眠200Tick后，再次调用LOS\_TickCountGet。将这两次获取的Tick数相减，其差应该大于等于休眠的200Tick。

**实例结果**

```
Kernel systick demo start to run.
LOS_MS2Tick tick = 1000.
LOS_Tick2MS ms = 1000.
LOS_CyclePerTickGet = 180000.
LOS_TickCountGet = 1132.
LOS_TickCountGet after delay = 1335.
Kernel systick demo finished.
```

**软件定时器实例：<a href="./api/los_api_timer.c" target="_blank">los_api_timer.c</a>/<a href="./include/los_api_timer.h" target="_blank">los_api_timer.h</a>**

软件定时器，是基于系统Tick时钟中断且由软件来模拟的定时器，当经过设定的Tick时钟计数后会触发用户定义的回调函数。定时精度与系统Tick时钟的周期有关。

本实例执行以下步骤：

1.  通过LOS\_SwtmrCreate创建两个定时器，定时1000Ticks的单次软件定时器Timer1和定时100Tick的周期软件定时器Timer2。
2.  通过LOS\_SwtmrStart启动Timer1。
3.  200Tick后通过LOS\_SwtmrStop停止Timer1，再通过LOS\_SwtmrStart启动Timer1。
4.  再经过1000Ticks后通过LOS\_SwtmrDelete删除Timer1（显示删除失败，此时Timer1定时器已经完成1000Ticks的定时，对于单次定时器，定时时间到达后系统会自动删除该定时器）。
5.  通过LOS\_SwtmrStart启动Timer2。
6.  休眠1000Ticks，在此期间，Timer2会触发10次回调函数。1000Ticks后停止并删除Timer2，任务结束执行。

**实例结果**

```
Kernel swtimer demo start to run.
Create software timer1 successfully.
Create software timer2 successfully.
Start software timer1 successfully.
Stop software timer1 successfully.
LOS_TickCountGet g_demoTimerCount1 = 1.
LOS_TickCountGet tickLast1 = 2559.
Delete software timer1 failed.
Start software timer2 successfully.
LOS_TickCountGet g_demoTimerCount2 = 1.
LOS_TickCountGet tickLast2 = 2670.
LOS_TickCountGet g_demoTimerCount2 = 2.
LOS_TickCountGet tickLast2 = 2770.
LOS_TickCountGet g_demoTimerCount2 = 3.
LOS_TickCountGet tickLast2 = 2870.
LOS_TickCountGet g_demoTimerCount2 = 4.
LOS_TickCountGet tickLast2 = 2970.
LOS_TickCountGet g_demoTimerCount2 = 5.
LOS_TickCountGet tickLast2 = 3070.
LOS_TickCountGet g_demoTimerCount2 = 6.
LOS_TickCountGet tickLast2 = 3170.
LOS_TickCountGet g_demoTimerCount2 = 7.
LOS_TickCountGet tickLast2 = 3270.
LOS_TickCountGet g_demoTimerCount2 = 8.
LOS_TickCountGet tickLast2 = 3370.
LOS_TickCountGet g_demoTimerCount2 = 9.
LOS_TickCountGet tickLast2 = 3470.
LOS_TickCountGet g_demoTimerCount2 = 10.
LOS_TickCountGet tickLast2 = 3570.
Kernel software timer demo finished.
```

<h3 id="f">2.6 双向链表</h3>

**双向链表实例：<a href="./api/los_api_list.c" target="_blank">los_api_list.c</a>/<a href="./include/los_api_list.h" target="_blank">los_api_list.h</a>**

双向链表是指含有往前和往后两个方向的链表，即每个结点中除存放下一个节点指针外，还增加一个指向其前一个节点的指针。其头指针head是唯一确定的。

本实例执行以下步骤：

1.  调用LOS\_MemAlloc生成链表的头指针。
2.  调用LOS\_ListInit初始化双向链表。
3.  调用LOS\_ListEmpty判断链表初始化是否成功。
4.  调用LOS\_MemAlloc生成两个中间节点和一个尾节点。
5.  调用LOS\_ListAdd在头指针之后插入上面的两个中间节点。
6.  调用LOS\_ListTailInsert插入尾节点。
7.  调用LOS\_ListDelete删除其中一个节点。

**实例结果**

```
Kernel list demo start to run.
Init list.
Node add and tail add.
Add node successfully.
Add tail successfully.
Delete node1.
Delete node1 successfully.
Kernel list demo finished.
```

<h2 id="3">3.可操作的测试实例介绍</h2>

**测试环境**

本实例仅支持在linux下编译，qemu仿真的realview-pbx-a9中运行。

<h3 id="g">3.1 测试任务实例</h3>

**测试任务使用的简单实例：<a href="./api/los_debug_task.c" target="_blank">los_debug_task.c</a>/<a href="./include/los_debug_task.h" target="_blank">los_debug_task.h</a>**

本实例执行以下步骤：

1.  通过LOS\_TaskCreate创建任务。
2.  通过LOS\_TaskInfoGet获取任务信息。
3.  循环所创建的任务TaskEntry。
4.  循环一定次数后完成测试，通过LOS\_TaskDelete来删除任务TaskEntry。

**实例结果**

```
Kernel debug task start to run.
Create the task successfully.
	Task information:
            Task name:task1
            Task id:6
            Task status:4
            Task priority:4
Start Scheduling.
Task running. Interval:1000
Task running. Interval:1000
Task running. Interval:1000
Task running. Interval:1000
Task running. Interval:1000
Delete the task successfully.
Kernel debug task finished.
```

<h3 id="h">3.2 测试队列实例</h3>

**测试队列使用的简单实例：<a href="./api/los_debug_queue.c" target="_blank">los_debug_queue.c</a>/<a href="./include/los_debug_queue.h" target="_blank">los_debug_queue.h</a>**

本实例执行以下步骤：

1.  在QueueDebug中锁任务调度，通过LOS\_TaskCreate创建两个任务 SendTaskEntry和ReceiveTaskEntry。
2.  在QueueDebug中通过LOS\_QueueCreate创建一个消息队列。
3.  解锁任务调度，任务SendTaskEntry被调度，该任务通过LOS\_QueueWriteCopy向队列写入消息。
4.  任务ReceiveTaskEntry被调度，该任务通过LOS\_QueueReadCopy从队列中读取消息。
5.  在任务ReceiveTaskEntry中通过LOS\_QueueInfoGet获取队列信息，并输出已使用的队列数queueInfo.usReadableCnt。
6.  通过LOS\_QueueDelete删除队列。
7.  通过LOS\_TaskDelete删除SendTaskEntry和ReceiveTaskEntry两个任务。

**实例结果**

```
Kernel debug queue start to run.
Create the queue successfully.
Send message ok, message: message
Receive message : message.
	Queue size : 10
	Current queue usage : 0
Send message ok, message: message
Receive message : message.
	Queue size : 10
	Current queue usage : 0
Send message ok, message: message
Receive message : message.
	Queue size : 10
	Current queue usage : 0
Send message ok, message: message
Receive message : message.
	Queue size : 10
	Current queue usage : 0
Send message ok, message: message
Receive message : message.
	Queue size : 10
	Current queue usage : 0
Delete the queue successfully.
Kernel debug queue finished.
```

<h3 id="i">3.3 测试事件实例</h3>

**测试事件使用的简单实例：<a href="./api/los_debug_event.c" target="_blank">los_debug_event.c</a>/<a href="./include/los_debug_event.h" target="_blank">los_debug_event.h</a>**

本实例执行以下步骤：

1.  在EventDebug中初始化事件控制块，并创建任务ReadTaskEntry。
2.  在EventDebug中通过LOS\_EventWrite写事件0x01，然后通过LOS\_TaskUnlock开启调度。
3.  ReadTaskEntry被调度，通过LOS\_EventRead读事件0x01。
4.  延时2500个ticks后EventDebug被调度，通过LOS\_EventClear清除事件0x01。
5.  ReadTaskEntry被调度，读取事件0x01超时报错。
6.  ReadTaskEntry任务完成测试，通过LOS\_EventDestroy删除事件。

**实例结果**

```
Kernel debug event start to run.
Init the event successfully.
Write event.
Read the event : 0x1.
Read the event : 0x1.
Read the event : 0x1.
Clear the event flag.
Read the event timeout.
Read the event timeout.
Delete the event successfully.
Kernel debug event finished.
```

<h3 id="j">3.4 测试互斥锁实例</h3>

**测试互斥锁使用的简单实例：<a href="./api/los_debug_mutex.c" target="_blank">los_debug_mutex.c</a>/<a href="./include/los_debug_mutex.h" target="_blank">los_debug_mutex.h</a>**

本实例执行以下步骤：

1.  在MutexDebug中创建一个互斥锁，锁任务调度，创建两个任务：高优先级任务TaskHiEntry和低优先级任务TaskLoEntry，解锁任务调度。
2.  TaskHiEntry被调度，通过LOS\_MuxPend申请互斥锁，此时互斥锁没有被其他任务持有，它成功申请到锁后，开始休眠1000Tick，TaskHiEntry挂起。
3.  TaskLoEntry被调度，申请互斥锁，因互斥锁仍被TaskHiEntry持有，TaskLoEntry挂起，根据互斥锁超时时间来决定休眠时间。
4.  1000Tick后TaskHiEntry休眠时间结束，其被唤醒后释放互斥锁。
5.  TaskLoEntry获得互斥锁，开始休眠1000Tick，TaskLoEntry挂起。
6.  1000Tick后TaskLoEntry休眠时间结束，其被唤醒后释放互斥锁。
7.  循环一定次数后结束，通过LOS\_MuxDelete来删除互斥锁。

**实例结果**

```
Kernel debug mutex start to run.
Mutex init.
Create the mutex successfully.
High priority task get the mutex successfully.
High priority task release the mutex.
Low priority task get the mutex successfully.
Low priority task release the mutex.
High priority task get the mutex successfully.
High priority task release the mutex.
Low priority task get the mutex successfully.
Low priority task release the mutex.
High priority task get the mutex successfully.
High priority task release the mutex.
Low priority task get the mutex successfully.
Low priority task release the mutex.
High priority task get the mutex successfully.
High priority task release the mutex.
Low priority task get the mutex successfully.
Low priority task release the mutex.
High priority task get the mutex successfully.
High priority task release the mutex.
Low priority task get the mutex successfully.
Low priority task release the mutex.
Delete the mutex successfully.
Kernel debug mutex finished.
```

<h3 id="k">3.5 测试信号量实例</h3>

**测试信号量使用的简单实例：<a href="./api/los_debug_sem.c" target="_blank">los_debug_sem.c</a>/<a href="./include/los_debug_sem.h" target="_blank">los_debug_sem.h</a>**

本实例执行以下步骤：

1.  在SemphoreDebug中创建一个计数值为0的信号量，锁任务调度，创建两个任务：低优先级任务LoTaskEntry和高优先级任务HiTaskEntry，解锁任务调度。
2.  HiTaskEntry被调度，通过LOS\_SemPend申请信号量，未申请到后挂起。
3.  LoTaskEntry被调度，通过LOS\_SemPend申请信号量，未申请到后挂起。
4.  SemphoreDebug被调度，执行LOS\_SemPost两次以释放两个信号量。
5.  HiTaskEntry和LoTaskEntry得到信号量，被唤醒，然后休眠500Tick。
6.  500Tick后HiTaskEntry和LoTaskEntry被调度，通过LOS\_SemPost释放信号量并休眠500Tick。
7.  循环一定次数后结束，通过LOS\_SemDelete来删除信号量。

**实例结果**

```
Kernel debug semphore start to run.
Create the semphore.
Create the semphore successfully.
High priority task run.
Low priority task run.
Release the semaphore twice.
High priority task get the semphore successfully.
Low priority task get the semphore successfully.
High priority task release the semphore.
Low priority task release the semphore.
High priority task get the semphore successfully.
Low priority task get the semphore successfully.
High priority task release the semphore.
Low priority task release the semphore.
High priority task get the semphore successfully.
Low priority task get the semphore successfully.
High priority task release the semphore.
Low priority task release the semphore.
High priority task get the semphore successfully.
Low priority task get the semphore successfully.
High priority task release the semphore.
Low priority task release the semphore.
High priority task get the semphore successfully.
Low priority task get the semphore successfully.
High priority task release the semphore.
Low priority task release the semphore.
Delete the semaphore successfully.
Kernel debug semphore finished.
```

<h2 id="4">4.运行实例</h2>

这里提供两种方式来运行各个实例。

### 方式1：通过宏开关来使能LOSCFG\_DEMOS\_KERNEL\_INSPECT\_ENTRY，以实现对[2.基本实例介绍](#2)中的所有实例的执行。

**实例：**<a href="./api/los_inspect_entry.c" target="_blank">los_inspect_entry.c</a>/<a href="./include/los_inspect_entry.h" target="_blank">los_inspect_entry.h</a>

执行make menuconfig打开宏开关：

`Demos --> Kernel Demo --> Enable Kernel Demo --> Kernel Demo Entry --> InspectEntry`

### 方式2：通过宏开关来判断是否调用各内核实例，以实现对[2.基本实例介绍](#2)和[3.可操作的测试实例介绍](#3)中某个或多个实例的执行。

**实例：**<a href="./api/los_demo_entry.c" target="_blank">los_demo_entry.c</a>/<a href="./include/los_demo_entry.h" target="_blank">los_demo_entry.h</a>

执行make menuconfig打开需要测试功能的宏开关，例如要测试task实例，则选择“DemoEntry”， 并选择实例：

`Demos --> Kernel Demo --> Enable Kernel Demo --> Kernel Demo Entry --> DemoEntry --> Run Kernel Task Demo`

### 编译运行

用户可以按照<a href="https://liteos.gitee.io/liteos_studio/#/project_stm32" target="_blank">LiteOS Studio STM32工程示例</a>对代码进行编译烧写测试。

用户也可以按照沙盒实验指南来修改[3.可操作的测试实例介绍](#3)中代码。

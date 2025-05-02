# Kernel Architecture<a name="EN-US_TOPIC_0305675048"></a>

**Figure  1**  Huawei LiteOS Kernel Architecture<a name="fig18479122161618"></a>  


![](figures/huawei_liteos_kernel_architecture.png)

The basic kernel of Huawei LiteOS includes an untailorable ultra-small kernel and some tailorable modules. The ultra-small kernel covers task management, memory management, interrupt management, error handling, and system clock. Tailorable modules include semaphore, mutex lock, queue management, event management, and software timer. Huawei LiteOS supports the uni-processor \(UP\) and symmetric multiprocessor \(SMP\) modes. That is, Huawei LiteOS can run environments with one or multiple processors.


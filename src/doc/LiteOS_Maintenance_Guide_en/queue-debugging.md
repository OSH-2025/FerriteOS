# Queue Debugging<a name="EN-US_TOPIC_0312244442"></a>

## Function Description<a name="en-us_topic_0298364511_section856011974016"></a>

A queue is a producer-consumer model in which a producer produces messages and puts them into a queue for consumers to use. If the queue is full, the producer is suspended. If the queue is empty, the consumer is suspended. When queues are used to transmit messages in Huawei LiteOS, you can set the timeout interval. Queues are used to implement asynchronous communication between tasks. You can run the shell command  [queue](/shell/doc_en/queue.md)  to view the queue usage.

## How to Use<a name="en-us_topic_0298364511_section53021951114019"></a>

The  **queue**  command depends on  **LOSCFG\_DEBUG\_QUEUE**. When using the  **queue**  command, you need to enable  **Enable Queue Debugging**  in  **menuconfig**.

```
Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Queue Debugging
```

## Example<a name="en-us_topic_0298364511_section162386014464"></a>

In the shell window, run the  **queue**  command to display the following queue information:

![](figures/run_queue.png)

For the description of the output items, see  [queue-output](/shell/doc_en/queue.md#output). During the debugging, the identification item  **TaskEntry of creator**  in the preceding figure is used.  **TaskEntry of creator**  is the address \(0x0x80242df8\) of the interface function for creating a queue. Find the address in the .asm disassembly file. By default, the disassembly file is in the  **Huawei\_LiteOS/out/**_Platform name_  directory. You can view the name of the function for creating a queue, for example, app\_init \(0x0x80242df8\) is shown in the following figure.

![](figures/find_addr_for_queue_debug.png)


# Task Status Check<a name="EN-US_TOPIC_0312244450"></a>

After Huawei LiteOS runs normally, run the  [**task**](/shell/doc_en/task.md)  command to view status of all tasks. Values of  **stackSize**,  **WaterLine**,  **StackPoint**, and  **Top0fStack**  can be used to determine whether a task stack causes illegal memory access.

The following use a task named  **shellTask**  as an example describes how to use the  **task**  command to determine whether memory is overwritten:

![](figures/check_memory_corruption_by_task.png)

StackSize = 0x3000 \(size of the stack allocated to the task when the task is created\)

WaterLine = 0x2810 \(size of the used memory of the stack\)

StackPoint = 0x80d10084  \(stack pointer that points to the address of the task\)

Top0fStack = 0x80d0d768 \(top of the stack\)

MaxStackPoint = Top0fStack + StackSize = 0x80d10768 \(maximum range of accessible stack\)

-   If WaterLine \> StackSize, the task causes memory overwriting.
-   If StackPoint \> MaxStackPoint or StackPoint < Top0fStack, the task stack causes memory overwriting.


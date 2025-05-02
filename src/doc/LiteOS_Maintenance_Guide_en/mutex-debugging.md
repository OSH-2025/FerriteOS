# Mutex Debugging<a name="EN-US_TOPIC_0312244443"></a>

## Usage Scenarios<a name="en-us_topic_0175230292_section7808104614227"></a>

The multi-task system uses mutexes to achieve resource exclusion so that other tasks cannot forcibly preempt the resources that have been occupied by a task. When a mutex is used, tasks may wait for each other to release resources, therefore causing a deadlock. Deadlocks cause tasks to be in infinite loop waiting, which affects service functions.

## Function Description<a name="en-us_topic_0175230292_section13598163943816"></a>

After the function  [dlock](/shell/doc_en/dlock.md)  of detecting the mutex is enabled, after a task acquires a mutex, it is the owner of the mutex. Therefore, the task ID can be used to obtain information about the mutex. In addition, the mutex control block itself records the tasks that are blocked because the tasks fail to acquire the mutex. The  **dlock**  command displays information about the mutex and task call stacks of all tasks in the system. You can determine which tasks encounter deadlocks based on the .asm disassembly file and code.

## Mutex Deadlock Debugging Mechanism<a name="en-us_topic_0175230292_section119655371167"></a>

If a task cannot be scheduled after a deadlock occurs. Record the last scheduling time of the task and set a timeout threshold. If the task cannot be scheduled during this period, it is suspected that a deadlock occurs in the task.

## How to Use<a name="en-us_topic_0175230292_section104865185441"></a>

Configure the  **LOSCFG\_DEBUG\_DEADLOCK** macro. You can run the  **make menuconfig**  command to enable  **Enable Mutex Deadlock Debugging**. If you disable this item, disable the deadlock detection function.

```
Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Mutex Deadlock Debugging
```

## Precautions<a name="en-us_topic_0175230292_section88011518542"></a>

The output of deadlock debugging is the task information that exceeds the time threshold \(10 minutes by default\). However, it does not indicate that deadlock occurs on all these tasks. You need to further confirm the deadlock based on the mutex information and task call stack information.

## Example for Locating Deadlocks<a name="en-us_topic_0175230292_section48030327141646"></a>

Construct the ABBA mutex deadlock scenario as follows:

The system has two tasks  **app\_Task**  and  **mutexDlock\_Task**. In addition, there are other default initial tasks in the system. Execute the MutexDlockDebug function in the  **app\_Task**  task and create the  **mutexDlock\_Task**  task in the function. The MutexDlockDebug function \(the  **app\_Task**  task\) creates and owns mutex0, and then creates a task  **mutexDlock\_Task**  with a higher priority. After a period of sleep, the  **app\_Task**  task applies for mutex1 but the application is blocked because the  **mutexDlock\_Task**  task already owns mutex1. The  **mutexDlock\_Task**  task creates and owns mutex1, and then applies for mutex0. However, the application is blocked because the  **app\_Task**  task already owns mutex0. The codes are as follows:

```c
#include "unistd.h"
#include "los_mux.h"
#include "los_task.h"

static UINT32 mutexTest[2];
extern UINT32 OsShellCmdMuxDeadlockCheck(UINT32 argc, const CHAR **argv);

VOID DlockDebugTask(VOID)
{
    UINT32 ret;

    ret = LOS_MuxPend(mutexTest[1], LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("pend mutex1 error %u\n", ret);
    }

    ret = LOS_MuxPend(mutexTest[0], LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("pend mutex0 error %u\n", ret);
    }

    ret = LOS_MuxPost(mutexTest[1]);
    if (ret != LOS_OK) {
        PRINT_ERR("post mutex1 error %u\n", ret);
    }

    ret = LOS_MuxPost(mutexTest[0]);
    if (ret != LOS_OK) {
        PRINT_ERR("post mutex0 error %u\n", ret);
    }
}

// The MutexDlockDebug function is scheduled in the user task app_Task.
STATIC UINT32 MutexDlockDebug(VOID)
{
    UINT32 ret;
    UINT32 taskId;
    TSK_INIT_PARAM_S debugTask;

    ret = LOS_MuxCreate(&mutexTest[0]);
    if (ret != LOS_OK) {
        PRINT_ERR("create mutex0 error %u\n", ret);
    }

    ret = LOS_MuxCreate(&mutexTest[1]);
    if (ret != LOS_OK) {
        PRINT_ERR("create mutex1 error %u\n", ret);
    }

    ret = LOS_MuxPend(mutexTest[0], LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("pend mutex0 error %u\n", ret);
    }

    (VOID)memset_s(&debugTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    debugTask.pfnTaskEntry = (TSK_ENTRY_FUNC)DlockDebugTask;
    debugTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    debugTask.pcName = "mutexDlock_Task";
    debugTask.usTaskPrio = 9;
    debugTask.uwResved = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&taskId, &debugTask);   // Create the mutexDlock_Task task. The task entry function is DlockDebugTask and the task priority is 9 which is higher than that of the app_Task task.
    if (ret != LOS_OK) {
        PRINT_ERR("create debugTask error %u\n", ret);
    }

    sleep(2);
    ret = LOS_MuxPend(mutexTest[1], LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("pend mutex1 error %u\n", ret);
    }

    ret = LOS_MuxPost(mutexTest[0]);
    if (ret != LOS_OK) {
        PRINT_ERR("post mutex0 error %u\n", ret);
    }

    ret = LOS_MuxPost(mutexTest[1]);
    if (ret != LOS_OK) {
        PRINT_ERR("post mutex1 error %u\n", ret);
    }
    return ret;
}
```

If you suspect that a deadlock occurs in the preceding scenario, perform the following steps to locate the issue:

1.  Run the  **dlock**  command in the shell window to detect deadlocks.

    The output is shown in the following figure.

    ![](figures/run_dlock.png)

    -   If mutex information is displayed below "Task\_name:app\_Task, ID:0x5, holds the Mutexs below:" and "Task\_name:mutexDlock\_Task, ID:0xc, holds the Mutexs below:", deadlocks may occur in tasks  **app\_Task**  \(task ID: 5\) and  **mutexDlock\_Task**  \(task ID: c\).
    -   Information below  **<Mutex0 info\>**  shows the details about the mutex, including  **Ptr handle**  \(mutex handle\),  **Owner**  \(mutex owner\),  **Count**  \(reference count of the mutex\), and  **Pended task**  \(tasks blocked by the mutex\). If the task owns multiple mutexes, information about the mutexes \(Mutex0 to MutexN\) is printed one by one. Currently, the  **app\_Task**  and  **mutexDlock\_Task**  tasks each have only one mutex.

2.  Run the  **task**  command in the shell window to display the status and information of all running tasks.

    The output is shown in the following figure.

    ![](figures/run_task.png)

    According to step 1,  **app\_Task**  and  **mutexDlock\_Task**  are the tasks where deadlocks may occur. In the preceding figure, the  **TaskEntryAddr**  column lists the task entry function address of the mutex pend when a deadlock occurs. In this example, the addresses are 0x8026f28c \(for app\_Task\) and 0x8026eef4 \(for  **mutexDlock\_Task**\).

3.  Find the corresponding function in the disassembly file.

    Open the .asm disassembly file. By default, the file is stored in the  **Huawei\_LiteOS/out/**_Platform name_  directory. Find the corresponding addresses in the .asm file. The following uses 0x8026eef4 \(for  **mutexDlock\_Task**\) as an example to show the location of the mutex pend and the called interface.

    ![](figures/find_addr_for_mutex_deadlock.png)

4.  Displays stack call information about a single task.

    To further confirm the call relationship of a task, run the  **task**  command with the task ID in the shell window to view the stack call information of the task, and then determine whether a deadlock occurs based on the context.



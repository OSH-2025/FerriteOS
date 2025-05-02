# Creating Tasks<a name="EN-US_TOPIC_0314628536"></a>

## Task Description<a name="section4752185813211"></a>

LiteOS supports multiple tasks. On LiteOS, a task indicates a thread. Tasks can use or wait for system resources such as CPU and memory, and run independently. LiteOS implements task switching and communication, helping developers manage service program processes. Developers can devote more energy to the implementation of service functions.

In LiteOS, the LOS\_TaskCreate\(\) function is used to create a task. The LOS\_TaskCreate\(\) function prototype is defined in the  **kernel\\base\\los\_task.c**  file. After a task is created using LOS\_TaskCreate\(\), the task is ready.

## Task Creation Process<a name="section844516171537"></a>

The following uses a cyclic indicator blinking as an example to describe how to create LiteOS tasks.

Create tasks in the  **targets\\Developer board name\\Src\\main.c**  file of the ported development board project as follows:

1.  Write a task function to create two LED indicator tasks with different blinking frequencies.

    ```
    UINT32 LED1_init(VOID)
    {
        while(1) {
            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9); // Correspond to the LED indicator pin configured in Creating a Bare Metal Project.
            LOS_TaskDelay(500000);
        }
        return 0;
    }
    
    UINT32 LED2_init(VOID)
    {
        while(1) {
            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10); // Correspond to the LED indicator pin configured in Creating a Bare Metal Project.
            LOS_TaskDelay(1000000);
        }
        return 0;
    }
    ```

2.  Set the parameters of the two tasks and create tasks.

    ```
    STATIC UINT32 LED1TaskCreate(VOID)
    {
        UINT32 taskId;
        TSK_INIT_PARAM_S LEDTask;
    
        (VOID)memset_s(&LEDTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
        LEDTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LED1_init;
        LEDTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
        LEDTask.pcName = "LED1_Task";
        LEDTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
        LEDTask.uwResved = LOS_TASK_STATUS_DETACHED;
        return LOS_TaskCreate(&taskId, &LEDTask);
    }
    
    STATIC UINT32 LED2TaskCreate(VOID)
    {
        UINT32 taskId;
        TSK_INIT_PARAM_S LEDTask;
    
        (VOID)memset_s(&LEDTask, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
        LEDTask.pfnTaskEntry = (TSK_ENTRY_FUNC)LED2_init;
        LEDTask.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
        LEDTask.pcName = "LED2_Task";
        LEDTask.usTaskPrio = LOSCFG_BASE_CORE_TSK_DEFAULT_PRIO;
        LEDTask.uwResved = LOS_TASK_STATUS_DETACHED;
        return LOS_TaskCreate(&taskId, &LEDTask);
    }
    ```

3.  Add the LED initialization and the corresponding header file to the hardware initialization function HardwareInit\(\).

    ```
    #include "gpio.h"
    MX_GPIO_Init();
    ```

4.  For the ported STM32F407\_OpenEdv project, the task processing function app\_init is defined in the  **targets\\STM32F407\_OpenEdv\\Src\\user\_task.c**  file, which contains the tasks related to the network and file system. Currently, these tasks do not need to be executed, you can delete this file from the  _USER\_SRC_  variable in  **targets\\STM32F407\_OpenEdv\\Makefile**. You can refer to this file when performing related tasks.
5.  Implement the task processing function app\_init\(\) before the main\(\) function in the  **main.c**  file, and add the calling of the LED task creation function.

    ```
    UINT32 app_init(VOID)
    {
        LED1TaskCreate();
        LED2TaskCreate();
    
        return 0;
    }
    ```


## Code Example<a name="section1095419341137"></a>

[main.c](resource/main.c)

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>This code example only creates a basic task. Developers can create their own tasks based on the actual requirements.


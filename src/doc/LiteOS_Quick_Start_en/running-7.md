# Running<a name="EN-US_TOPIC_0308937192"></a>

You can run the following command to start the guest VM using QEMU to run Huawei LiteOS. SMP \(multi-core\) is enabled by default in the realview-pbx-a9 project, and you need to set the  **-smp**  parameter when starting the VM.

```
$ qemu-system-arm -machine realview-pbx-a9 -smp 4 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>The parameters in the preceding command are described as follows. For more information, run the  **qemu-system-arm --help**  command.
>-   **-machine**: sets the type of the VM to be emulated by QEMU.
>-   **-smp**: sets the number of CPUs of the guest VM.
>-   **-m**: specifies the memory reserved for the guest VM. If this parameter is not specified, the default value  **128M**  is used.
>-   **-kernel**: specifies the image file \(including the file path\) to be run.
>-   **-nographic**: starts a VM in non-GUI mode.

After the VM is started, Huawei LiteOS is started and the shell CLI window is displayed \(the  **Huawei LiteOS \#**  prompt is displayed\). The following information is displayed:

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0      
Processor  : Cortex-A9 * 4
Run Mode   : SMP
GIC Rev    : GICv1
build time : Jan  5 2021 16:30:06  

********************************** 

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 2 entering scheduler
cpu 3 entering scheduler
cpu 1 entering scheduler
app init!

Kernel task demo begin.
LOS_TaskLock() ok.

Create Example_TaskHi ok.
Create Example_TaskLo ok.
Enter TaskLo Handler.
Enter TaskHi Handler.
TaskHi LOS_TaskDelay Done.
TaskHi LOS_TaskSuspend ok.
TaskHi LOS_TaskResume ok.
Kernel task demo ok.

Huawei LiteOS #
```

For more information about the shell function, see  [Shell User Guide](/shell/doc_en/README_EN.md).


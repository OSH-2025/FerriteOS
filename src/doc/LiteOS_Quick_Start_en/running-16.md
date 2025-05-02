# Running<a name="EN-US_TOPIC_0317470053"></a>

You can run the following command to start the guest VM using QEMU to run Huawei LiteOS. SMP \(multi-core\) is enabled by default in the qemu-virt-a53 project, so you need to set the  **-smp**  parameter when starting the VM.

```
$ qemu-system-aarch64 -machine virt -smp 4 -m 512M -cpu cortex-a53 -kernel out/qemu-virt-a53/Huawei_LiteOS.elf -nographic
```

>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>The parameters in the preceding command are described as follows. For more information, run the  **qemu-system-arm --help**  command.
>-   **-machine**: sets the type of the VM to be emulated by QEMU.
>-   **-smp**: sets the number of CPUs of the guest VM.
>-   **-m**: specifies the memory reserved for the guest VM. If this parameter is not specified, the default value  **128M**  is used.
>-   **-cpu**: sets the chip architecture of the VM to be emulated.
>-   **-kernel**: specifies the image file \(including the file path\) to be run. Note that the file format is ELF.
>-   **-nographic**: starts a VM in non-GUI mode.

After the VM is started, Huawei LiteOS is started and the shell CLI window is displayed \(the  **Huawei LiteOS \#**  prompt is displayed\). The following information is displayed:

```
********Hello Huawei LiteOS********

Processor   : Cortex-A53 * 4
Run Mode    : SMP
GIC Rev     : unknown
build time  : Dec  5 2020 02:50:54

**********************************

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
app_init
cpu 1 entering scheduler
cpu 2 entering scheduler
cpu 3 entering scheduler

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


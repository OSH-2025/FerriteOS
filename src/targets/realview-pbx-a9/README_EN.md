# Cortex-A9 Running LiteOS in QEMU

## Installing QEMU

- QEMU is an open-source tool. For details about QEMU, visit <a href="<https://github.com/qemu/qemu>" target="_blank">https://github.com/qemu/qemu</a>.

- Ubuntu Linux is used in this example. If QEMU is not installed, run the following commands to download and install it:

    ```
    apt-get install qemu
    apt-get install qemu-system
    ```

## Building PBX A9 in Symmetric Multi-Processors (SMP) Mode on Linux

- Change the `.config` file required for building to the default configuration file of the PBX A9.

    ```
    cp tools/build/config/realview-pbx-a9.config .config
    ```

- Use `make` for building. Kernel SMP is enabled by default.

    ```
    make clean; make -j
    ```

- Use QEMU to start the guest VM and run LiteOS.

    ```
    qemu-system-arm -machine realview-pbx-a9 -smp 4 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
    ```

    You can run the qemu-system-arm --help command to view the description of QEMU command parameters. The parameters in the preceding command are described as follows:
    | Parameter | Description |
    |:---:|---|
    |-machine| Specifies the type of a VM to be emulated by QEMU. |
    |-smp| Specifies the number of CPUs of the guest VM. |
    |-m| Specifies the memory reserved for the guest VM. If this parameter is not specified, the default value 128M is used. |
    |-kernel| Specifies the path of an image file to run. |
    |-nographic| Specifies that the non-graphical interface is started. |

- After Kernel SMP is enabled, the following information is displayed when LiteOS is started:

    ```
    ********Hello Huawei LiteOS********

    Processor   : Cortex-A9 * 4
    Run Mode    : SMP
    GIC Rev     : unknown
    build time  : Dec  1 2020 04:17:44

    **********************************

    main core booting up...
    osAppInit
    releasing 3 secondary cores
    cpu 0 entering scheduler
    cpu 1 entering scheduler
    cpu 3 entering scheduler
    cpu 2 entering scheduler
    app_init

    Huawei LiteOS #
    ```

## Building PBX A9 in Uni-Processor (UP) Mode on Linux

- Kernel SMP is enabled by default. To disable SMP, you can disable `LOSCFG_KERNEL_SMP` in `make menuconfig`. The configuration is as follows:

    ```
    Kernel --> Enable Kernel SMP
    ```

- After the build, run the following command to run QEMU:

    ```
    qemu-system-arm -machine realview-pbx-a9 -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
    ```

- After Kernel SMP is disabled, the following information is displayed when LiteOS is started:

    ```
    ********Hello Huawei LiteOS********

    Processor   : Cortex-A9
    Run Mode    : UP
    GIC Rev     : unknown
    build time  : Dec  1 2020 04:26:08

    **********************************

    main core booting up...
    osAppInit
    cpu 0 entering scheduler
    app_init

    Huawei LiteOS #
    ```
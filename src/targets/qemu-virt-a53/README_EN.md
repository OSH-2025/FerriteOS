# Cortex-A53 Running LiteOS in QEMU

## Installing QEMU

- QEMU is an open-source tool. For details about QEMU, visit <a href="<https://github.com/qemu/qemu>" target="_blank">https://github.com/qemu/qemu</a>.

- Ubuntu Linux is used in this example. If QEMU is not installed, run the following commands to download and install it:

    ```
    apt-get install qemu
    apt-get install qemu-system
    ```

    Install the package related to 64-bit QEMU for the 64-bit Cortex-A53.

    ```
    apt-get install qemu-system-aarch64
    ```

## Building VIRT A53 in Symmetric Multi-Processors (SMP) Mode on Linux

### Deploying a Build Environment

Install a 64-bit compiler to build VIRT A53. The Ubuntu environment is used in this example to describe how to install the 64-bit ARM cross compiler `aarch64-none-elf-gcc`.

```
$ wget https://developer.arm.com/-/media/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf.tar.xz
$ tar -xf gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf.tar.xz
$ export PATH=$PATH:{$USERPATH}/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin
```

### Building and Running

- Change the `.config` file required for building to the default configuration file of VIRT A53.

    ```
    cp tools/build/config/qemu-virt-a53.config .config
    ```

- Use `make` for building. Kernel SMP is enabled by default.

    ```
    make clean; make -j
    ```

- Use QEMU to start the guest VM and run LiteOS.

    ```
    qemu-system-aarch64 -machine virt -smp 4 -m 512M -cpu cortex-a53 -kernel out/qemu-virt-a53/Huawei_LiteOS.elf -nographic
    ```

    You can run the qemu-system-arm --help command to view the description of QEMU command parameters. The parameters in the preceding command are described as follows:
    | Parameter | Description |
    |:---:|---|
    |-machine| Specifies the type of a VM to be emulated by QEMU. |
    |-smp| Specifies the number of CPUs of the guest VM. |
    |-m| Specifies the memory reserved for the guest VM. If this parameter is not specified, the default value 128M is used. |
    |-cpu| Specifies the chip architecture of the VM to emulate. |
    |-kernel| Specifies the path of an image file to run. |
    |-nographic| Specifies that the non-graphical interface is started. |

- After Kernel SMP is enabled, the following information is displayed when LiteOS is started:

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

    Huawei LiteOS #
    ```

## Building VIRT A53 in Uni-Processor (UP) Mode on Linux

- Kernel SMP is enabled by default. To disable SMP, you can disable `LOSCFG_KERNEL_SMP` in `make menuconfig`. The configuration is as follows:

    ```
    Kernel --> Enable Kernel SMP
    ```

- After the build, run the following command to run QEMU:

    ```
    qemu-system-aarch64 -machine virt -cpu cortex-a53 -kernel out/qemu-virt-a53/Huawei_LiteOS.elf -nographic
    ```

- After Kernel SMP is disabled, the following information is displayed when LiteOS is started:

    ```
    ********Hello Huawei LiteOS********

    Processor   : Cortex-A53
    Run Mode    : UP
    GIC Rev     : unknown
    build time  : Dec  1 2020 04:26:08

    **********************************

    main core booting up...
    osAppInit
    cpu 0 entering scheduler

    Huawei LiteOS #
    ```
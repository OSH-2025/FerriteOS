# Setting Up the Development Environment<a name="EN-US_TOPIC_0317470051"></a>

The development environment of the qemu-virt-a53 project on Linux includes the build environment and QEMU.

-   Set up the build environment.

    For details, see  [Setting up the Linux Build Environment](../LiteOS_Build_and_IDE_en/setting-up-the-linux-build-environment.md). Note that the Cortex-A53 processor uses the AArch64 architecture. Therefore, the GNU Arm Embedded Toolchain compiler must be aarch64-linux-gun.

-   Install a QEMU.

    You can run the following command to install QEMU:

    ```
    $ apt-get install qemu
    $ apt-get install qemu-system
    $ apt-get install qemu-system-aarch64
    ```

    In addition, you can install QEMU by  [building the QEMU source code package](https://www.qemu.org/download/#source).



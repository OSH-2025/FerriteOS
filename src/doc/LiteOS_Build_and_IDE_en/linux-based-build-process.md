# Linux-based Build Process<a name="EN-US_TOPIC_0302049456"></a>

After the environment is set up by referring to  [Setting up the Linux Build Environment](setting-up-the-linux-build-environment.md), perform the following steps to complete the build:

1.  Download the Huawei LiteOS code.

    Download the complete Huawei LiteOS code of master branch from  [LiteOS code repository](https://gitee.com/LiteOS/LiteOS)  on Gitee.

2.  Copy the .config file of the development board to the root directory.

    Copy the default configuration file  _$\{platform\}_**.config**  in  **tools/build/config/**  to the root directory and rename it  **.config**.

3.  \(Optional\) Configure the system.

    If you do not want to use the default system configuration, run the  **make menuconfig**  command in the root directory of Huawei\_LiteOS to tailor modules or modify configuration items on the GUI. Save the modification and exit. The modification is saved to the .config file in the root directory by default.

4.  Clear the project.

    Before build, run the  **make clean**  command in the root directory of Huawei\_LiteOS to delete binary files that have been built before.

5.  Build the project.

    Run the  **make**  command in the root directory of Huawei\_LiteOS to build the project. The build result is displayed on the screen.

    Take Cloud\_STM32F429IGTx\_FIRE as an example. The generated system image files and disassembly files are stored in the  **out/Cloud\_STM32F429IGTx\_FIRE**  directory, the library files are stored in the  **out/Cloud\_STM32F429IGTx\_FIRE/lib**  directory, and the intermediate files are stored in the  **out/Cloud\_STM32F429IGTx\_FIRE/obj**  directory.


>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>If you build a module instead of an entire project code, after the module is configured, perform the following steps to complete the build:
>1. Export the  _LITEOSTOPDIR_  environment variable.
>
>    _LITEOSTOPDIR_  indicates the path for storing Huawei LiteOS code. For example, if the code path is  **/usr1/mypc/Huawei\_LiteOS**, run the following command:
     >    ```
     >    export LITEOSTOPDIR=/usr1/mypc/Huawei_LiteOS
     >    ```
>2. Build the module.
>
>    Go to the directory of the module to be built and run the  **make**  command.


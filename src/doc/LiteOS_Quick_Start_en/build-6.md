# Build<a name="EN-US_TOPIC_0308937190"></a>

1.  Download the Huawei LiteOS code.

    Download the complete Huawei LiteOS code of master branch from  [LiteOS code repository](https://gitee.com/LiteOS/LiteOS)  on Gitee.

2.  Copy the emulator project configuration file as the .config file in the root directory.

    Run the following command in the root directory of Huawei\_LiteOS to copy the .config file of the realview-pbx-a9 emulator project:

    ```
    $ cp tools/build/config/realview-pbx-a9.config .config
    ```

3.  Configure the demo to be executed.

    Run the  **make menuconfig**  command in the Huawei\_LiteOS root directory to open the menuconfig GUI and enable the demo to be executed. The following uses Kernel Task Demo as an example to describe how to configure a demo. For details about how to enable other demos, see README in each subdirectory of the  **demos**  directory in the root directory of the Huawei LiteOS source code.

    Choose  **Demos**  \>  **Kernel Demo**. Press the space bar to select  **Enable Kernel Demo**  \(**\[ \* \]**  is displayed after this configuration item is enabled\). Then, choose  **Kernel Demo Entry**  and select  **DemoEntry**. \(This configuration item can be used to execute one or multiple kernel demos. Another configuration item  **InspectEntry**  indicates that all kernel demos are executed.\) Choose  **Run Kernel Task Demo**  from the submenu, as shown in the following figure.

    ![](figures/select_task_demo_in_menuconfig.png)

    After the configuration is complete, enter  **S**  to save configuration items to the .config file in the root directory by default. Press  **Enter**. Enter  **Q**  to exit menuconfig.

4.  Clear the project.

    Before build, run the  **make clean**  command in the root directory of Huawei\_LiteOS to delete binary files that have been built before.

5.  Build the project.

    Run the  **make**  command in the root directory of Huawei\_LiteOS to build the project. The build result is displayed on the screen. The generated system image files and disassembly files are stored in the  **out/realview-pbx-a9**  directory, and the library files are stored in the  **out/realview-pbx-a9/lib**  directory, the intermediate file is stored in the  **out/realview-pbx-a9/obj**  directory.  **Kernel Task Demo**  is enabled in step 3, and the  **libkernel\_demo.a**  library file exists in the  **lib**  directory where the library file is saved.



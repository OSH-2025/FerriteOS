# Using LiteOS Studio to Test a Bare Metal Project<a name="EN-US_TOPIC_0314628522"></a>

1.  Configure the target board.

    On the  **Project Config**  page, click  **Board**. In the  **Operation**  column, click  **+**. Enter the STM32F407 development board information, select the new development board, and click  **Confirm**, as shown in the following figure.

    ![](figures/target_board_configuration.png)

2.  Build the project.

    Right-click  **Makefile**  in the root directory of the bare metal project, choose  **Set as Makefile file**  from the shortcut menu, and build the project. The generated binary image file is stored in the  **build**  directory in the root directory of the project, as shown in the following figure.

    **Figure  1**  Building a bare metal project<a name="fig11370205501817"></a>  
    ![](figures/build_bare_project.png "build_bare_project")

3.  Burn the image.
    1.  Configure the burner.

        On the  **Project Config**  tab page, click  **Burner**, as shown in the following figure. The binary image file to be burnt is the .bin file generated in the previous step. Retain the default values of  **Speed\(KHz\)**  and  **Address**.

        ![](figures/burner_configuration.png)

    2.  Click the burn button on the toolbar to start burning.

        ![](figures/burn_button.png)

        After the burning is successful, the following information is displayed on  **TERMINAL**:

        ![](figures/output_of_successful_burning.png)

    3.  Check the serial port output.

        Click the  ![](figures/serial_terminal_button.png)  icon on the toolbar to go to the  **SERIALPORT TERMINAL**  page. As shown in the following figure, you only need to set the actual port number for connecting to the development board and enable the serial port. After pressing the  **RESET**  button on the development board, you can see that  **hello**  is continuously output on the  **SERIALPORT TERMINAL**  and the LED indicator of the development board blinks.

        ![](figures/serial_output_of_bare_project.png)



>![](public_sys-resources/icon-note.gif) **NOTE:** 
>
>For details about how to use LiteOS Studio, see  [STM32 Project Example](https://liteos.gitee.io/liteos_studio/#/project_stm32)  in the LiteOS Studio official document.

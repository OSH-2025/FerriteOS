# Verifying LiteOS Studio IDE<a name="EN-US_TOPIC_0314628509"></a>

Before porting, you can verify whether the LiteOS code can be successfully built and burnt in the current development environment. Currently, the  [open-source LiteOS](https://gitee.com/LiteOS/LiteOS)  supports several development boards, such as Cloud\_STM32F429IGTx\_FIRE, STM32F769IDISCOVERY and STM32L431\_BearPi. You can verify the environment based on the actual requirements.

-   If no officially adapted development board is available, you can use the development board project supported by LiteOS to verify the build function. The burning function will be verified in  [Testing Bare Metal Project](testing-bare-metal-project.md).
-   If the officially adapted development board is available, use the corresponding development board project to verify the build and burning functions. That is:
    -   For the Cloud\_STM32F429IGTx\_FIRE development board, select  **STM32F429IG**  when configuring the target board information on LiteOS Studio.
    -   For the STM32F769IDISCOVERY development board, select  **STM32F769NI**  when configuring the target board information on LiteOS Studio.
    -   For the STM32L431\_BearPi development board, select  **STM32L431RC**  when configuring the target board information on LiteOS Studio.


For details about the verification method, see  [Getting Started](https://liteos.gitee.io/liteos_studio/#/project_stm32?id=使用入门)  in "STM32 Project Example" of the LiteOS Studio official document. \(You only need to focus on "Opening a Project", "Configuring a Target Board", "Compiler Configuration - Building Code", and "Burner Configuration - Burning."\)


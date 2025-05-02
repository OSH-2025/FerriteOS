# Constraints<a name="EN-US_TOPIC_0302339209"></a>

-   In addition to its own APIs, Huawei LiteOS supports POSIX and CMSIS APIs, but hybrid use of them may lead to unpredictable results. \(For example, a POSIX API is used for requesting semaphores while a Huawei LiteOS API is used for releasing semaphores.\)
-   Use only Huawei LiteOS APIs for driver development. POSIX APIs are recommended for app development.


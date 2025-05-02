
## LiteOS Code Entry

The LiteOS entry is in the main.c file of a project. The basic process is as follows:

```c
INT32 main(VOID)
{
    BoardConfig();
    HardwareInit();

    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n"
                  "\nLiteOS Kernel Version : %s\n"
                  "build data : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING, __DATE__, __TIME__);

    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    OsStart();

    return 0;
}
```

Configure the memory end address using BoardConfig(), initialize the hardware using HardwareInit(), and print the version information of Huawei LiteOS.

Execute OsMain() to initialize the Huawei LiteOS kernel and routines. A user task is created in the OsMain() function, and the task processing function is app_init().

Call OsStart() to start task scheduling. Huawei LiteOS starts to work properly.

## Description of the LiteOS Code Directory Structure

Source code stored in each directory is as follows:

| Level-1 Directory   | Level-2 Directory                 | Level-3 Directory              | Description                                                         |
| ---------- | ------------------------ | --------------------- | ------------------------------------------------------------ |
| arch       | arm                      | cortex_a_r            | Cortex-A architecture support                                                  |
|            |                          | cortex_m              | Cortex-M architecture support                                                  |
|            | arm64                    |                       | ARM64 architecture support                                                |
| build      |                          |                       | Configuration and scripts required for building LiteOS                               |
| compat     | cmsis                    |                       | CMSIS-RTOS 1.0 and 2.0 APIs provided by LiteOS                          |
| components | ai                       |                       | MindSpore-based AI operator library implementation                                  |
|            | connectivity             | agent_tiny            | agent_tiny, which is a device-cloud interconnect componentcontaining the public header file, sample code, client implementation code, and OS adaptation layer code |
|            |                          | lwm2m                 | Implementation of the LwM2M protocol                                                |
|            |                          | mqtt                  | Implementation of the MQTT open-source protocol                                             |
|            |                          | nb_iot                | LiteOS NB-IoT API                                            |
|            | fs                       |                       | File systems, including vfs, spiffs, ramfs, littlefs, kifs, fatfs and devfs |
|            | gui                      |                       | Open-source LittlevGL graphics library                                          |
|            | language                 |                       | Language-related components, including lua                                                  |
|            | lib                      | cjson                 | C language JSON library                                                  |
|            | log                      |                       | Log level control                                                 |
|            | media                    |                       | Media-related components, including Opus and libpng                                         |
|            | net                      | at_device             | AT device adaptation layer                                                 |
|            |                          | at_frame              | LiteOS AT framework API                                             |
|            |                          | lwip/lwip_port        | lwIP driver and OS adaptation code                                         |
|            |                          | lwip/lwip-2.1.2       | Implementation of the lwIP protocol                                                 |
|            |                          | lwip/ppp_port         | Support for the PPP port of the lwIP protocol                                          |
|            |                          | sal                   | Socket communication support                                               |
|            | ota                      |                       | Firmware upgrade code                                                 |
|            | security                 | mbedtls/mbedtls_port  | mbed TLS OS adaptation code                                         |
|            |                          | mbedtls/mbedtl-2.16.8 | mbed TLS protocol implementation                                             |
|            | sensorhub                | include               | Sensor manager header file                                         |
|            |                          | src                   | Sensor manager source code implementation                                     |
|            | utility                  |                       | Parsing tools, including bidireference, freetype, iconv, iniparser, json-c and libxml2 |
| demos      | agenttiny_lwm2m          |                       | LwM2M protocol demo                                               |
|            | agenttiny_mqtt           |                       | MQTT protocol demo                                                |
|            | ai                       |                       | AI demo                                                     |
|            | dtls_server              |                       | DTLS protocol demo                                                 |
|            | fs                       |                       | File system demo                                                 |
|            | gui                      |                       | GUI demo                                                    |
|            | ipv6_client              |                       | IPv6 protocol demo                                                 |
|            | kernel                   | api                   | Demo code example for developers to test the LiteOS kernel                         |
|            |                          | include               | Directory of API function header files                                        |
|            | language                 |                       | Language-related component demo                                               |
|            | lms                      |                       | LMS demo                                                    |
|            | media                    |                       | Media-related component demo                                                   |
|            | nbiot_without_atiny      |                       | NB-IoT demo                                                  |
|            | sensorhub                | gyro                  | Demo for periodically reading the original data of the MPU6050 gyroscope based on the sensorhub sensor framework     |
|            | trace                    |                       | Trace demo                                                  |
|            | utility                  |                       | Parser demo                                               |
| doc        |                          |                       | LiteOS-related documents, such as the usage guide and API description                |
| include    |                          |                       | Header files which each components module depends on                             |
| kernel     | base                     |                       | LiteOS basic kernel code, providing functions such as tasks, interrupts, software timers, queues, events, semaphores, mutex locks, and ticks |
|            |                          | debug                 | LiteOS kernel debugging code, including the debugging of queues, semaphores, mutexes, and task scheduling |
|            |                          | include               | Header files internally used by the LiteOS basic kernel                               |
|            |                          | mem                   | Code related to memory management in LiteOS                                   |
|            |                          | sched                 | Task scheduling, including multi-core scheduling                           |
|            |                          | shellcmd              | Shell commands related to the basic kernel on LiteOS, including memcheck, task, systeminfo, and swtmr |
|            | extended                 | cppsupport            | Bottom-layer APIs for the C++ compatible adaptation layer                                        |
|            |                          | cpup                  | cpup | APIs for collecting statistics on CPU usage                                            |
|            |                          | include               | Header files required by the extended directory                                     |
|            |                          | lms                   | Library file of LMS (used to check the validity of memory operations in real time)                    |
|            |                          | lowpower              | Code related to the low-power framework                                           |
|            |                          | trace                 | Event tracing, which is used to record the system running track in real time                      |
|            | include                  |                       | LiteOS open-source kernel header file                                         |
|            | init                     |                       | LiteOS kernel initialization code                                     |
| lib        | huawei_libc              |                       | LiteOS Huawei-developed libc library and adapted POSIX API                            |
|            | libc                     |                       | The musl libc library for LiteOS adaptation                                      |
|            | libsec                   |                       | Huawei security function library                                               |
|            | zlib                     |                       | Open-source zlib library                                                   |
| osdepends  | liteos                   |                       | Some OS adaptation interfaces provided by LiteOS                                   |
| shell      |                          |                       | Implementing the code of the shell command and supporting the basic debugging function                        |
| targets    | bsp                      |                       | General board support package                                               |
|            | Cloud_STM32F429IGTx_FIRE |                       | Wildfire STM32F429 (ARM Cortex M4) development project source code package         |
|            | qemu-virt-a53            |                       | QEMU development project source code package of Coretex-A53                              |
|            | realview-pbx-a9          |                       | QEMU development project source code package of Coretex-A9                               |
|            | STM32F072_Nucleo         |                       | Source code package of the development project for the STM32F072_Nucleo (ARM Cortex-M0) development board      |
|            | STM32F103_FIRE_Arbitrary |                       | Development project source code package of the Wildfire STM32F103 (ARM Cortex-M3) Hegemony development board     |
|            | STM32F407_ATK_Explorer   |                       | STM32F407 (ARM Cortex-M4) explorer board development project source code package |
|            | STM32F769IDISCOVERY      |                       | Source code package of the development project for the STM32F769IDISCOVERY (ARM Cortex-M7) development board   |
|            | STM32L4R9I_Discovery     |                       | Source code package of the development project for the STM32L4R9IDISCOVERY (ARM Cortex-M4) development board   |
|            | STM32L431_BearPi         |                       | Source code package of the development project for BearPi STM32L431 (ARM Cortex-M4) development board       |
|            | STM32L476_NB476          |                       | Source code package of the Shenzhen maker STM32L476 (ARM Cortex-M4) development board       |
|            | STM32L496_Nucleo         |                       | Source code package of the development project for the STM32L496 NUCLEO-144 (ARM Cortex-M4) development board  |
| tools      | build                    |                       | Configuration file of the development board supported by LiteOS                               |
|            | menuconfig               |                       | The menuconfig script required for building LiteOS                               |
| Makefile   |                          |                       | LiteOS Makefile                                              |
| .config    |                          |                       | Configuration file of the development board. If you do not reselect a development board, the configuration file of the wildfire challenger STM32F429 development board is used by default. |

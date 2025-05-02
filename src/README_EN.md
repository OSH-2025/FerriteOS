[![star](https://gitee.com/LiteOS/LiteOS/badge/star.svg?theme=gvp)](https://gitee.com/LiteOS/LiteOS/stargazers)
[![fork](https://gitee.com/LiteOS/LiteOS/badge/fork.svg?theme=gvp)](https://gitee.com/LiteOS/LiteOS/members)

[中文](README.md)

## Introduction to Huawei LiteOS

Huawei LiteOS is a lightweight operating system based on real-time kernel developed by Huawei for the IoT. The existing basic kernel includes an untailorable ultra-small kernel and some tailorable modules. The ultra-small kernel covers task management, memory management, interrupt management, error handling, and system clock. Tailorable modules include semaphore, mutex lock, queue management, event management, and software timer. In addition to the basic kernel functions, Huawei LiteOS also provides the enhanced kernel functions, including the C++ support, low power consumption, and maintenance and test module. The low power mechanisms such as tickless and run-stop hibernation and wakeup can be used to greatly reduce the system power consumption. The maintenance and test module can be used to obtain CPU usage, trace events, and run shell commands.

Huawei LiteOS provides device-cloud synergy capabilities and integrates the full set of IoT interconnection protocol stacks including LwM2M, CoAP, Mbed TLS, and LwIP. In addition, Huawei LiteOS provides the AgentTiny module based on LwM2M. You only need to focus on your own applications instead of LwM2M implementation details. You can directly use the AgentTiny-encapsulated APIs to implement secure and reliable connections with the cloud platforms.

Since the release of open-source community, Huawei LiteOS has enabled partners in terms of technologies, ecosystems, solutions, and commercial support in the NB-IoT market to build an open-source IoT ecosystem. Currently, Huawei has aggregated more than 50 MCUs and solution partners to jointly launch a series of open-source development kits and industry solutions, helping industry customers quickly launch IoT terminals and services. Such customers are involved in various industries, such as meter reading, parking, street lamp, environmental protection, bicycle sharing, and logistics. Moreover, Huawei LiteOS provides developers with a one-stop software platform, lowering development requirements and improving development efficiency.

For details about Huawei LiteOS, see [LiteOS Product Introduction](./doc/LiteOS_Introduction_en/README_EN.md).

## LiteOS Code Overview

[LiteOS Kernel Source Code Directory Description](./doc/LiteOS_Code_Info_en.md)

This document describes the source code of the LiteOS kernel. Through this document, you can understand the source code structure and the main() function of LiteOS.

## LiteOS Build and Development Tools

[LiteOS Build and Development Tools](./doc/LiteOS_Build_and_IDE_en/README_EN.md)

This document describes the LiteOS build framework and how to build LiteOS on Linux and Windows.

## LiteOS Quick Start

[LiteOS Quick Start](./doc/LiteOS_Quick_Start_en/README_EN.md)

This document describes how to quickly run LiteOS on the development board and QEMU emulator.

## LiteOS Porting Guide

[LiteOS Porting Guide](./doc/LiteOS_Porting_Guide_en/README_EN.md)

This document describes how to quickly port LiteOS based on the STM32 chip platform.

## LiteOS Kernel Development Guide

[LiteOS Kernel Development Guide](./doc/LiteOS_Kernel_Developer_Guide_en/README_EN.md)

This document describes the development and implementation principles of each LiteOS module. Through this document, you can understand how to use each module.

## LiteOS Maintenance and Test Guide

[LiteOS Maintenance and Test Guide](./doc/LiteOS_Maintenance_Guide_en/README_EN.md)

This document describes the maintenance and test capabilities of LiteOS, including the memory commissioning method, IPC communication commissioning method, event tracing, scheduling statistics, and CPU usage obtaining.

## LiteOS Shell

[Shell Tutorial](./shell/doc_en/README_EN.md)

This document describes how to customize shell commands on LiteOS and how to execute built-in shell commands.

## LiteOS Standard Library

[LiteOS Standard Library](./doc/LiteOS_Standard_Library.md)

This document lists the POSIX and CMSIS APIs supported by LiteOS.

## LiteOS Demos

The [LiteOS demos](./demos) directory contains demos and documents of each module provided by LiteOS. This document describes functions and running methods of demos.

## LiteOS API Reference

For details about <a href="https://www.huawei.com/minisite/liteos/cn/api/index.html" target="_blank">APIs</a>, visit the LiteOS official website.

## LiteOS Code Contribution Guide

[LiteOS Code & Document Contribution Guide](./doc/LiteOS_Contribute_Guide_en.md)

This document describes the LiteOS programming specifications, document writing specifications, and specifications for filling in information when committing code and documents to the LiteOS repository.

## Hardware Supported by LiteOS

* Currently, the LiteOS open-source project supports chip architectures such as ARM Cortex-M0, Cortex-M3, Cortex-M4, Cortex-M7 and Cortex-A.

* LiteOS cooperates with the industry mainstream MCUs and has adapted to multiple common MCU development suites through developer activities.

## Open-Source Protocol

* Comply with the BSD-3 open-source license agreement.

* [Huawei LiteOS IPR Policy](./doc/LiteOS_Contribute_Guide_en.md#protocols)

## Contacting Us

* Technical support

  <a href="https://gitee.com/LiteOS/LiteOS/issues" target="_blank">Commit an issue</a> that you are concerned about.
  You can also send issues to LiteOSSupport@huawei.com.

* Technical cooperation

  If you are interested in being Huawei LiteOS ecosystem partners, send an email to LiteOSSupport@huawei.com, or visit the <a href="https://www.huawei.com/minisite/liteos/en/index.html" target="_blank">Huawei LiteOS official website</a> for more details.

# Overview<a name="EN-US_TOPIC_0312244420"></a>

## Basic Concepts<a name="en-us_topic_0175230304_section58658659128"></a>

The central processing unit percent \(CPUP\) of Huawei LiteOS consists of the system CPU usage and task CPU usage.

The system CPU usage refers to the CPU usage of the system within a period of time. It indicates the busy or idle state of the system within a period of time and also indicates the CPU load.

Task CPU usage refers to the CPU usage of a single task, which indicates the busy or idle status of a single task in a period of time.

## Operational Mechanism<a name="en-us_topic_0175230304_section64191838171416"></a>

The CPUP of Huawei LiteOS uses the task-level recording mode. During task switching, the CPUP records the task start time and the time when the task is switched out or exited. Each time a task is exited, the system accumulates the task duration.

The formula for calculating the CPU usage is as follows:

System CPU usage = Total running time of all tasks except idle tasks/Total running time of the system.

CPU usage of a task = Total running time of the task/Total running time of the system.


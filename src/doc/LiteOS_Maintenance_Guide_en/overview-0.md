# Overview<a name="EN-US_TOPIC_0312244425"></a>

## Basic Concepts<a name="en-us_topic_0278792246_section7842986141442"></a>

The trace module records system behaviors in real time, which is similar to the video recording function. When an exception occurs in the system, you can use this function to view historical events to locate the fault.

## Operational Mechanism<a name="en-us_topic_0278792246_section4198546414140"></a>

The trace module of Huawei LiteOS uses the static code stub in buffer recording mode. When the stub is executed, the context of the event is obtained and written to the buffer.

The input parameters of the instrumentation function need to provide the trace event type, event operation object, and event parameters. The information is written into the buffer. In addition, the event context information such as the event occurrence time and task information in the system is recorded in the buffer.


# Introduction to Task Processing Functions<a name="EN-US_TOPIC_0314628535"></a>

The main function of LiteOS is defined in the  **main.c**  file of the development board project. This function is used to initialize the hardware and kernel and start task scheduling after the initialization is complete. In the OsMain function called by main\(\), OsAppInit\(\) is called to create a task named  **app\_Task**. The processing function of this task is app\_init\(\). You can directly add your own code to app\_init\(\). The code can be a segment of function code or a task.


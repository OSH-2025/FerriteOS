# Porting Procedure<a name="EN-US_TOPIC_0314628524"></a>

The porting operations are performed based on the existing bare metal project. The procedure is as follows:

1.  Add the directory of the new development board.
2.  Adapt to the peripheral driver and HAL library configuration file of the new development board.
3.  Configure the system clock.
4.  Adapt to the timer initialization file.
5.  Adapt to the GPIO initialization file.
6.  Adapt to the serial port initialization file.
7.  Modify the link script.
8.  Adapt to the build configuration.


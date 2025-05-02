# Usage Guidelines<a name="EN-US_TOPIC_0311018400"></a>

## Functions<a name="en-us_topic_0175230303_section185414131459"></a>

The exception management function processes chip hardware exceptions that occur during system running. The exception types vary according to chips. For details about the exception types, see the chip manual.

## Problem Locating Procedure<a name="en-us_topic_0175230303_section4760051492346"></a>

The procedure for exception management is as follows:

1.  Open the disassembling image \(ASM\) file generated after compilation.
2.  Search for the position of the PC \(pointing to the command that is being executed\) in the ASM file to find the abnormal function.
3.  <a name="en-us_topic_0175230303_li583000852037"></a>Find the parent function of the abnormal function based on the LR value.
4.  Repeat  [3](#en-us_topic_0175230303_li583000852037)  to obtain the call relationship between functions and locate the exception cause.

For details about the locating method, see the example.


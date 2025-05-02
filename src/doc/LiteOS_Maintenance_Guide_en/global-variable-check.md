# Global Variable Check<a name="EN-US_TOPIC_0312244449"></a>

During debugging, it is found that a global variable is set to  **0**  in only one place. However, the value becomes a non-zero abnormal value in the displayed information. There is a high probability that the global variable is overwritten.

If the memory of a global variable is overwritten, you can find the address of the global variable in the  **Huawei\_LiteOS/out/**_<platform\>_**/.map**  file. Pay attention to the recently used variable before the address and check whether the memory overwriting is caused by improper operations on the variable. For example, when  **memcpy**  or  **memset**  is performed on the variable, the memory overwriting occurs and current global variable is overwritten due to overflow.

Two global variables are defined in the file and are initialized.

```c
UINT32 g_uwEraseMap[16] = {0};
UINT32 g_uwEraseCount = 0;
```

You can find the locations of the global variables in the bss section in the .map file.

![](figures/find_global_var_in_bss.png)

If  **g\_uwEraseMap** is overwritten, find the address in the .map file, and then search for the variable before the address, that is,  **g\_uwEraseCount**. Analyze the usage of the  **g\_uwEraseCount** variable and check whether the **g\_uwEraseCount** variable is overwritten.


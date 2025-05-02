# Precautions<a name="EN-US_TOPIC_0312244432"></a>

-   The LMS is a commissioning feature. When the product is released, the tailoring switch of the LMS module LOSCFG\_KERNEL\_LMS needs to be disabled, the LMS compilation check option -fsanitize=kernel-address needs to be deleted, and other configuration items enabled for enabling the LMS feature need to be restored. For details, see  [Usage Process](usage-guidelines.md#en-us_topic_0287763514_section15585181011559).
-   If a large number of read and write operations are performed on the heap memory of the detected module, increase the size of the task stack.
-   The LMS compilation check option -fsanitize=kernel-address does not need to be added to the memory and LMS modules.
-   To check whether memory problems are introduced when the  **memcpy**,  **memmove**,  **strcat**,  **strcpy**,  **memcpy\_s**,  **memmove\_s**,  **strcat\_s**, and  **strcpy\_s**  functions are used, include the header file  **los\_lms.h**.
-   The LMS does not support overflow detection of the stack memory and global variables, which requires the support of the compiler.
-   The LMS supports only the bestfit memory algorithms and does not support the SLAB algorithms.


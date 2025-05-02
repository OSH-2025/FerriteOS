# Programming Example<a name="EN-US_TOPIC_0311018407"></a>

## Example Description<a name="en-us_topic_0175230635_section12652506102830"></a>

The following functions are demonstrated:

1.  Registering the hook function for error handling
2.  Executing the function for error handling

## Example Code<a name="en-us_topic_0175230635_section15395384102858"></a>

The code is as follows:

```c
#include "los_err.h"
#include "los_typedef.h"
#include <stdio.h>

void Test_ErrHandle(CHAR *fileName, UINT32 lineNo, UINT32 errorNo, UINT32 paraLen, VOID  *para)
{
    printf("err handle ok\n");
}

static UINT32 TestCase(VOID)
{
    UINT32 errNo = 0;
    UINT32 ret;
    UINT32 errLine = 16;

    LOS_RegErrHandle(Test_ErrHandle);

    ret = LOS_ErrHandle("os_unspecific_file", errLine, errNo, 0, NULL);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175230635_section12568740144737"></a>

The verification result is as follows:

```
Huawei LiteOS # err handle ok
```

## Complete Code<a name="en-us_topic_0175230635_section2429941814480"></a>

[sample\_err.c](resource/sample_err.c)


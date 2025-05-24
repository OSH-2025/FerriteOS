#include "los_typedef.h"
#include <stdarg.h>  // 确保包含变参函数支持

VOID ArchHaltCpu(VOID)
{
    __asm__ __volatile__("swi 0");
}


// 为了处理变参函数这种Rust不支持的情况
// 直接实现LOS_Panic
VOID LOS_Panic(const CHAR *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    UartVprintf(fmt, ap);
    va_end(ap);
    ArchHaltCpu();
}

// 定义g_excInfoPool结构体
struct ExcInfoPool {
    CHAR *buf;
    UINT32 offset;
    UINT32 len;
    UINTPTR dumpAddr;
} g_excInfoPool = {0};  // 定义全局变量并初始化

#ifndef PRINT_ERR
#if PRINT_LEVEL < LOS_ERR_LEVEL
#define PRINT_ERR(fmt, ...)
#else
#ifdef LOSCFG_SHELL_LK
#define PRINT_ERR(fmt, ...) LOS_LkPrint(LOS_ERR_LEVEL, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define PRINT_ERR(fmt, ...) do {           \
    (dprintf("[ERR] "), dprintf(fmt, ##__VA_ARGS__)); \
} while (0)
#endif
#endif
#endif

// WriteExcBufVa实现
VOID WriteExcBufVa(const CHAR *format, va_list arglist)
{
    INT32 ret;

    if (g_excInfoPool.len > g_excInfoPool.offset) {
        ret = vsnprintf_s((g_excInfoPool.buf + g_excInfoPool.offset), (g_excInfoPool.len - g_excInfoPool.offset),
                          (g_excInfoPool.len - g_excInfoPool.offset - 1), format, arglist);
        if (ret == -1) {
            PRINT_ERR("exc info buffer is not enough or vsnprintf_s is error.\n");
            return;
        }
        g_excInfoPool.offset += ret;
    }
}

// WriteExcInfoToBuf实现
VOID WriteExcInfoToBuf(const CHAR *format, ...)
{
    va_list arglist;

    va_start(arglist, format);
    WriteExcBufVa(format, arglist);
    va_end(arglist);
}
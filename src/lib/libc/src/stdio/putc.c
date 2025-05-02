#include <stdio.h>
#include "putc.h"

int putc(int c, FILE *f)
{
#ifdef LOSCFG_KERNEL_CPPSUPPORT
    uint32_t p = (uintptr_t)f;
    extern char _sstack, _estack;
    static const int sstack = (const int)&_sstack;
    static const int estack = (const int)&_estack;
    if (p < sstack || p > estack) {
        f = stdout;
    }
#endif
    return do_putc(c, f);
}

weak_alias(putc, _IO_putc);

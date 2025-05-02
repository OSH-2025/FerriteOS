#include "los_task_pri.h"
#include "arch/canary.h"
#include "los_swtmr_pri.h"
#include "los_config.h"
#include "los_printf.h"
#include "los_atomic.h"
#include "gic_common.h"
#include "uart.h"

Atomic ncpu = 1;
extern void LOS_HelloRust(void);

LITE_OS_SEC_TEXT_INIT void osSystemInfo(void)
{
    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n\n"
	        "LiteOS Kernel Version : %s\n"
            "Processor   : %s"
#ifdef LOSCFG_KERNEL_SMP
            " * %d\n"
            "Run Mode    : SMP\n"
#else
            "\n"
            "Run Mode    : UP\n"
#endif
            "GIC Rev     : %s\n"
            "build time  : %s %s\n\n"
            "**********************************\n",
			HW_LITEOS_KERNEL_VERSION_STRING,
            LOS_CpuInfo(),
#ifdef LOSCFG_KERNEL_SMP
            LOSCFG_KERNEL_SMP_CORE_NUM,
#endif
            HalIrqVersion(), __DATE__,__TIME__);
            LOS_HelloRust();
}

LITE_OS_SEC_TEXT_INIT int secondary_cpu_start(void)
{
    OsCurrTaskSet(OsGetMainTask());

    /* increase cpu counter */
    LOS_AtomicInc(&ncpu);

    HalIrqInitPercpu();
#ifdef LOSCFG_BASE_CORE_SWTMR
    OsSwtmrInit();
#endif
    OsIdleTaskCreate();
    OsStart();

    while(1){
        __asm volatile("wfi");
    }
}

#ifdef LOSCFG_KERNEL_SMP
LITE_OS_SEC_TEXT_INIT VOID release_secondary_cores(void)
{
    /* send SGI to wakeup APs */
    (void)HalIrqSendIpi(0x00, 0x0F);
    *(int*)SYS_FLAGSSET = 0x10000;

    /* wait until all APs are ready */
    while (LOS_AtomicRead(&ncpu) < LOSCFG_KERNEL_CORE_NUM) {
        asm volatile("wfe");
    }
}
#endif

LITE_OS_SEC_TEXT_INIT int main(void)
{
    UINT32 ret = LOS_OK;

#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    /* early init uart output */
    uart_early_init();

    /* system and chip info */
    osSystemInfo();

    PRINTK("\nmain core booting up...\n");
    ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

#ifdef LOSCFG_KERNEL_SMP
    PRINTK("releasing %u secondary cores\n", LOSCFG_KERNEL_SMP_CORE_NUM - 1);
    release_secondary_cores();
#endif

    OsStart();

    while (1) {
        __asm volatile("wfi");
    }
}
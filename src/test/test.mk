LIB_SUBDIRS += test

# Kernel base test module
ifeq ($(LOSCFG_TEST_KERNEL_BASE), y)
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_TASK), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/task
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_SWTMR), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/swtmr
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_HWI), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/hwi
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_IPC_QUEUE), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/queue
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_IPC_MUX), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/mux
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_IPC_SEM), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/sem
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_IPC_EVENT), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/event
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_MP), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/mp
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_MEM), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/mem
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_MISC), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/misc
    endif
    ifeq ($(LOSCFG_TEST_KERNEL_BASE_ATOMIC), y)
        LIB_SUBDIRS +=  test/sample/kernel/base/atomic
    endif
endif

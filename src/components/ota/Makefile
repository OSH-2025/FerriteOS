include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := ota

LOCAL_SRCS := \
    $(wildcard upgrade/*.c) \
    $(wildcard flag_operate/*.c) \
    $(wildcard utility/*.c) \
    $(wildcard package/*.c) \
    $(wildcard package/opt/*.c) \
    $(wildcard sota/*.c)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_INCLUDE)

LOCAL_EXT_FLAG := -Wno-pointer-sign -Wno-unused-but-set-variable \
                  -Wno-return-type -Wno-unused-variable \
                  -Wno-unused-function -Wno-implicit-function-declaration \
                  -Wno-int-conversion

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)

include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := can_demo

CAN_DEMO_SRC += $(wildcard *.c)

HAL_DRIVER_INC := \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F4xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/compat/cmsis
CAN_DEMO_INC += $(HAL_DRIVER_INC)

USER_INCLUDE += -I $(LITEOSTOPDIR)/targets/Cloud_STM32F429IGTx_FIRE/Inc
CAN_DEMO_INC += $(USER_INCLUDE)

CAN_DEMO_INC += -I $(LITEOSTOPDIR)/demos/drivers/can

LOCAL_SRCS := $(CAN_DEMO_SRC)
LOCAL_INCLUDE := $(CAN_DEMO_INC)
LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)

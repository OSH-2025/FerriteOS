include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/gui/gui.mk

MODULE_NAME := gui_demo

BOARD_INCLUDES += \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers

BOARD_INCLUDES += $(COMPONENTS_GUI_INCLUDE)
LVGL_DEMO_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/gui \
    -I $(LITEOSTOPDIR)/demos/gui/widgets
BOARD_INCLUDES += $(LVGL_DEMO_INCLUDE)

# C includes
HAL_DRIVER_INC := \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32F7xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32F769IDISCOVERY/Drivers/BSP/STM32F769I-Discovery/Utilities/Fonts \
    -I $(LITEOSTOPDIR)/compat/cmsis
BOARD_INCLUDES += $(HAL_DRIVER_INC)

LVGL_DEMO_SRC = \
    $(wildcard *.c) \
    $(wildcard widgets/*.c)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(LVGL_DEMO_SRC))

LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_CDEFS = \
    -D STM32F7 \
    -D STM32F769xx \
    -D LV_CONF_INCLUDE_SIMPLE

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)

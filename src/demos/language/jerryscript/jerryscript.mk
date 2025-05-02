DEMOS_JERRYSCRIPT_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/language/jerryscript

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -ljerryscript_demo
endif


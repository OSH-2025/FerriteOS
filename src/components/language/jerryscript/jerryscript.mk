JERRYSCRIPT_VERSION = jerryscript-2.4.0

COMPONENTS_JERRYSCRIPT_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/language/jerryscript/$(JERRYSCRIPT_VERSION)/jerry-core/include \
    -I $(LITEOSTOPDIR)/components/language/jerryscript/$(JERRYSCRIPT_VERSION)/jerry-ext/include

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -ljerryscript
endif


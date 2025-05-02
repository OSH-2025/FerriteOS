DEMOS_LANGUAGE_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_JERRYSCRIPT), y)
include $(LITEOSTOPDIR)/demos/language/jerryscript/jerryscript.mk
DEMOS_LANGUAGE_INCLUDE += $(DEMOS_JERRYSCRIPT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LUA), y)
include $(LITEOSTOPDIR)/demos/language/lua/lua.mk
DEMOS_LANGUAGE_INCLUDE += $(DEMOS_LUA_INCLUDE)
endif

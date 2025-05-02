REGEX_VERSION = regex-master

COMPONENTS_REGEX_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/regex/$(REGEX_VERSION)

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -lregex
endif

MICROPYTHON_VERSION = micropython-1.16

COMPONENTS_MICROPYTHON_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/py

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -lmicropython
endif


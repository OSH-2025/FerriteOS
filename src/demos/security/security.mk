DEMOS_SECURITY_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_OPENSSL), y)
include $(LITEOSTOPDIR)/demos/security/openssl/openssl.mk
DEMOS_SECURITY_INCLUDE += $(DEMOS_OPENSSL_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_RHASH), y)
include $(LITEOSTOPDIR)/demos/security/rhash/rhash.mk
DEMOS_SECURITY_INCLUDE += $(DEMOS_RHASH_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TINYCRYPT), y)
include $(LITEOSTOPDIR)/demos/security/tinycrypt/tinycrypt.mk
DEMOS_SECURITY_INCLUDE += $(DEMOS_TINYCRYPT_INCLUDE)
endif

COMPONENTS_MBEDTLS_INCLUDE :=

MBEDTLS_VERSION = mbedtls-3.0.0

COMPONENTS_MBEDTLS_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls_port \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/$(MBEDTLS_VERSION)/include

include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/openssl/openssl.mk

MODULE_NAME := openssl

LOCAL_INCLUDE := \
    -I $(OPENSSL_VERSION) \
    -I $(OPENSSL_VERSION)/include \
    -I $(OPENSSL_VERSION)/crypto/include \
    -I $(OPENSSL_VERSION)/crypto/modes

LOCAL_DIRS_y := \
    aes asn1 async bf bio blake2 bn buffer camellia cast chacha cmac \
    cms comp ct conf des dh dsa dso ec engine err evp hmac idea \
    kdf lhash md4 md5 mdc2 modes objects ocsp pem \
    pkcs12 rand rc2 rc4  ripemd rsa ecdsa  ecdh krb5 pqueue \
    seed sha srp stack ts txt_db ui whrlpool x509 x509v3 poly1305

SSL_SRCS := \
    $(wildcard $(OPENSSL_VERSION)/ssl/*.c) \
    $(wildcard $(OPENSSL_VERSION)/ssl/statem/*.c) \
    $(wildcard $(OPENSSL_VERSION)/ssl/record/*.c) \
    $(wildcard $(OPENSSL_VERSION)/crypto/async/arch/async_null.c) \
    $(wildcard $(OPENSSL_VERSION)/crypto/pkcs7/pk7_lib.c) \
    $(wildcard $(OPENSSL_VERSION)/crypto/pkcs7/pkcs7err.c)

CRYPTO_SRCS := \
    $(wildcard $(OPENSSL_VERSION)/crypto/*.c) \
    $(foreach dir,$(LOCAL_DIRS_y), $(wildcard $(OPENSSL_VERSION)/crypto/$(dir)/*.c))

OUT_SRCS := \
    $(OPENSSL_VERSION)/crypto/bf/bf_cbc.c \
    $(OPENSSL_VERSION)/crypto/x509v3/tabtest.c \
    $(OPENSSL_VERSION)/crypto/x509v3/v3conf.c \
    $(OPENSSL_VERSION)/crypto/x509v3/v3prin.c \
    $(OPENSSL_VERSION)/crypto/LPdir_nyi.c \
    $(OPENSSL_VERSION)/crypto/LPdir_unix.c \
    $(OPENSSL_VERSION)/crypto/LPdir_vms.c \
    $(OPENSSL_VERSION)/crypto/LPdir_win.c \
    $(OPENSSL_VERSION)/crypto/LPdir_win32.c \
    $(OPENSSL_VERSION)/crypto/LPdir_wince.c \
    $(OPENSSL_VERSION)/crypto/o_dir.c

OPENSSL_SRCS := $(SSL_SRCS) $(CRYPTO_SRCS)
LOCAL_SRCS := $(filter-out $(OUT_SRCS), $(OPENSSL_SRCS))

LOCAL_CDEFS := -DOPENSSLDIR="\"$(OPENSSLDIR)\"" -DENGINESDIR="\"$(ENGINESDIR)\""

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-implicit-int -Wno-strict-prototypes -Wno-format-nonliteral \
    -Wno-unused-but-set-variable -Wno-unused-variable -Wno-float-equal \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)

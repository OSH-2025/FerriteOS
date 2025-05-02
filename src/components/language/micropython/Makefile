include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/language/micropython/micropython.mk

MODULE_NAME := $(notdir $(CURDIR))

MICROPYTHON_SRCS := \
    $(wildcard $(MICROPYTHON_VERSION)/py/*.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modujson.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modure.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moduzlib.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moduheapq.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modutimeq.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moduhashlib.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moducryptolib.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modubinascii.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/virtpin.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_mem.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_pinbase.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_signal.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_pulse.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_i2c.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/machine_spi.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modussl_axtls.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modussl_mbedtls.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modurandom.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moduselect.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/moduwebsocket.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modwebrepl.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/modframebuf.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_blockdev.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_reader.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_posix.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_posix_file.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_fat.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_fat_diskio.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_fat_file.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/vfs_lfs.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/utime_mphal.c) \
    $(wildcard $(MICROPYTHON_VERSION)/extmod/uos_dupterm.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/utils/printf.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/utils/stdout_helpers.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/utils/sys_stdio_mphal.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/utils/interrupt_char.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/mp-readline/readline.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/timeutils/timeutils.c) \
    $(wildcard $(MICROPYTHON_VERSION)/lib/utils/gchelper_generic.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/main.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/alloc.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/gccollect.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/unix_mphal.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/input.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/modmachine.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/modos.c) \
    $(wildcard $(MICROPYTHON_VERSION)/ports/unix/build-standard/frozen_content.c)

MICROPYTHON_CMD_SRCS := $(wildcard micropython_shellcmd.c)
MICROPYTHON_SRCS += $(MICROPYTHON_CMD_SRCS)

MICROPYTHON_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/language/micropython \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION) \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/py \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/ports/unix \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/ports/unix/build \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/ports/unix/build-standard/ \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/ports/unix/variants/standard \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/extmod/axtls-include \
    -I $(LITEOSTOPDIR)/components/language/micropython/$(MICROPYTHON_VERSION)/ports/unix/build-standard/genhdr

LOCAL_DEFS := \
    -D MICROPY_USE_INTERNAL_PRINTF=0 \
    -D MICROPY_VFS_FAT=0 \
    -D MICROPY_SSL_AXTLS=0 \
    -D MICROPY_FORCE_32BIT=0 \
    -D MICROPY_USE_READLINE=1 \
    -D MICROPY_PY_BTREE=0 \
    -D MICROPY_PY_THREAD=0 \
    -D MICROPY_PY_TERMIOS=0 \
    -D MICROPY_PY_SOCKET=0 \
    -D MICROPY_PY_FFI=0 \
    -D MICROPY_PY_USSL=0 \
    -D MICROPY_SSL_AXTLS=0 \
    -D MICROPY_SSL_MBEDTLS=0 \
    -D MICROPY_PY_JNI=0 \
    -D MICROPY_STANDALONE=0 \
    -D MICROPY_PY_USELECT_POSIX=0

LOCAL_EXT_FLAG := \
    -fstack-protector-strong \
    -Wno-error=strict-prototypes \
    -Wno-strict-prototypes \
    -Wno-error=unused-function \
    -Wno-error=float-equal \
    -Wno-float-equal \
    -Wno-implicit-function-declaration \
    -Wno-unused-variable \
    -Wno-unused-function

LOCAL_SRCS := $(filter-out $(MICROPYTHON_OUT_SRCS), $(MICROPYTHON_SRCS))
LOCAL_INCLUDE := $(MICROPYTHON_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) $(LOCAL_DEFS)

include $(MODULE)


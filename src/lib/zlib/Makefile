include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := z

LOCAL_SRCS := adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c \
			  gzread.c gzwrite.c infback.c inffast.c inflate.c inftrees.c \
			  trees.c uncompr.c zlib.3 zutil.c
LOCAL_SRCS := $(addprefix src/, $(LOCAL_SRCS))

include $(MODULE)
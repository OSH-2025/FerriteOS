include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/pcap/pcap.mk

MODULE_NAME := pcap

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/net/pcap/$(LIBPCAP_VERSION) \
    -I $(LITEOSTOPDIR)/components/net/pcap/$(LIBPCAP_VERSION)/missing \
    -I $(LITEOSTOPDIR)/components/net/pcap/$(LIBPCAP_VERSION)/pcap

LOCAL_INCLUDE += $(LWIP_INCLUDE)

LIBPCAP_SRC := \
    bpf_dump.c bpf_image.c etherent.c fad-gifc.c gencode.c \
    inet.c nametoaddr.c optimize.c pcap.c savefile.c \
    sf-pcap.c sf-pcap-ng.c grammar.c scanner.c version.c \
    pcap-common.c pcap_liteos.c

LOCAL_SRCS = $(foreach file, $(LIBPCAP_SRC), $(LIBPCAP_VERSION)/$(file))
LOCAL_SRCS += $(wildcard $(LIBPCAP_VERSION)/bpf/net/bpf_filter.c)
LOCAL_SRCS += $(wildcard pcap_dump.c)

LOCAL_CDEFS = -DHAVE_CONFIG_H -D_U_=

LOCAL_EXT_FLAG := -Wno-error -Wno-unused-const-variable \
    -Wno-unused-variable -Wno-incompatible-pointer-types \
    -Wno-unused-function -Wno-pointer-sign -Wno-format-nonliteral \
    -Wno-discarded-qualifiers -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)

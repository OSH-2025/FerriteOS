############################# SRCS #################################
HAL_DRIVER_TYPE :=

########################## CB2201 Options ####################################
ifeq ($(LOSCFG_PLATFORM_CB2201), y)
    HAL_DRIVER_TYPE := csky_driver
########################## SMARTL_E802 Options ####################################
else ifeq ($(LOSCFG_PLATFORM_SMARTL_E802), y)
    HAL_DRIVER_TYPE := csky_driver_qemu
######################### GD32E103C_START Options ###############################
else ifeq ($(LOSCFG_PLATFORM_GD32E103C_START), y)
    LITEOS_CMACRO_TEST += -DGD32E103R_START
    GD_DRIVER_TYPE := GD32E10x_Firmware/GD32E10x_Firmware_Library/Firmware/GD32E10x_standard_peripheral
######################### GD32F303RGT6_BearPi Options ###############################
else ifeq ($(LOSCFG_PLATFORM_GD32F303RGT6_BEARPI), y)
    LITEOS_CMACRO_TEST += -DGD32F30X_XD 
    GD_DRIVER_TYPE := GD32F30x_Firmware/GD32F30x_Firmware_Library/Firmware/GD32F30x_standard_peripheral
######################### GD32VF103VBT6 Options ###############################
else ifeq ($(LOSCFG_PLATFORM_GD32VF103V_EVAL), y)
    LITEOS_CMACRO_TEST += -DGD32VF103V_EVAL
    GD_DRIVER_TYPE := GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/GD32VF103_standard_peripheral
########################## qemu virt a53 Options ##############################
else ifeq ($(LOSCFG_PLATFORM_QEMU_VIRT_A53), y)
    LITEOS_CMACRO_TEST += -DTESTVIRTA53
######################### realview pbx a9 Options #############################
else ifeq ($(LOSCFG_PLATFORM_PBX_A9), y)
    LITEOS_CMACRO_TEST += -DTESTPBXA9
######################### Cloud_STM32F429IGTx_FIRE Options ####################
else ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
    LITEOS_CMACRO_TEST += -DTESTSTM32F429IGTX
    LITEOS_CMACRO_TEST += -DSTM32F429xx
    HAL_DRIVER_TYPE := STM32F4xx_HAL_Driver
######################### STM32F072_Nucleo Options #############################
else ifeq ($(LOSCFG_PLATFORM_STM32F072_Nucleo), y)
    LITEOS_CMACRO_TEST += -DSTM32F072xB
    HAL_DRIVER_TYPE := STM32F0xx_HAL_Driver
######################### STM32F103_FIRE_Arbitrary Options #####################
else ifeq ($(LOSCFG_PLATFORM_STM32F103_FIRE_ARBITRARY), y)
    LITEOS_CMACRO_TEST += -D__FPU_PRESENT -DSTM32F103xE
    HAL_DRIVER_TYPE := STM32F1xx_HAL_Driver
######################### STM32F103_C8T6 #####################
else ifeq ($(LOSCFG_PLATFORM_STM32F103_C8T6), y)
    LITEOS_CMACRO_TEST += -D__FPU_PRESENT -DSTM32F103xB
    HAL_DRIVER_TYPE := STM32F1xx_HAL_Driver
######################### STM32F407_ATK_Explorer Options #######################
else ifeq ($(LOSCFG_PLATFORM_STM32F407_ATK_EXPLORER), y)
    LITEOS_CMACRO_TEST += -DSTM32F407xx
    HAL_DRIVER_TYPE := STM32F4xx_HAL_Driver
######################### STM32F769IDISCOVERY Options ##########################
else ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY), y)
    LITEOS_CMACRO_TEST += -DTESTSTM32F769IDISCOVERY
    LITEOS_CMACRO_TEST += -DSTM32F769xx
    HAL_DRIVER_TYPE := STM32F7xx_HAL_Driver
######################### STM32F767_Nucleo Options ##########################
else ifeq ($(LOSCFG_PLATFORM_STM32F767_NUCLEO), y)
    LITEOS_CMACRO_TEST += -DSTM32F767xx
    HAL_DRIVER_TYPE := STM32F7xx_HAL_Driver
######################### STM32F746_Nucleo Options ##########################
else ifeq ($(LOSCFG_PLATFORM_STM32F746_NUCLEO), y)
    LITEOS_CMACRO_TEST += -DSTM32F746xx
    HAL_DRIVER_TYPE := STM32F7xx_HAL_Driver
######################### STM32L4R9I_Discovery Options #########################
else ifeq ($(LOSCFG_PLATFORM_STM32L4R9I_DISCOVERY), y)
    LITEOS_CMACRO_TEST += -DSTM32L4R9xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
######################### STM32L073_Nucleo Options #############################
else ifeq ($(LOSCFG_PLATFORM_STM32L073_NUCLEO), y)
    LITEOS_CMACRO_TEST += -DSTM32L073xx
    HAL_DRIVER_TYPE := STM32L0xx_Firmware/STM32L0xx_HAL_Driver
######################### STM32L431_BearPi Options #############################
else ifeq ($(LOSCFG_PLATFORM_STM32L431_BearPi), y)
    LITEOS_CMACRO_TEST += -DTESTSTM32L431_BearPi -DSTM32L431xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
######################### STM32L476_NB476 Options ##############################
else ifeq ($(LOSCFG_PLATFORM_STM32L476_NB476), y)
    LITEOS_CMACRO_TEST += -DSTM32L476xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
######################### STM32L496_Nucleo Options #############################
else ifeq ($(LOSCFG_PLATFORM_STM32L496_NUCLEO), y)
    LITEOS_CMACRO_TEST += -DSTM32L496xx
    HAL_DRIVER_TYPE := STM32L4xx_HAL_Driver
######################### STM32L552_Nucleo Options #############################
else ifeq ($(LOSCFG_PLATFORM_STM32L552_NUCLEO), y)
    LITEOS_CMACRO_TEST += -DSTM32L552xx
    HAL_DRIVER_TYPE := STM32L5xx_Firmware/STM32L5xx_HAL_Driver
######################### HiFive1_Rev1_B01 Options###############################
else ifeq ($(LOSCFG_PLATFORM_HIFIVE1_REV1_B01), y)
    HAL_DRIVER_TYPE := Hifive1_Revb_Firmware/freedom-metal
######################### FM33LC0XX_DEMO Options #############################
else ifeq ($(LOSCFG_PLATFORM_FM33LC0XX_DEMO), y)
    LITEOS_CMACRO_TEST += -DFM33LC0XX
    HAL_DRIVER_TYPE := FM33LC0xx_LL_Driver
######################### APM32F103_Geehy Options ###############################
else ifeq ($(LOSCFG_PLATFORM_APM32F103_GEEHY), y)
    LITEOS_CMACRO_TEST += -DAPM32F10X_HD
    APM_DRIVER_TYPE := APM32F10x_StdPeriphDriver
######################### RASPBERRY_PI2B Options ################################
else ifeq ($(LOSCFG_PLATFORM_RASPBERRY_PI2B), y)
    HAL_DRIVER_TYPE := BCM2836_HAL_Driver
endif

HAL_DRIVER_SRC := drivers/$(HAL_DRIVER_TYPE)

LITEOS_PLATFORM  := $(subst $\",,$(LOSCFG_PLATFORM))

PLATFORM_INCLUDE := -I $(LITEOSTOPDIR)/targets/bsp/common \
                    -I $(LITEOSTOPDIR)/targets/bsp/drivers/at_hal \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc \
                    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/asm \
                    -I $(LITEOSTOPDIR)/arch/arm/cortex_m/cmsis \
                    -I $(LITEOSTOPDIR)/lib/huawei_libc/time

ifeq ($(LOSCFG_ARCH_RISCV_RV32IMC), y)
PLATFORM_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/drivers \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/stubs
    PLATFORM_DEFS +=  -DUSE_STDPERIPH_DRIVER
endif

ifeq ($(LOSCFG_DRIVER_HAL_LIB), y)
PLATFORM_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(HAL_DRIVER_TYPE)/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(HAL_DRIVER_TYPE)/Inc/Legacy
endif

ifeq ($(LOSCFG_DRIVER_GD_LIB), y)
PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(GD_DRIVER_TYPE)/Include
PLATFORM_DEFS +=  -DUSE_STDPERIPH_DRIVER
endif

ifeq ($(LOSCFG_DRIVER_CSKY_LIB), y)
PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(HAL_DRIVER_TYPE)/Inc
endif

ifeq ($(LOSCFG_DRIVER_QEMU_CSKY_LIB), y)
PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(HAL_DRIVER_TYPE)/Inc
endif

ifeq ($(LOSCFG_DRIVER_SIFIVE_LIB), y)
    PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/targets/bsp/$(HAL_DRIVER_SRC)
endif

ifeq ($(LOSCFG_DRIVER_APM_LIB), y)
    PLATFORM_INCLUDE += -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(APM_DRIVER_TYPE)/Inc
endif

LIB_SUBDIRS += targets/bsp
LIB_SUBDIRS += targets/$(LITEOS_PLATFORM)

#determine libs to link for windows
ifneq ($(OS), Linux)
    LITEOS_BASELIB += -l$(LITEOS_PLATFORM) -lsec -lbase -linit -lbsp -lc -lm -losdepends

    LITEOS_BASELIB += -linterrupt -ltimer
    ifeq ($(LOSCFG_KERNEL_LOWPOWER), y)
        LITEOS_BASELIB += -llowpower
    endif

    ifeq ($(LOSCFG_DRIVERS_UART_ARM_PL011), y)
        LITEOS_BASELIB += -larm_pl011
    endif
    ifeq ($(LOSCFG_DRIVERS_SIMPLE_UART), y)
        LITEOS_BASELIB += -luart
    endif

    ifeq ($(LITEOS_CPU_TYPE), $(findstring $(LITEOS_CPU_TYPE), cortex-a7 cortex-a9))
        LITEOS_BASELIB += -lcsysdeps
    endif
    ifeq ($(LOSCFG_LIB_CPP_EXTEND), y)
        LITEOS_BASELIB += -lc++
    endif
    ifeq ($(LOSCFG_SHELL), y)
        LITEOS_BASELIB += -lshell
    endif
    ifeq ($(LOSCFG_COMPAT_CMSIS), y)
        LITEOS_BASELIB += -lcmsis
    endif
    ifeq ($(LOSCFG_COMPAT_POSIX), y)
        LITEOS_BASELIB += -lposix
    endif
    ifeq ($(LOSCFG_KERNEL_TICKLESS), y)
        LITEOS_BASELIB += -ltickless
    endif
    ifeq ($(LOSCFG_KERNEL_CPUP), y)
        LITEOS_BASELIB += -lcpup
    endif
    ifeq ($(LOSCFG_KERNEL_TRACE), y)
        LITEOS_BASELIB += -ltrace
    endif
    ifeq ($(LOSCFG_KERNEL_CPPSUPPORT), y)
        LITEOS_BASELIB += -lcppsupport
    endif
    ifeq ($(LOSCFG_COMPONENTS_FS), y)
        LITEOS_BASELIB += -lfs
    endif
    ifeq ($(LOSCFG_COMPONENTS_GUI), y)
        LITEOS_BASELIB += -lgui
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_AT), y)
        LITEOS_BASELIB += -lnet_at
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_NB_IOT), y)
        LITEOS_BASELIB += -lnb_iot
    endif
    ifeq ($(LOSCFG_COMPONENTS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub
    endif
    ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
        LITEOS_BASELIB += -lsensorhub_demo
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_LWIP), y)
        LITEOS_BASELIB += -llwip
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_IFCONFIG), y)
        LITEOS_BASELIB += -lifconfig
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_PING), y)
        LITEOS_BASELIB += -lping
    endif
    ifeq ($(LOSCFG_COMPONENTS_SECURITY_MBEDTLS), y)
        LITEOS_BASELIB += -lmbedtls
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIB_CJSON), y)
        LITEOS_BASELIB += -lcjson
    endif
    ifeq ($(LOSCFG_COMPONENTS_ATINY_LOG), y)
        LITEOS_BASELIB += -latiny_log
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_MQTT), y)
        LITEOS_BASELIB += -lmqtt
    endif
    ifeq ($(LOSCFG_COMPONENTS_CONNECTIVITY_LWM2M), y)
        LITEOS_BASELIB += -llwm2m
    endif
    ifeq ($(LOSCFG_COMPONENTS_ATINY_MQTT), y)
        LITEOS_BASELIB += -latiny_mqtt
    endif
    ifeq ($(LOSCFG_COMPONENTS_ATINY_LWM2M), y)
        LITEOS_BASELIB += -latiny_lwm2m
    endif
    ifeq ($(LOSCFG_COMPONENTS_NET_SAL), y)
        LITEOS_BASELIB += -lsal
    endif
    ifeq ($(LOSCFG_COMPONENTS_INIPARSER), y)
        LITEOS_BASELIB += -liniparser
    endif
    ifeq ($(LOSCFG_COMPONENTS_JSON_C), y)
        LITEOS_BASELIB += -ljson-c
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIBPNG), y)
        LITEOS_BASELIB += -lpng
        LITEOS_BASELIB += -lz
    endif
    ifeq ($(LOSCFG_COMPONENTS_FREETYPE), y)
        LITEOS_BASELIB += -lfreetype
    endif
    ifeq ($(LOSCFG_COMPONENTS_HARFBUZZ), y)
        LITEOS_BASELIB += -lharfbuzz
    endif
    ifeq ($(LOSCFG_COMPONENTS_BIDIREFERENCE), y)
        LITEOS_BASELIB += -lbidireference
    endif
    ifeq ($(LOSCFG_COMPONENTS_LUA), y)
        LITEOS_BASELIB += -llua
    endif
    ifeq ($(LOSCFG_COMPONENTS_OPUS), y)
        LITEOS_BASELIB += -lopus
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIBXML2), y)
        LITEOS_BASELIB += -lxml2
        LITEOS_BASELIB += -lz
    endif
    ifeq ($(LOSCFG_COMPONENTS_IPERF), y)
        LITEOS_BASELIB += -llos_iperf
    endif
    ifeq ($(LOSCFG_COMPONENTS_PCAP), y)
        LITEOS_BASELIB += -lpcap
    endif
    ifeq ($(LOSCFG_COMPONENTS_CURL), y)
        LITEOS_BASELIB += -lcurl
    endif
    ifeq ($(LOSCFG_COMPONENTS_OPENSSL), y)
        LITEOS_BASELIB += -lopenssl
    endif
    ifeq ($(LOSCFG_COMPONENTS_SQLITE), y)
        LITEOS_BASELIB += -lsqlite
    endif
    ifeq ($(LOSCFG_COMPONENTS_JSONCPP), y)
        LITEOS_BASELIB += -ljsoncpp
    endif
    ifeq ($(LOSCFG_COMPONENTS_THTTPD), y)
        LITEOS_BASELIB += -lthttpd
    endif
    ifeq ($(LOSCFG_COMPONENTS_OPENEXIF), y)
        LITEOS_BASELIB += -lopenexif
    endif
    ifeq ($(LOSCFG_COMPONENTS_TINYXML2), y)
        LITEOS_BASELIB += -ltinyxml2
    endif
    ifeq ($(LOSCFG_COMPONENTS_TFTP_SERVER), y)
        LITEOS_BASELIB += -ltftp_server
    endif
    ifeq ($(LOSCFG_COMPONENTS_UPNP), y)
        LITEOS_BASELIB += -lupnp
    endif
    ifeq ($(LOSCFG_COMPONENTS_QRENCODE), y)
        LITEOS_BASELIB += -lqrencode
    endif
    ifeq ($(LOSCFG_COMPONENTS_FASTLZ), y)
        LITEOS_BASELIB += -lfastlz
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIBRWS), y)
        LITEOS_BASELIB += -llibrws
    endif
    ifeq ($(LOSCFG_COMPONENTS_INIH), y)
        LITEOS_BASELIB += -linih
    endif
    ifeq ($(LOSCFG_COMPONENTS_PIXMAN), y)
        LITEOS_BASELIB += -lpixman
    endif
    ifeq ($(LOSCFG_COMPONENTS_SFUD), y)
        LITEOS_BASELIB += -lsfud
    endif
    ifeq ($(LOSCFG_COMPONENTS_C_ALGORITHMS), y)
        LITEOS_BASELIB += -lc-algorithms
    endif
    ifeq ($(LOSCFG_COMPONENTS_HTTP_PARSER), y)
        LITEOS_BASELIB += -lhttp_parser
    endif
    ifeq ($(LOSCFG_COMPONENTS_URIPARSER), y)
        LITEOS_BASELIB += -luriparser
    endif
    ifeq ($(LOSCFG_COMPONENTS_RHASH), y)
        LITEOS_BASELIB += -lrhash
    endif
    ifeq ($(LOSCFG_COMPONENTS_JSMN), y)
        LITEOS_BASELIB += -ljsmn
    endif
    ifeq ($(LOSCFG_COMPONENTS_MINMEA), y)
        LITEOS_BASELIB += -lminmea
    endif
    ifeq ($(LOSCFG_COMPONENTS_TINYCRYPT), y)
        LITEOS_BASELIB += -ltinycrypt
    endif
    ifeq ($(LOSCFG_COMPONENTS_GENANN), y)
        LITEOS_BASELIB += -lgenann
    endif
    ifeq ($(LOSCFG_COMPONENTS_TINYFRAME), y)
        LITEOS_BASELIB += -ltinyframe
    endif
    ifeq ($(LOSCFG_COMPONENTS_EASYFLASH), y)
        LITEOS_BASELIB += -leasyflash
    endif
    ifeq ($(LOSCFG_COMPONENTS_LIBEVENT), y)
        LITEOS_BASELIB += -llibevent
    endif
    ifeq ($(LOSCFG_COMPONENTS_APRILTAG), y)
        LITEOS_BASELIB += -lapriltag
    endif
    ifeq ($(LOSCFG_DEMOS_KERNEL), y)
        LITEOS_BASELIB += -lkernel_demo
    endif
    ifeq ($(LOSCFG_DEMOS_FS), y)
        LITEOS_BASELIB += -lfs_demo
    endif
    ifeq ($(LOSCFG_DEMOS_GUI), y)
        LITEOS_BASELIB += -lgui_demo
    endif
    ifeq ($(LOSCFG_DEMOS_AGENT_TINY_MQTT), y)
        LITEOS_BASELIB += -lagenttiny_mqtt
    endif
    ifeq ($(LOSCFG_DEMOS_AGENT_TINY_LWM2M), y)
        LITEOS_BASELIB += -lagenttiny_lwm2m
    endif
    ifeq ($(LOSCFG_DEMOS_DTLS_SERVER), y)
        LITEOS_BASELIB += -ldtls_server
    endif
    ifeq ($(LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY), y)
        LITEOS_BASELIB += -lnbiot_without_atiny
    endif
    ifeq ($(LOSCFG_DEMOS_IPV6_CLIENT), y)
        LITEOS_BASELIB += -lipv6_client
    endif
    ifeq ($(LOSCFG_DEMOS_LMS), y)
        LITEOS_BASELIB += -llms_demo
    endif
    ifeq ($(LOSCFG_DEMOS_TRACE), y)
        LITEOS_BASELIB += -ltrace_demo
    endif
    ifeq ($(LOSCFG_DEMOS_AI), y)
        LITEOS_BASELIB += -lai_demo
    endif
    ifeq ($(LOSCFG_COMPONENTS_ICONV), y)
        LITEOS_BASELIB += -liconv
    endif
    ifeq ($(LOSCFG_DEMOS_ICONV), y)
        LITEOS_BASELIB += -liconv_demo
    endif
    ifeq ($(LOSCFG_DEMOS_JSON_C), y)
        LITEOS_BASELIB += -ljson-c_demo
    endif
    ifeq ($(LOSCFG_DEMOS_INIPARSER), y)
        LITEOS_BASELIB += -liniparser_demo
    endif
    ifeq ($(LOSCFG_DEMOS_LIBPNG), y)
        LITEOS_BASELIB += -lpng_demo
    endif
    ifeq ($(LOSCFG_DEMOS_BIDIREFERENCE), y)
        LITEOS_BASELIB += -lbidireference_demo
    endif
    ifeq ($(LOSCFG_DEMOS_FREETYPE), y)
        LITEOS_BASELIB += -lfreetype_demo
    endif
    ifeq ($(LOSCFG_DEMOS_HARFBUZZ), y)
        LITEOS_BASELIB += -lharfbuzz_demo
    endif
    ifeq ($(LOSCFG_DEMOS_LUA), y)
        LITEOS_BASELIB += -llua_demo
    endif
    ifeq ($(LOSCFG_DEMOS_OPUS), y)
        LITEOS_BASELIB += -lopus_demo
    endif
    ifeq ($(LOSCFG_DEMOS_LIBXML2), y)
        LITEOS_BASELIB += -lxml2_demo
    endif
    ifeq ($(LOSCFG_DEMOS_CURL), y)
        LITEOS_BASELIB += -lcurl_demo
    endif
    ifeq ($(LOSCFG_DEMOS_OPENSSL), y)
        LITEOS_BASELIB += -lopenssl_demo
    endif
    ifeq ($(LOSCFG_DEMOS_SQLITE), y)
        LITEOS_BASELIB += -lsqlite_demo
    endif
    ifeq ($(LOSCFG_DEMOS_JSONCPP), y)
        LITEOS_BASELIB += -ljsoncpp_demo
    endif
    ifeq ($(LOSCFG_DEMOS_OPENEXIF), y)
        LITEOS_BASELIB += -lopenexif_demo
    endif
    ifeq ($(LOSCFG_DEMOS_TINYXML2), y)
        LITEOS_BASELIB += -ltinyxml2_demo
    endif
    ifeq ($(LOSCFG_DEMOS_UPNP), y)
        LITEOS_BASELIB += -lupnp_demo
    endif
    ifeq ($(LOSCFG_DEMOS_QRENCODE), y)
        LITEOS_BASELIB += -lqrencode_demo
    endif
    ifeq ($(LOSCFG_DEMOS_PIXMAN), y)
        LITEOS_BASELIB += -lpixman_demo
    endif
    ifeq ($(LOSCFG_DEMOS_FASTLZ), y)
        LITEOS_BASELIB += -lfastlz_demo
    endif
    ifeq ($(LOSCFG_DEMOS_LIBRWS), y)
        LITEOS_BASELIB += -llibrws_demo
    endif
    ifeq ($(LOSCFG_DEMOS_INIH), y)
        LITEOS_BASELIB += -linih_demo
    endif
    ifeq ($(LOSCFG_DEMOS_HTTP_CLIENT), y)
        LITEOS_BASELIB += -lhttpc_demo
    endif
    ifeq ($(LOSCFG_DEMOS_SFUD), y)
        LITEOS_BASELIB += -lsfud_demo
    endif
    ifeq ($(LOSCFG_DEMOS_URIPARSER), y)
        LITEOS_BASELIB += -luriparser_demo
    endif
    ifeq ($(LOSCFG_DEMOS_C_ALGORITHMS), y)
        LITEOS_BASELIB += -lc_algorithms_demo
    endif
    ifeq ($(LOSCFG_DEMOS_RHASH), y)
        LITEOS_BASELIB += -lrhash_demo
    endif
    ifeq ($(LOSCFG_DEMOS_RHASH), y)
        LITEOS_BASELIB += -lrhash_demo
    endif
    ifeq ($(LOSCFG_DEMOS_JSMN), y)
        LITEOS_BASELIB += -ljsmn_demo
    endif
    ifeq ($(LOSCFG_DEMOS_MINMEA), y)
        LITEOS_BASELIB += -lminmea_demo
    endif
    ifeq ($(LOSCFG_DEMOS_AGRICULTURE), y)
        LITEOS_BASELIB += -lagriculture
    endif
    ifeq ($(LOSCFG_DEMOS_TINYCRYPT), y)
        LITEOS_BASELIB += -ltinycrypt_demo
    endif
    ifeq ($(LOSCFG_DEMOS_GENANN), y)
        LITEOS_BASELIB += -lgenann_demo
    endif
    ifeq ($(LOSCFG_DEMOS_EASYFLASH), y)
        LITEOS_BASELIB += -lef_demo
    endif
    ifeq ($(LOSCFG_DEMOS_TINYFRAME), y)
        LITEOS_BASELIB += -ltinyframe_demo
    endif
    ifeq ($(LOSCFG_DEMOS_LIBEVENT), y)
        LITEOS_BASELIB += -llibevent_demo
    endif
    ifeq ($(LOSCFG_DEMOS_APRILTAG), y)
        LITEOS_BASELIB += -lapriltag_demo
    endif
endif

ifeq ($(LOSCFG_DEMOS_AI), y)
    ifeq ($(LOSCFG_ARCH_ARM_CORTEX_A), y)
        LITEOS_BASELIB += -lcortex-a-nnacl
    else ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
        LITEOS_BASELIB += -lcortex-m-nnacl
endif
endif

ifeq ($(LOSCFG_KERNEL_LMS), y)
    LITEOS_BASELIB += -llms
endif

LITEOS_PLATFORM_INCLUDE += $(PLATFORM_INCLUDE)
LITEOS_CXXINCLUDE       += $(PLATFORM_INCLUDE)

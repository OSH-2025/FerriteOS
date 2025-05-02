/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Demo Entry Implementation
 * Author: Huawei LiteOS Team
 * Create: 2020-12-10
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "los_typedef.h"
#include "los_task.h"
#include "fs/fs_init.h"

#ifdef LOSCFG_KERNEL_CPPSUPPORT
#include "los_cppsupport.h"
#endif

#ifdef CONFIG_FEATURE_FOTA
#include "ota_port.h"
#endif /* CONFIG_FEATURE_FOTA */

#ifdef LOSCFG_COMPONENTS_NET_LWIP
#include "sys_init.h"
#endif /* LOSCFG_COMPONENTS_NET_LWIP */

#ifdef LOSCFG_COMPONENTS_NET_AT
#include "at_register.h"
#endif /* LOSCFG_COMPONENTS_NET_AT */


#ifdef LOSCFG_GUI_ENABLE
#include "lvgl_demo.h"
#endif /* LOSCFG_GUI_ENABLE */

#ifdef LOSCFG_DEMOS_AGENT_TINY_MQTT
#include "agent_tiny_mqtt_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_MQTT */

#ifdef LOSCFG_DEMOS_AGENT_TINY_LWM2M
#include "agent_tiny_lwm2m_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_MQTT */

#ifdef LOSCFG_DEMOS_SENSORHUB
#include "sensorhub_demo.h"
#endif /* LOSCFG_DEMOS_AGENT_TINY_LWM2M */

#ifdef LOSCFG_DEMOS_KERNEL
#include "los_inspect_entry.h"
#include "los_demo_entry.h"
#endif /* LOSCFG_DEMOS_KERNEL */

#ifdef LOSCFG_DEMOS_DTLS_SERVER
#include "test_dtls_server.h"
#endif /* LOSCFG_DEMOS_DTLS_SERVER */

#ifdef LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY
#include "nb_demo.h"
#endif /* LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY */

#ifdef LOSCFG_DEMOS_LMS
#include "lms_demo.h"
#endif /* LOSCFG_DEMOS_LMS */

#ifdef LOSCFG_DEMOS_TRACE
#include "trace_demo.h"
#endif /* LOSCFG_DEMOS_TRACE */

#ifdef LOSCFG_SHELL
#include "shell.h"
#include "shcmd.h"
#endif /* LOSCFG_SHELL */

#ifdef LOSCFG_DEMOS_IPV6_CLIENT
#include "client_demo.h"
#endif /* LOSCFG_DEMOS_IPV6_CLIENT */

#ifdef LOSCFG_DEMOS_FS
#include "fs_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AI
#include "ai_demo.h"
#endif

#ifdef LOSCFG_DEMOS_ICONV
#include "iconv_demo.h"
#endif

#ifdef LOSCFG_DEMOS_INIPARSER
#include "iniparser_demo.h"
#endif /* LOSCFG_DEMOS_INIPARSER */

#ifdef LOSCFG_DEMOS_JSON_C
#include "json_c_demo.h"
#endif

#ifdef LOSCFG_DEMOS_JSONCPP
#include "jsoncpp_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBPNG
#include "libpng_demo.h"
#endif

#ifdef LOSCFG_DEMOS_BIDIREFERENCE
#include "bidireference_demo.h"
#endif

#ifdef LOSCFG_DEMOS_FREETYPE
#include "freetype_demo.h"
#endif

#ifdef LOSCFG_DEMOS_HARFBUZZ
#include "harfbuzz_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LUA
#include "lua_demo.h"
#endif

#ifdef LOSCFG_DEMOS_OPUS
#include "opus_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBXML2
#include "libxml2_demo.h"
#endif

#ifdef LOSCFG_DEMOS_CURL
#include "curl_demo.h"
#endif

#ifdef LOSCFG_DEMO_CAN
#include "can_demo.h"
#endif

#ifdef LOSCFG_DEMOS_OPENSSL
#include "openssl_demo.h"
#endif

#ifdef LOSCFG_DEMOS_QRENCODE
#include "qrencode_demo.h"
#endif

#ifdef LOSCFG_DEMOS_PIXMAN
#include "pixman_demo.h"
#endif

#ifdef LOSCFG_DEMOS_SQLITE
#include "sqlite_demo.h"
#endif

#ifdef LOSCFG_DEMOS_OPENEXIF
#include "openexif_demo.h"
#endif

#ifdef LOSCFG_DEMOS_TINYXML2
#include "tinyxml2_demo.h"
#endif

#ifdef LOSCFG_DEMOS_UPNP
#include "upnp_demo.h"
#endif

#ifdef LOSCFG_DEMOS_FASTLZ
#include "fastlz_demo.h"
#endif

#ifdef LOSCFG_DEMOS_PDFGEN
#include "pdfgen_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBRWS
#include "librws_demo.h"
#endif

#ifdef LOSCFG_DEMOS_INIH
#include "inih_demo.h"
#endif

#ifdef LOSCFG_DEMOS_MINMEA
#include "minmea_demo.h"
#endif

#ifdef LOSCFG_DEMOS_HTTP_CLIENT
#include "httpc_demo.h"
#endif

#ifdef LOSCFG_DEMOS_SFUD
#include "sfud_demo.h"
#endif

#ifdef LOSCFG_DEMOS_URIPARSER
#include "uriparser_demo.h"
#endif

#ifdef LOSCFG_DEMOS_C_ALGORITHMS
#include "c_algorithms_demo.h"
#endif

#ifdef LOSCFG_DEMOS_JERRYSCRIPT
#include "jerryscript_demo.h"
#endif

#ifdef LOSCFG_DEMOS_AGRICULTURE
#include "agriculture.h"
#endif

#ifdef LOSCFG_DEMOS_RHASH
#include "rhash_demo.h"
#endif

#ifdef LOSCFG_DEMOS_JSMN
#include "jsmn_demo.h"
#endif

#ifdef LOSCFG_DEMOS_GENANN
#include "genann_demo.h"
#endif

#ifdef LOSCFG_DEMOS_TJPGDEC
#include "tjpgdec_demo.h"
#endif

#ifdef LOSCFG_DEMOS_TINYCRYPT
#include "tinycrypt_demo.h"
#endif

#ifdef LOSCFG_DEMOS_TINYFRAME
#include "tinyframe_demo.h"
#endif

#ifdef LOSCFG_DEMOS_EASYFLASH
#include "ef_demo.h"
#endif

#ifdef LOSCFG_DEMOS_PERF
#include "perf_demo.h"
#endif

#ifdef LOSCFG_DEMOS_LIBEVENT
#include "libevent_demo.h"
#endif

#ifdef LOSCFG_DEMOS_APRILTAG
#include "apriltag_demo.h"
#endif

#ifdef LOSCFG_LWIP_NIP
#include "nip_bupt.h"
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
#define NETWORK_DEMO_TASK_PRIORITY 2
#if defined(CONFIG_FEATURE_FOTA) || defined(LOSCFG_COMPONENTS_CONNECTIVITY_MQTT)
#define NETWORK_DEMO_TASK_SIZE 0x2000
#else
#define NETWORK_DEMO_TASK_SIZE 0x1000
#endif
STATIC UINT32 g_networkTaskId;

STATIC VOID NetworkDemoTaskEntry(VOID)
{
#ifdef LOSCFG_COMPONENTS_NET_LWIP
    hieth_hw_init();
    net_init();
#elif defined (LOSCFG_COMPONENTS_NET_AT)
    AT_DeviceRegister();
#endif

#ifdef LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY
    NBIoT_DemoEntry();
#ifdef CONFIG_FEATURE_FOTA
    hal_init_ota();
#endif /* CONFIG_FEATURE_FOTA */
#endif /* LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY */

#ifdef LOSCFG_DEMOS_AGENT_TINY_MQTT
    AgentTinyMqttDemoEntry();
#endif

#ifdef LOSCFG_DEMOS_AGENT_TINY_LWM2M
    AgentTinyLwm2mDemoEntry();
#endif

#ifdef LOSCFG_DEMOS_LIBRWS
    LibrwsDemo();
#endif

#ifdef LOSCFG_DEMOS_HTTP_CLIENT
    HttpClientDemoTask();
#endif

#ifdef LOSCFG_LWIP_NIP
    NewipCmdTask();
#endif
}

VOID NetworkDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }

    taskInitParam.usTaskPrio = NETWORK_DEMO_TASK_PRIORITY;
    taskInitParam.pcName = "NetworkDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)NetworkDemoTaskEntry;
    taskInitParam.uwStackSize = NETWORK_DEMO_TASK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_networkTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create agenttiny demo task failed.\n");
    }
}
#endif /* LOSCFG_COMPONENTS_NETWORK */

VOID DemoEntry(VOID)
{
#ifndef LOSCFG_KERNEL_SMP
    printf("Hello, welcome to liteos demo!\n");
#endif

#ifdef LOSCFG_SHELL
    (VOID)ShellQueueCreat();
    if (OsShellInit(0) != LOS_OK) {
        printf("Shell init failed.\n");
    }
#endif

#ifdef LOSCFG_COMPONENTS_FS
    FileSystemInit();
#endif

#ifdef LOSCFG_KERNEL_CPPSUPPORT
    extern UINT32 __init_array_start, __init_array_end;
    INT32 ret = LOS_CppSystemInit((UINT32)&__init_array_start, (UINT32)&__init_array_end, NO_SCATTER);
    if (ret != LOS_OK) {
        printf("LiteOS Cpp Init fail.\n");
    }
#endif

#ifdef LOSCFG_DEMOS_JSONCPP
    JsoncppDemoTask();
#endif

#ifdef LOSCFG_DEMOS_OPENEXIF
    OpenExifDemoTask();
#endif

#ifdef LOSCFG_DEMOS_AI
    AiDemoTask();
#endif

#ifdef LOSCFG_GUI_ENABLE
    LvglDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LMS
    LmsDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TRACE
    TraceDemoTask();
#endif

#ifdef LOSCFG_COMPONENTS_NETWORK
    NetworkDemoTask();
#endif

#ifdef LOSCFG_DEMOS_IPV6_CLIENT
    Ipv6DemoTask();
#endif

#ifdef LOSCFG_DEMOS_KERNEL_ENTRY
    KernelDemoTask();
#elif defined(LOSCFG_DEMOS_KERNEL_INSPECT_ENTRY)
    KernelInspectDemoTask();
#endif

#ifdef LOSCFG_DEMOS_FS
    FsDemoTask();
#endif

#ifdef LOSCFG_DEMOS_DTLS_SERVER
    DtlsServerDemoTask();
#endif

#ifdef LOSCFG_DEMOS_SENSORHUB
    SensorHubDemoTask();
#endif

#ifdef LOSCFG_DEMOS_ICONV
    IconvDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LIBPNG
    LibpngDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LUA
    LuaDemoTask();
#endif

#ifdef LOSCFG_DEMOS_BIDIREFERENCE
    BidiRefDemoTask();
#endif

#ifdef LOSCFG_DEMOS_OPUS
    OpusDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LIBXML2
    Libxml2DemoTask();
#endif

#ifdef LOSCFG_DEMOS_INIPARSER
    IniparserDemoTask();
#endif

#ifdef LOSCFG_DEMOS_JSON_C
    JsonDemoTask();
#endif

#ifdef LOSCFG_DEMOS_FREETYPE
    FreeTypeDemoTask();
#endif

#ifdef LOSCFG_DEMOS_HARFBUZZ
    HarfbuzzDemoTask();
#endif

#ifdef LOSCFG_DEMOS_CURL
    CurlDemoTask();
#endif

#ifdef LOSCFG_DEMO_CAN
    CanDemoTask();
#endif

#ifdef LOSCFG_DEMOS_OPENSSL
    OpensslDemoTask();
#endif

#ifdef LOSCFG_DEMOS_QRENCODE
    QrencodeDemoTask();
#endif

#ifdef LOSCFG_DEMOS_PIXMAN
    PixmanDemoTask();
#endif

#ifdef LOSCFG_DEMOS_SQLITE
    SqliteDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TINYXML2
    Tinyxml2DemoTask();
#endif

#ifdef LOSCFG_DEMOS_UPNP
    UpnpDemoTask();
#endif

#ifdef LOSCFG_DEMOS_FASTLZ
    FastlzDemoTask();
#endif

#ifdef LOSCFG_DEMOS_PDFGEN
    PdfgenDemoTask();
#endif

#ifdef LOSCFG_DEMOS_INIH
    InihDemoTask();
#endif

#ifdef LOSCFG_DEMOS_MINMEA
    MinmeaDemoTask();
#endif

#ifdef LOSCFG_DEMOS_SFUD
    SfudDemoTask();
#endif

#ifdef LOSCFG_DEMOS_URIPARSER
    UriparserDemoTask();
#endif

#ifdef LOSCFG_DEMOS_C_ALGORITHMS
    AlgorithmsDemoTask();
#endif

#ifdef LOSCFG_DEMOS_JERRYSCRIPT
    JerryscriptDemoTask();
#endif

#ifdef LOSCFG_DEMOS_AGRICULTURE
    AgricultureDemoTask();
#endif

#ifdef LOSCFG_DEMOS_RHASH
    RhashDemoTask();
#endif

#ifdef LOSCFG_DEMOS_JSMN
    JsmnDemoTask();
#endif

#ifdef LOSCFG_DEMOS_GENANN
    GenannDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TJPGDEC
    TjpgDecDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TINYCRYPT
    TinycryptDemoTask();
#endif

#ifdef LOSCFG_DEMOS_TINYFRAME
    TinyframeDemoTask();
#endif

#ifdef LOSCFG_DEMOS_EASYFLASH
    EasyFlashDemoTask();
#endif

#ifdef LOSCFG_DEMOS_PERF
    PerfDemoTask();
#endif

#ifdef LOSCFG_DEMOS_LIBEVENT
    LibeventDemoTask();
#endif

#ifdef LOSCFG_DEMOS_APRILTAG
    ApriltagDemoTask();
#endif
}

#!/bin/bash

g_sourceFile=""
g_boardName=""

CopySourceFile()
{
    g_sourceFile="$1"
    if [ ${g_sourceFile} = "SMARTL-E802-SDK" ]; then
        rm *.sha256
        mkdir Inc Src
        cp SMARTL-E802-SDK/board/smartl_802_evb/include/* Inc
        cp SMARTL-E802-SDK/csi_core/include/* Inc
        cp SMARTL-E802-SDK/csi_driver/include/* Inc
        cp SMARTL-E802-SDK/csi_driver/smartl/include/* Inc
        cp SMARTL-E802-SDK/csi_driver/smartl/ck_usart.c Src
        cp SMARTL-E802-SDK/csi_driver/smartl/devices.c Src
        cp SMARTL-E802-SDK/csi_driver/smartl/dw_gpio.c Src
        cp SMARTL-E802-SDK/csi_driver/smartl/dw_timer.c Src
        cp SMARTL-E802-SDK/csi_driver/smartl/pinmux.c Src
        cp SMARTL-E802-SDK/csi_driver/smartl/sys_freq.c Src

    fi
    return 0
}

CopySourceFile "$1" "$2"

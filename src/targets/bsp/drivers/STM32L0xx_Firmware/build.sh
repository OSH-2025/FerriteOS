#!/bin/bash

g_sourceFile=""
g_boardName=""

CopySourceFile()
{
    g_sourceFile="$1"
    g_boardName="$2"
    if [ ${g_sourceFile} = "CMSIS" ]; then
        if [ ${g_boardName} = "STM32L073_Nucleo" ]; then
            cp ${g_sourceFile}/Include/stm32l0xx.h      ../../../${g_boardName}/Inc
            cp ${g_sourceFile}/Include/stm32l073xx.h    ../../../${g_boardName}/Inc
            cp ${g_sourceFile}/Include/system_stm32l0xx.h ../../../${g_boardName}/Inc
            cp ${g_sourceFile}/Source/Templates/system_stm32l0xx.c ../../../${g_boardName}/Src
        fi
    fi
    return 0
}

CopySourceFile "$1" "$2"
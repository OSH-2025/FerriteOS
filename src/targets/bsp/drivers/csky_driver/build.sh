#!/bin/bash

g_sourceFile=""
g_boardName=""

CopySourceFile()
{
    g_sourceFile="$1"
    if [ ${g_sourceFile} = "CB2201-CSI-SDK" ]; then
        mv ${g_sourceFile}/CB2201-v1.6.3.zip .
        unzip CB2201-v1.6.3.zip
        rm *.zip
        rm *.sha256
        mkdir Inc Src
        cp CB2201-v1.6.3/board/cb2201/include/* Inc
        cp CB2201-v1.6.3/csi_core/include/* Inc
        cp CB2201-v1.6.3/csi_driver/include/* Inc
        cp CB2201-v1.6.3/csi_driver/ch2201/include/* Inc
        cp CB2201-v1.6.3/csi_driver/ch2201/*.c Src
        rm CB2201-v1.6.3/ -r

    fi
    return 0
}

CopySourceFile "$1" "$2"

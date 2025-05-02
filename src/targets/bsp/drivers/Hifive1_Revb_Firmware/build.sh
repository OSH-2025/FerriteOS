#!/bin/bash

g_sourceFile=""
g_boardName=""

CopySourceFile()
{
    g_sourceFile="$1"
    g_boardName="$2"

    if [ ${g_sourceFile} = "freedom-e-sdk" ]; then
        if [ ! -e freedom-metal/metal/machine.h ]; then
            cp ${g_sourceFile}/bsp/sifive-hifive1-revb/metal.h freedom-metal/metal/machine.h
        fi
        if [ ! -e freedom-metal/metal/machine/inline.h ]; then
            cp ${g_sourceFile}/bsp/sifive-hifive1-revb/metal-inline.h freedom-metal/metal/machine/inline.h
        fi
        if [ ! -e freedom-metal/metal/machine/platform.h ]; then
            cp ${g_sourceFile}/bsp/sifive-hifive1-revb/metal-platform.h freedom-metal/metal/machine/platform.h
        fi
        if [ ! -e ../../../${g_boardName}/liteos.ld ]; then
            cp ${g_sourceFile}/bsp/sifive-hifive1-revb/metal.default.lds ../../../${g_boardName}/liteos.ld
            cp board.patch ../../../${g_boardName}/board.patch
        fi

    elif [ ${g_sourceFile} = "freedom-metal" ]; then
        mkdir ${g_sourceFile}/metal/machine
        if [ -e driver.patch ]; then
            rm driver.patch
        fi
        if [ -e board.patch ]; then
            rm board.patch
        fi
    fi

    return 0
}

CopySourceFile "$1" "$2"
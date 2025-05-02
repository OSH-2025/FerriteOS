#!/bin/bash

g_sourceFile=""
g_boardName=""

OperationSourceFile()
{
    if [ ${g_boardName} = "GD32VF103V_EVAL" ]; then
        cp  ${g_sourceFile}/Firmware/GD32VF103_standard_peripheral/*.h ../../../${g_boardName}/Inc
        cp  ${g_sourceFile}/Firmware/GD32VF103_standard_peripheral/system_gd32vf103.c  ../../../${g_boardName}/Src
        mv  board.patch ../../../${g_boardName} > /dev/null 2>&1 
    fi
    return 0
}

CopySourceFile()
{
    local toolName="`which unrar`"
    local OS=`uname -s`

    g_sourceFile="$1"
    g_boardName="$2"

    if [ $OS != "Linux" ]; then
        if [ ! -e ../../../../../${g_sourceFile} ]; then
            cp ${g_sourceFile}.rar ../../../../../
            echo -e "\033[31m[Info]: unrar failed, Manually extract the package and try again."\
                    " The software package is stored in the same directory as the LiteOS top-level directory.\033[0m"
            return 1
        else
            mv ../../../../../${g_sourceFile} .
            OperationSourceFile
            return 0
        fi
    fi

    if [ -z "$toolName" ]; then
        echo -e "\033[31m[Info]: Program unrar has not been installed. You can use the following" \
        "command to install:\nsudo apt-get install unrar\033[0m"
        return 1
    fi
    
    unrar x ${g_sourceFile}.rar > /dev/null 2>&1
    tmpName="`unrar lb GD32VF103_Firmware_Library.rar | tail -n 1`"
    mv $tmpName ${g_sourceFile} 
    OperationSourceFile

    return 0
}

CopySourceFile "$1" "$2"

#!/bin/bash

g_dirName=""
g_file1="source/bidiref.c"
g_file2="source/bidirefp.h"
g_file3="source/brinput.c"
g_file4="source/brrule.c"
g_file5="source/brtable.c"
g_file6="source/brtest.c"
g_file7="source/brutils.c"
g_file8="include/bidiref.h"

DownloadSourceCode()
{
    i=0
    mkdir ${g_dirName}
    mkdir ${g_dirName}/source
    mkdir ${g_dirName}/include

    while [ $i -lt 8 ]
    do
        let i++
        file="g_file$i"
        fileName=`eval echo '$'${file}`
        curl -BL http://unicode.org/Public/PROGRAMS/BidiReferenceC/8.0.0/${fileName} -o ${g_dirName}/${fileName}
        if [ $? -ne 0 ]; then
            rm -rf ${g_dirName}
            return 1
        fi
    done
    return 0
}

Main()
{
    g_dirName="$1"

    DownloadSourceCode
    if [ $? -ne 0 ]; then
        return 1
    fi
    return 0
}

Main "$1"

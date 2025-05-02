#!/bin/bash

g_componentName=""
g_componentPath=""
g_componentEnableFlag=""
g_componentDownloadAddr=""
g_patchFile="origin.patch"
g_srcSha256File="src.sha256"
g_patchSha256File="patch.sha256"
g_boardName=""
g_boardPatch="board.patch"
g_driverPatch="driver.patch"


ClearDownload()
{
    rm -rf ${g_componentName} > /dev/null 2>&1
    rm -rf ${g_componentName}.zip > /dev/null 2>&1
    rm -rf ${g_componentName}.rar > /dev/null 2>&1
    rm -rf ${g_componentName}.tar.gz > /dev/null 2>&1
    rm ${g_patchFile} ${g_srcSha256File} ${g_patchSha256File} > /dev/null 2>&1
}

ClearDir()
{
    rm -rf ${g_componentName}.zip > /dev/null 2>&1
    rm -rf ${g_componentName}.rar > /dev/null 2>&1
    rm -rf ${g_componentName}.tar.gz > /dev/null 2>&1
    rm ${g_srcSha256File} ${g_patchSha256File} > /dev/null 2>&1
    if [ "${g_componentDownloadAddr}" != "" ]; then
        rm ${g_patchFile} > /dev/null 2>&1
    fi
}

Sha256sumCheck()
{
    local ret=0
    local sha256File=$1
    local targetFile=$2

    if [ $# -lt 2 ]; then
        echo "[Error]: Missing parameter."
        return 1
    fi

    if [ ! -e "${sha256File}" ]; then
        echo "[Error]: $1 file not exist."
        return 1
    fi

    if [ `tail -n1 ${sha256File} | wc -l` -ne 1 ]; then
        echo "" >> ${sha256File}
    fi

    while read line
    do
        if [ "$line" ]; then
            oldSha256sum="`echo "$line" | awk -F ' ' '{print $1}'`"
            fileName="`echo "$line" | awk -F ' ' '{print $2}' | tr -d "\n" | tr -d "\r"`"
            tmpString="`echo "${fileName}" | grep "${targetFile}"`"

            if [ "${fileName}isNull" = "isNull" -o "${tmpString}isNull" = "isNull" ]; then
                continue
            fi

            isGitFile="`echo ${fileName} | grep "${g_componentName}/.git/"`"
            if [ "${isGitFile}" ]; then
                continue
            fi

            fileNameTmp="`sha256sum ${fileName}`"
            if [ $? -ne 0 ]; then
                let ret++
                continue
            fi

            newSha256sum="`echo "${fileNameTmp}" | awk -F ' ' '{print $1}'`"
            # echo "${oldSha256sum}|${newSha256sum}|${fileName}"
            if [ "${oldSha256sum}" != "${newSha256sum}" ]; then
                echo "[Error]: ${fileName} sha256sum check failed! <${oldSha256sum}|${newSha256sum}>"
                let ret++
            fi
        fi
    done < ${sha256File}

    return $ret
}

ComponentFeatures()
{
    local ret=0
    local dirType="`echo "${g_componentPath}" | grep "targets"`"

    cd ${g_componentPath}
    if [ "${dirType}Targets" != "Targets" ]; then
        if [ -f "build.sh" ]; then
            sh build.sh "${g_componentName}" "${g_boardName}"
            ret=$?
        fi
    fi
    cd -
    return $ret
}

ApplyPatch()
{
    local patchFile="$1"
    local filePath="$2"

    cd ${filePath}
    if [ -e ${patchFile} ]; then
        echo "[Info]: Apply patch."
        patch -p1 -N --binary < ${patchFile}
        if [ $? -ne 0 ]; then
            ClearDownload
            echo -e "\033[31m[Error]:\033[0m Apply patch failed!"
        else
            ClearDir
            echo "[Info]: Apply patch success."
        fi
    fi
    ClearDir
    cd -
    return 0
}

CheckPatchFile()
{
    local i=0
    local patchName=$1
    local sha256Ret=0

    cd -
    cd ${g_componentPath}
    while [ ${i} -lt $# ]
    do
        let i++
        patchName="`eval echo '$'${i}`"
        if [ -f ${patchName} ]; then 
            Sha256sumCheck ${g_patchSha256File} ${patchName}
            sha256Ret=`expr ${sha256Ret} + $?`
        fi
    done
    cd -
    cd ${g_componentPath}/LiteOS_Components
    return ${sha256Ret}
}

CopyFile()
{
    local fileName="$1"
    local ToPath="$2"

    if [ -e ${fileName} ]; then
        cp -rf ${fileName} ${ToPath}
    fi
    return 0
}

DownloadPatchAndSha256File()
{
    local iNum=3
    local sha256Ret=0
    local originPatchFile="${g_componentPath}/${g_patchFile}"
    local boardPatchFile="${g_componentPath}/${g_boardPatch}"
    local driverPatchFile="${g_componentPath}/${g_driverPatch}"
    local srcSha256File="${g_componentPath}/${g_srcSha256File}"
    local patchsha256File="${g_componentPath}/${g_patchSha256File}"

    cd ${g_componentPath}
    if [ -e "LiteOS_Components" ]; then
        rm -rf LiteOS_Components
    fi
    mkdir LiteOS_Components
    cd -
    cd ${g_componentPath}/LiteOS_Components

    git init
    git config --local core.autocrlf input
    git config core.sparseCheckout true
    echo ${g_componentPath} >> .git/info/sparse-checkout
    git remote add origin http://gitee.com/LiteOS/LiteOS_Components.git
    echo "[Info]: Start download ${g_componentName} patch and sha256 file."

    while [ ${iNum} -gt 0 ]
    do
        sha256Ret=0
        let iNum--
        git pull origin master
        if [ $? -ne 0 ]; then
            echo "[Info]: Download failed or timeout, Trying to download again."
        else
            if [ ! -e ${g_componentPath} ]; then      # The resource does not exist
                echo "[Error]: ${g_componentPath} does not exist on the website."
                break
            fi

            CopyFile "${srcSha256File}"   "../"
            CopyFile "${boardPatchFile}"  "../"
            CopyFile "${patchsha256File}" "../"
            CopyFile "${originPatchFile}" "../"
            CopyFile "${driverPatchFile}" "../"

            CheckPatchFile "${g_patchFile}" "${g_driverPatch}" "${g_boardPatch}"
            sha256Ret=`expr ${sha256Ret} + $?`
            if [ $sha256Ret -ne 0 ]; then
                echo "[Error]: Patch file sha256sum check failed!"
            else
                break
            fi
        fi
    done

    git config core.sparseCheckout false
    rm -rf ../LiteOS_Components
    if [ $iNum -le 0 ]; then            # download fail
        echo "[Error]: Download ${g_componentName} patch and sha256 file failed!"
        rm ../${g_patchFile} ../${g_srcSha256File} ../${g_patchSha256File} > /dev/null 2>&1
        cd -
        return 1
    fi
    echo "[Info]: Download ${g_componentName} patch and sha256 file success."
    cd -
    return 0
}

DownloadSourceCode()
{
    local iNum=3
    local sha256Ret=0
    local packageType="`echo "${g_componentDownloadAddr}" | awk -F ' ' '{print $1}'`"
    local packageType1="`echo ${g_componentDownloadAddr:0-4:4}`"
    local packageType2="`echo ${g_componentDownloadAddr:0-7:7}`"

    echo "[Info]: Start download ${g_componentName} source code."
    while [ ${iNum} -gt 0 ]
    do
        let iNum--
        if [ -e ${g_componentName} ]; then
            rm -rf ${g_componentName}
        fi

        # get source code
        if [ "${packageType1}" = ".zip" ]; then
            curl -BL --insecure ${g_componentDownloadAddr} -o ${g_componentName}.zip
        elif [ "${packageType2}" = ".tar.gz" ]; then
            curl -BL --insecure ${g_componentDownloadAddr} -o ${g_componentName}.tar.gz
        elif [ "${packageType1}" = ".rar" ]; then
            curl -BL --insecure ${g_componentDownloadAddr} -o ${g_componentName}.rar
        elif [ "${packageType}" = "git" -o "${packageType1}" = ".git" ]; then
            git config --global core.autocrlf input
            ${g_componentDownloadAddr} ${g_componentName}
        elif [ "${packageType}" = "sh" ]; then
            ${g_componentDownloadAddr}
        else
            echo "[Info]: Download method support."
            iNum=0
            break
        fi

        if [ $? -ne 0 ]; then                   # download fail
            echo "[Info]: Download failed or timeout, Trying to download again."
        else
            Sha256sumCheck ${g_srcSha256File} ${g_componentName}    # download success
            sha256Ret=$?
            if [ $sha256Ret -ne 0 ]; then
                echo "[Error]: sha256sum check fails for ${sha256Ret} files in ${g_componentName}!"
            else
                break
            fi
        fi
    done

    if [ $iNum -le 0 ]; then
        echo -e "\033[31m[Error]:\033[0m Download ${g_componentName} source code failed!"
        ClearDownload
        return 1
    fi
    echo "[Info]: Download ${g_componentName} source code success."
    return 0
}

GetSourceCode()
{
    local ret=0
    local i=0
    local packageType=("zip" "tar.gz" "rar")

    if [ "${g_componentDownloadAddr}" = "" ]; then
        return 0
    fi

    cd ${g_componentPath}
    while [ $i -lt ${#packageType[@]} ]
    do
        if [ -f "${g_componentName}.${packageType[i]}" ]; then
            Sha256sumCheck ${g_srcSha256File} ${g_componentName}
            ret=$?
            if [ $ret -ne 0 ]; then
                rm -rf ${g_componentName}.${packageType[i]}
                DownloadSourceCode
                ret=$?
            fi
            break
        fi
        let i++
    done

    if [ $i -eq ${#packageType[@]} ]; then
        DownloadSourceCode
        ret=$?
    fi

    if [ $ret -eq 0 -a -f "${g_componentName}.zip" ]; then
        unzip "${g_componentName}.zip" > /dev/null 2>&1 && ret=$?
        tmpName="`unzip -v ${g_componentName}.zip | grep 'Stored' | awk '{print $8}' | head -n 1 | tr -d "/"`"
    elif [ $ret -eq 0 -a -f "${g_componentName}.tar.gz" ]; then
        tar -xzvf "${g_componentName}.tar.gz" > /dev/null 2>&1 && ret=$?
        tmpName="`tar -tf ${g_componentName}.tar.gz | head -n 1 | tr -d "/"`"
    fi

    if [ -d "${tmpName}" -a "${tmpName}" != "${g_componentName}" ]; then
        mv ${tmpName} ${g_componentName}
    fi

    cd -
    return $ret
}

ComponentsIfEnable()
{
    isEnable="NotEnable`cat .config | grep -w "${g_componentEnableFlag}"`"
    fileIsExist="${g_componentPath}/${g_componentName}"

    if [ ${isEnable} != "NotEnable" ]; then
        if [ -d ${fileIsExist} ]; then
            if [ "`ls ${fileIsExist}`" ]; then
                echo "[Info]: ${fileIsExist} is exist."
                return 0
            fi
        elif [ -f ${fileIsExist} ]; then
            echo "[Info]: ${fileIsExist} is exist."
            return 0
        fi

        DownloadPatchAndSha256File
        if [ $? -ne 0 ]; then
            return 1
        fi

        GetSourceCode ${g_componentName}
        if [ $? -ne 0 ]; then
            return 1
        fi

        ComponentFeatures
        if [ $? -ne 0 ]; then
            return 1
        fi

        local dirType="`echo "${g_componentPath}" | grep "targets"`"
        if [ "${dirType}Targets" != "Targets" ]; then
            ApplyPatch ${g_driverPatch} ${g_componentPath}
            ApplyPatch ${g_boardPatch} targets/${g_boardName}
        else
            ApplyPatch ${g_patchFile} ${g_componentPath} 
        fi
    fi

    return 0
}

ParseComponentsInfo()
{
    g_componentName=`echo $1 | awk -F '&#&' '{print $1}' | sed -e 's/^[ \t]*//g' -e 's/[ \t]*$//g'`
    g_componentPath=`echo $1 | awk -F '&#&' '{print $2}' | sed -e 's/^[ \t]*//g' -e 's/[ \t]*$//g'`
    g_componentEnableFlag=`echo $1 | awk -F '&#&' '{print $3}' | sed -e 's/^[ \t]*//g' -e 's/[ \t]*$//g'`
    g_componentDownloadAddr=`echo $1 | awk -F '&#&' '{print $4}' | sed -e 's/^[ \t]*//g' -e 's/[ \t]*$//g' | tr -d "\n" | tr -d "\r"`
    g_boardName=`echo $1 | awk -F '&#&' '{print $5}' | sed -e 's/^[ \t]*//g' -e 's/[ \t]*$//g' | tr -d "\n" | tr -d "\r"`

    if [ -z "${g_componentPath}" -o -z "${g_componentName}" ]; then
        return 1
    fi

    if [ ! -d "${g_componentPath}" ]; then
        return 1
    fi

    # echo "[Info]: ${g_componentName} ${g_componentPath} ${g_componentEnableFlag} ${g_componentDownloadAddr}"
    ComponentsIfEnable ${g_componentName} ${g_componentPath} ${g_componentEnableFlag} ${g_componentDownloadAddr}
    return 0
}

ReadComponentsInfo()
{
    local count=0
    if [ ! -f $1 ]; then
        echo "[Error]: The $1 file does not exist or the path is incorrect!"
        return 1
    fi

    if [ `tail -n1 $1 | wc -l` -ne 1 ]; then
        echo "" >> $1
    fi

    while read line
    do
        let count++
        if [ "NULL$line" != "NULL" ]; then
            ParseComponentsInfo "$line"
        fi
    done < $1
    return 0
}

CheckToolIsExist() 
{
    local i=0
    local ret=0

    while [ ${i} -lt $# ]
    do
        let i++
        toolName="`eval echo '$'${i}`"
        toolPath="`which $toolName`"
        if [ -z "${toolPath}" ]; then
            let ret++
            echo -e "\033[31m[Info]: The '${toolName}' tool needs to be installed.\033[0m"
        fi
    done
    return $ret
}

Main()
{
    CheckToolIsExist "patch" "sha256sum" "curl" "unzip"
    if [ $? -ne 0 ]; then
        return 1
    fi
    ReadComponentsInfo "components/online_components"
}

Main


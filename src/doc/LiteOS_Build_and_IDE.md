# Huawei LiteOS 编译和开发工具

## 目 录

-   [配置&编译框架简介](#1)
-   [Huawei LiteOS开发工具介绍](#2)
-   [Linux下配置工具的使用](#3)
-   [Linux下的编译](#4)
    -   [搭建Linux编译环境](#4-1)
    -   [Linux下编译流程](#Linux下编译流程)
-   [Windows下的编译](#5)
    -   [搭建Windows编译环境](#5-1)
    -   [Windows下编译流程](#5-2)

<h2 id="1">编译框架简介</h2>

Huawei LiteOS使用Kconfig文件配置系统，基于GCC/Makefile实现组件化编译。

不论是Linux下使用make menuconfig命令配置系统，还是Windows下使用Huawei LiteOS Studio进行图形化配置，Huawei LiteOS都会同时解析、展示根目录下的.config文件和tools/menuconfig/config.in文件（该文件包含了各个模块的Kconfig文件），同时在开发板的include文件夹下生成menuconfig.h。config.in文件由Kconfig语言（一种菜单配置语言）编写而成。config.in文件决定了要展示的配置项，.config文件决定了各个配置项的默认值。

Huawei LiteOS通过在根目录下执行make命令完成自动化编译整个工程。对于根目录下的Makefile文件，其中包含了config.mk，config.mk又包含了los\_config.mk，而los\_config.mk则包含了各个模块的Makefile和.config文件，从而定义了对整个工程的编译链接规则。

![](figures/build_and_IDE/Makefile_inclusion_relationship.png)

Huawei LiteOS目前支持Windows及Linux平台的配置&编译。

-   对于Windows平台，提供了Huawei LiteOS Studio图形化IDE，用户可直接在 Studio上完成配置&编译。
-   对于Linux平台，通过menuconfig进行组件化配置及裁剪后，执行make命令完成编译。

<h2 id="2">Huawei LiteOS开发工具介绍</h2>

Huawei LiteOS Studio是Huawei LiteOS提供的一款Windows下的图形化开发工具。它以Visual Studio Code的社区开源代码为基础，根据C语言编程特点、Huawei LiteOS嵌入式系统软件的业务场景开发的工具。它提供了代码编辑、组件配置、编译、烧录、调试等功能，可以对系统关键数据进行实时跟踪、保存与回放。关于这款工具的详细使用介绍，请访问<a href="https://liteos.gitee.io/liteos_studio/#/" target="_blank">LiteOS Studio官方网站</a>。

<h2 id="3">Linux下配置工具的使用</h2>

在Huawei\_LiteOS根目录下执行make menuconfig命令会展示图形化配置界面，用户可以根据需要自行裁剪模块或者修改设置。执行完保存菜单退出，该命令会更新根目录下.config文件。

menuconfig的使用方式，主要包括：

-   上下键：选择不同的行，即移动到不同的选项上。
-   空格键/回车：用于开启或关闭选项。
    1.  开启选项：对应的选项前面会显示“ \[ \* \]“，括号里面有一个星号，表示已经开启该选项。
    2.  关闭选项：对应的选项前面只显示中括号“\[  \]”, 括号里面是空。
    3.  如果选项后面有三个短横线加上一个右箭头，即---\>，表示此项下面还有子选项，输入空格键/回车后可以进入子菜单。
-   ESC键：返回上一级菜单，或退出menuconfig并提示保存。
-   问号? ：展示配置项的帮助信息。
-   斜线/ :  进入搜索配置项界面，支持配置项的搜索。
-   字母F：进入帮助模式，在界面下方会显示配置项的帮助信息，再次输入字母F可以退出此模式。
-   字母C：进入name模式，在此模式下，会显示配置项对应的宏定义开关，再次输入字母C可以退出此模式。
-   字母A：进入all模式，在此模式下，会展开显示菜单中的所有子选项，再次输入字母A可以退出此模式。
-   字母S : 保存配置项。
-   字母Q：退出menuconfig并提示保存。

>![](public_sys-resources/icon-note.gif) **说明：** 
>-   以上字母，不区分大小写。
>-   可以通过斜线/进入搜索界面，在输入栏中输入要搜索的配置项宏定义，例如搜索“LOSCFG\_BASE\_IPC\_SEM”，输入该宏定义后，会自动联系出匹配到此关键字的配置项，选中所需要的配置项，按回车键即可进入。
>    ![](figures/build_and_IDE/search_macro_in_menuconfig.png)

<h2 id="4">Linux下的编译</h2>

<h3 id="4-1">搭建Linux编译环境</h3>

#### 软件要求

-   Ubuntu Version 14.04及以上版本，作为编译Huawei LiteOS的服务器。
-   GNU Arm Embedded Toolchain编译器，用于代码编译。
-   GNU Make构建器，用于文件组织与链接。
-   python 2.7/3.2+，pip包管理工具，kconfiglib库，用于编译前通过图形化界面完成配置。

#### 安装编译器&构建器

-   安装GNU Arm Embedded Toolchain编译器。
    1.  下载编译器。

        - 对于32位芯片架构，官方下载地址为：<a href="https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads" target="_blank">GNU Arm Embedded Toolchain编译器</a>，建议使用2019-q4-major及以上版本。
        - 对于64位芯片架构，官方下载地址为：<a href="https://www.linaro.org/downloads/" target="_blank">64位GNU Arm Embedded Toolchain编译器</a>，建议使用最新版本的aarch64-linux-gnu编译器。

    2.  解压编译器。

        可以参考如下命令完成解压，将压缩包名替换为实际下载的软件包名：

        ```
        tar -xvf gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2
        ```

        解压后可以得到文件夹gcc-arm-none-eabi-9-2019-q4-major。

    3.  添加编译器的执行路径到环境变量。

        以第二步解压的编译器为例，将gcc-arm-none-eabi-9-2019-q4-major/bin目录添加到环境变量中，可以编辑\~/.bashrc文件，参考如下方法设置PATH环境变量：

        ```
        export PATH=$PATH:YOUR_PATH/gcc-arm-none-eabi-9-2019-q4-major/bin/
        ```

        然后执行以下命令使新设置的环境变量立即生效：

        ```
        source ~/.bashrc
        ```

-   升级GNU Make构建器到最新版。
    1.  通过官网下载最新<a href="http://ftp.gnu.org/pub/gnu/make/?C=M;O=D" target="_blank">Make构建器</a>。
    2.  参考如下命令完成解压，将压缩包名替换为实际下载的软件包名：

        ```
        tar -xf make-4.3.tar.gz
        ```

    3.  检查依赖。

        解压后进入到目录中，执行./configure命令以检查编译与安装Make构建器所需的依赖：

        ```
        cd make-4.3
        ./configure
        ```

        如果没有报错就继续下一步操作，如果存在报错就根据提示安装依赖。

    4.  编译&安装Make。

        继续在当前目录下，参考如下命令完成Make构建器的编译与安装：

        ```
        sh build.sh
        sudo make
        sudo make install
        ```


#### 安装图形化配置解析工具
-   安装python 2.7/3.2+。

    下面以python3.8为例介绍安装方法。

    -   命令行方式安装：

        ```
        sudo apt-get install python3.8
        ```

    -   源码包编译安装：
        1.  通过官网下载<a href="https://www.python.org/ftp/python/3.8.5/Python-3.8.5.tgz" target="_blank">python源码包</a>。
        2.  解压源码包。

            参考如下命令完成解压，将压缩包名替换为实际下载的源码包名：

            ```
            tar -xf Python-3.8.5.tgz
            ```

        3.  检查依赖。

            解压后进入到目录中，执行./configure命令以检查编译与安装python所需的依赖：

            ```
            cd Python-3.8.5
            ./configure
            ```

            如果没有报错就继续下一步操作，如果存在报错就根据提示安装依赖。

        4.  编译&安装python。

            ```
            sudo make
            sudo make install
            ```

        5.  检查python版本并正确链接python命令。

            ```
            python --version
            ```

            如果显示的不是刚刚安装的python版本，则需要执行以下命令来正确链接python命令。

            a.  获取python目录，例如对于python 3.8.5，执行如下命令。

                ```
                which python3.8
                ```

            b.  链接python命令到刚刚安装的python包。

                将以下命令中的 "python3.8-path" 替换为 "which python3.8" 命令执行后的回显路径：

                ```
                cd /usr/bin && sudo rm python && sudo ln -s "python3.8-path" python
                ```

            c.  再次检查python版本。

                ```
                python --version
                ```

-   安装pip包管理工具。

    如果pip命令不存在，可以下载pip源码包进行安装。pip依赖setuptools，如果setuptools不存在，也需要安装。

    -   命令行方式安装：

        ```
        sudo apt-get install python3-setuptools python3-pip -y
        sudo pip3 install --upgrade pip
        ```

    -   源码包方式安装：
        1.  安装setuptools。

            点击<a href="https://pypi.org/project/setuptools/" target="_blank">setuptools源代码包下载地址</a>，可以参考下面的命令进行安装：

            ```
            sudo unzip setuptools-50.3.2.zip
            cd setuptools
            sudo python setup.py install
            ```

            >![](public_sys-resources/icon-notice.gif) **须知：** 
            >setuptools最新版本不支持python 2.7，如果使用python 2.7，请下载<a href="https://pypi.org/project/setuptools/45.0.0/#files" target="_blank">setuptools 45.0.0版本</a>以支持python 2.7。

        2.  安装pip。

            点击<a href="https://pypi.org/project/pip/" target="_blank">pip源代码包下载地址</a>，可以参考下面的命令进行安装：

            ```
            sudo tar -xf pip-20.2.4.tar.gz
            cd pip-20.2.4
            sudo python setup.py install
            ```

-   安装kconfiglib库。
    -   对于服务器可以联网的情况。

        可以直接使用如下命令安装kconfiglib：

        ```
        sudo pip install kconfiglib
        ```

    -   对于服务器不能联网的情况。

        可以采用离线的方式安装。首先在其他能联网的环境上<a href="https://pypi.org/project/kconfiglib/#files" target="_blank">下载kconfiglib</a>，可以下载kconfiglib的wheel文件kconfiglib-14.1.0-py2.py3-none-any.whl或源代码文件kconfiglib-14.1.0.tar.gz，这里以14.1.0版本为例。

        a.   wheel文件的安装，可以参考如下命令：

            ```
            sudo pip install kconfiglib-14.1.0-py2.py3-none-any.whl
            ```

        b.   源代码文件的安装，可以参考如下命令：

            ```
            sudo tar -zxvf kconfiglib-14.1.0.tar.gz
            cd kconfiglib-14.1.0
            sudo python setup.py install
            ```

### Linux下编译流程

参照[搭建Linux编译环境](#4-1)完成环境搭建后，即可按以下步骤完成编译。

1.  下载Huawei LiteOS代码。

    请下载完整的Huawei LiteOS代码，代码仓在gitee上，请选择master分支进行下载：<a href="https://gitee.com/LiteOS/LiteOS" target="_blank">LiteOS代码仓</a>。

2.  拷贝开发板配置文件为根目录.config文件。

    根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件$\{platform\}.config到根目录，并重命名为.config。

3.  配置系统（可选）。

    如果不希望使用系统的默认配置，可以在Huawei\_LiteOS根目录下执行**make menuconfig**命令，在图形化配置界面中自行裁剪模块或修改配置。修改完保存菜单退出，其修改默认会保存到根目录下.config文件中。

4.  清理工程。

    在编译前，先在Huawei\_LiteOS根目录下执行**make clean**命令删除以前编译出的二进制文件。

5.  编译工程。

    在Huawei\_LiteOS根目录下执行**make**命令即可完成工程编译，编译结果会在屏幕上输出。

    以Cloud\_STM32F429IGTx\_FIRE为例，生成的系统镜像文件、反汇编等文件保存在out/Cloud\_STM32F429IGTx\_FIRE目录中，库文件在out/Cloud\_STM32F429IGTx\_FIRE/lib目录中，中间文件在out/Cloud\_STM32F429IGTx\_FIRE/obj目录中。


>![](public_sys-resources/icon-note.gif) **说明：** 
>如果只编译某个模块，而不是整个工程代码，**在完成该模块的配置后**，参考如下步骤完成编译。
>1.  导出LITEOSTOPDIR环境变量。
>    LITEOSTOPDIR变量表示Huawei LiteOS代码的保存路径，例如代码路径为“/usr1/mypc/Huawei\_LiteOS”，则执行如下命令：
>    ```
>    export LITEOSTOPDIR=/usr1/mypc/Huawei_LiteOS
>    ```
>2.  编译模块。
>    进入要编译的模块目录，执行make命令。


<h2 id="5">Windows下的编译</h2>

<h3 id="5-1">搭建Windows编译环境</h3>

Windows下的编译可以使用Huawei LiteOS Studio图形化IDE。

1.  下载Huawei LiteOS Studio。

    访问<a href="https://gitee.com/LiteOS/LiteOS_Studio/releases/" target="_blank">Huawei LiteOS Studio下载链接</a>获取最新发行版。

2.  搭建Huawei LiteOS Studio开发环境。

    参考<a href="https://liteos.gitee.io/liteos_studio/#/install" target="_blank">Huawei LiteOS Studio安装指南</a>，完成开发环境的搭建。

<h3 id="5-2">Windows下编译流程</h3>

参考<a href="https://liteos.gitee.io/liteos_studio/#/project_stm32?id=%e4%bd%bf%e7%94%a8%e5%85%a5%e9%97%a8" target="_blank">Huawei LiteOS Studio使用入门</a>，完成Huawei LiteOS工程编译。IDE中各个界面介绍可以参考<a href="https://liteos.gitee.io/liteos_studio/#/introduction" target="_blank">HUAWEI LiteOS Studio界面介绍</a>。
# QEMU（C-SKY 教程）

## 1.简介

QEMU可以模拟内核运行在不同的单板，解除对物理开发板的依赖。`SmartL_E802/`子目录包含部分Qemu C-SKY虚拟化平台验证的LiteOS的代码，通过它来模拟一个基于C-SKY架构的单板。

## 2.环境搭建

   1. 编译工具链安装

      a) 创建`csky_toolchain`文件夹并进入

         ```shell
         $ mkdir csky_toolchain && cd csky_toolchain
         ```

      b) 下载csky-elfabiv2-tools-x86_64-minilibc-20210423.tar.gz 到`csky_toolchain`文件夹并解压

         ```shell
         $ wget https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource/1356021/1619529111421/csky-elfabiv2-tools-x86_64-minilibc-20210423.tar.gz
         $ tar -xf csky-elfabiv2-tools-x86_64-minilibc-20210423.tar.gz
         ```

      c) 将csky-elfabiv2编译工具链加入环境变量(将user_toolchain_xxx_path修改为自己的安装路径)

         ```shell
         $ vim ~/.bashrc
         $ export PATH=$PATH:user_toolchain_xxx_path/csky_toolchain/bin
         $ source ~/.bashrc
         ```

   2. qemu安装

      a) 创建`csky_qemu`文件夹并进入

         ```shell
         $ mkdir csky_qemu && cd csky_qemu
         ```

      b) 下载csky-qemu-x86_64-Ubuntu-16.04-20210202-1445.tar.gz到`csky_qemu`文件夹下并解压

         ```shell
         $ wget https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource/1356021/1612269502091/csky-qemu-x86_64-Ubuntu-16.04-20210202-1445.tar.gz
         $ tar -xf csky-qemu-x86_64-Ubuntu-16.04-20210202-1445.tar.gz
         ```

      c) 将qemu加入环境变量(将user_qemu_xxx_path修改为自己的安装路径)

         ```shell
         $ vim ~/.bashrc
         export PATH=$PATH:user_qemu_xxx_path/csky-qemu/bin
         $ source ~/.bashrc
         ```

      d) 安装依赖

         ```shell
         $ ldd qemu_installation_path/bin/qemu-system-cskyv2
         ```

         根据ldd执行结果，安装缺少的依赖库

         (注：更多使用安装指导，请参考官方指南：https://occ.t-head.cn/community/download?id=636946310057951232)

## 3. 编译LiteOS SmartL_E802工程

   从<a href="https://gitee.com/LiteOS/LiteOS.git" target="_blank"> LiteOS 仓库 </a>拉取LiteOS源码：

   ```shell
   $ mkdir ~/SmartL_E802
   $ cd ~/SmartL_E802
   $ git clone https://gitee.com/LiteOS/LiteOS.git
   ```

   进入 LiteOS 工程，拷贝对应`SmartL_E802/`的配置文件替换当前配置文件：

   ```shell
   $ cd ~/SmartL_E802/LiteOS
   $ cp tools/build/config/SmartL_E802.config .config
   ```

   编译LiteOS SmartL_E802工程：
   ```shell
   $ make clean && make -j
   ```
   编译完成后会在 out/SmartL_E802/ 路径下生成`Huawei_LiteOS.elf/`文件，该文件即为SmartL_E802的执行文件。

## 4.在Qemu中运行镜像

   1. 启动qemu(不配合GDB)

      ```
      qemu-system-cskyv2 -machine smartl -kernel Huawei_LiteOS.elf -nographic
      ```
      * 注: `-machine/` - 选择模拟的开发板; `-kernel/` - 需运行的程序; `-nographic/` - 禁止所有的图形输出，模拟的串口将会重定向到命令行。

   2. 启动qemu(配合GDB)

      a)启动GDB服务器，等待连接

      ```
      qemu-system-cskyv2 -machine smartl -kernel Huawei_LiteOS.elf -nographic -s -S
      ```
   
      b)GDB调试

      ```
      $ csky-abiv2-elf-gdb liteos.elf -ex "target remote :1234"
      ```

      * 注: qemu退出方式为：按下ctrl加a键，然后松开再按下x键。

   3. 运行结果

      ```
      ********Hello Huawei LiteOS********

      LiteOS Kernel Version : 5.1.0
      Processor  : c-sky
      Run Mode   : UP
      GIC Rev    : VIC
      build time : Oct 20 2021 11:02:32

      **********************************

      main core booting up...
      OsAppInit
      cpu 0 entering scheduler
      app init!
      Hello, welcome to liteos demo!

      Huawei LiteOS # 
      ```

## 5. shell 命令

  输入命令 help 查看支持的 shell 命令
  
  ```
  Huawei LiteOS # help
  *******************shell commands:*************************

  cpup          date          free          help          hwi           memcheck      swtmr         systeminfo    task          uname         watch         

  Huawei LiteOS # 
  ```

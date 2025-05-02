# QEMU（ESP32 教程）

## 1.简介

QEMU可以模拟内核运行在不同的单板，解除对物理开发板的依赖。`ESP32/`子目录包含部分Qemu Xtensa虚拟化平台验证的LiteOS的代码，通过它来模拟一个基于xtensa架构的单板。


## 2.环境搭建

   1. esp-idf环境安装

      使用安装指导请参考：(https://docs.espressif.com/projects/esp-idf/zh_CN/release-v4.1/get-started/index.html)

   2. esptool工具安装

      ```shell
      python -m pip install esptool
      ```

   3. qemu-system-xtensa安装

      a) 编译安装

         ```shell
         $ git clone https://github.com/espressif/qemu.git
         $ cd qemu
         $ ./configure --target-list=xtensa-softmmu \
            --enable-gcrypt \
            --enable-debug --enable-sanitizers \
            --disable-strip --disable-user \
            --disable-capstone --disable-vnc \
            --disable-sdl --disable-gtk
         ```

      b) 等待编译结束，执行安装命令（如果编译失败请参考https://github.com/espressif/qemu/issues/21）:

         ```shell
         $ ninja -C build
         ```

      c) 将qemu添加到环境变量中(user_qemu_xxx_path修改为自己的安装路径):

         ```shell
         $ vim ~/.bashrc
         $ export QEMU=user_qemu_xxx_path/qemu/build
         $ source ~/.bashrc
         ```

      d) 安装依赖

         ```shell
         $ ldd $QEMU/qemu-system-xtensa
         ```

         根据ldd执行结果，安装缺少的依赖库

         (注：更多安装指导，请参考链接：[Home · espressif/qemu Wiki · GitHub](https://github.com/espressif/qemu/wiki#configure))

## 3. 编译LiteOS ESP32工程

   从<a href="https://gitee.com/LiteOS/LiteOS.git" target="_blank"> LiteOS 仓库 </a>拉取LiteOS源码：

   ```shell
   $ mkdir ~/esp32
   $ cd ~/esp32
   $ git clone https://gitee.com/LiteOS/LiteOS.git
   ```

   进入 LiteOS 工程，拷贝对应`ESP32/`的配置文件替换当前配置文件：

   ```shell
   $ cd ~/esp32/LiteOS
   $ cp tools/build/config/ESP32_QEMU.config .config
   ```

   编译LiteOS ESP32工程：

   ```shell
   $ make clean && make -j
   ```

   编译完成后会在 out/ESP32/ 路径下生成`Huawei_LiteOS.elf`文件，该文件即为ESP32的执行文件。

## 4.在Qemu中运行镜像

   1. 设置QEMU_XTENSA_CORE_REGS_ONLY环境变量

      ```shell
      $ export QEMU_XTENSA_CORE_REGS_ONLY=1
      ```

   2. 使用esptool.py的merge_bin命令，将使用esp-idf编译的bootloader.bin，partition-table.bin以及LiteOS下编译生成的Huawei_LiteOS.bin合成一个Huawei_LiteOS_merge.bin

      ```shell
      $ esptool.py --chip esp32 merge_bin --fill-flash-size 4MB -o out/ESP32/Huawei_LiteOS_merge.bin 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 out/ESP32/Huawei_LiteOS.bin
      ```

      注：需要先使用esp-idf的任意例程编译生成bootloader.bin和partition-table.bin并拷贝到LiteOS目录下。

   3. 启动qemu(不配合GDB)

      ```shell
      $ $QEMU/qemu-system-xtensa -nographic -machine esp32 -drive file=out/ESP32/Huawei_LiteOS_merge.bin,if=mtd,format=raw
      ```

      注：由于默认安装的qemu自带qemu-system-xtensa工具与当前安装的qemu-system-xtensa工具重名，因此采用绝对路径执行当前的qemu-system-xtensa工具。

   4. 启动qemu(配合GDB)

      a) 启动GDB服务器，等待连接

      ```shell
      $ $QEMU/qemu-system-xtensa -nographic -s -S -machine esp32 -drive file=out/ESP32/Huawei_LiteOS_merge.bin,if=mtd,format=raw
      ```

      b) 新建终端并使用GDB连接qemu
      
      ```shell
      $ xtensa-esp32-elf-gdb out/ESP32/Huawei_LiteOS.elf -ex "target remote :1234"
      ```

   注：qemu退出方式为：按下ctrl加a键，然后松开再按下x键。

(注：更多操作指导，请参考：[Home · espressif/qemu Wiki · GitHub](https://github.com/espressif/qemu/wiki#configure))

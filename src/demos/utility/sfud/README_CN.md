# Sfud Demo说明文档
`Sfud`是一个开源 SPI FLASH 驱动库，该库集合了常用的flash型号参数表，通过查找`内置的常用flash参数表`或`SFDP参数表`，实现对flash的读写，擦除等操作。

## 使能 Sfud Demo

1. 以`Linux`系统编译环境和`Cloud_STM32F429IGTx_FIRE`开发板为例，拷贝`Cloud_STM32F429IGTx_FIRE`开发板的默认配置文件`${platform}.config`到在`LiteOS`工程的根目录下，并重命名为`.config`。  
```
   cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
```

2. 在`LiteOS`工程的根目录下执行`make menuconfig`命令，并勾选以下相关配置，使能`Sfud Demo`：  

```
   Demos  --->
        Utility Demo  --->
               [*] Enable Sfud Demo
```

3. 勾选`Sfud Demo`后，自动使能其依赖的`sfud`组件。保存并退出`menuconfig`后，自动执行`components/download.sh`脚本。  
* 从互联网下载`sfud-master`源代码。  
* 从`gitee`的`LiteOS_Components`组件仓库下载对应的patch包，并把该patch打入源码中。  

关于组件自动下载的详细流程，请查阅`components/download.sh`脚本。  

**注意**：下载组件时，`Linux`环境可以访问互联网，并已安装`git`工具。  

## 编译运行 Sfud Demo
1. 在`LiteOS`工程根目录下执行以下命令进行编译：  
```
make clean ; make -j
```
2. 编译结束后，在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成`libsfud.a`和`libsfud_demo.a`，系统镜像文件为out/Cloud_STM32F429IGTx_FIRE/`Huawei_LiteOS.bin`。  
3. 烧录系统镜像文件到`Cloud_STM32F429IGTx_FIRE`开发板，上电复位后可以看到串口输出Demo运行结果日志如下所示。  
```c
**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
[SFUD]Find a Winbond W25Q256JV flash chip. Size is 33554432 bytes.
[SFUD]W25Q256JV flash device is initialize success.
Sfud start to write.

Huawei LiteOS # Sfud write successfully.
Sfud start to read.
Sfud read successfully.
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 
60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 
70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 
80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 
90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f 
a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af 
b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf 
c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf 
d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df 
e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef 
f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff 
01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 
11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 
21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 
31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f 40 
41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f 50 
51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f 60 
61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f 70 
71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f 80 
81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f 90 
91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f a0 
a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af b0 
b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf c0 
c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf d0 
d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df e0 
e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef f0 
f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff 01 

Sfud start to erase.
Sfud erase successfully.
```


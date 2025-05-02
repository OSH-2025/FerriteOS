# 文件系统
本文介绍LiteOS中的文件系统及其例程。

野火`STM32F429`开发板有V1，V2两个版本，根据实际使用的开发板硬件，选择 `Hw Board V1` 或`Hw Board V2`，默认使用的是`Hw Board V2`。

```
Targets --> Target Cloud_STM32F429IGTx_FIRE --> Choose Hardware Board Version 
                                                    ( ) Hw Board V1
                                                    (X) Hw Board V2
```



## VFS
VFS是Virtual File System（虚拟文件系统）的缩写，是LiteOS文件系统的统一接口，其他文件系统基于VFS提供的接口实现。

VFS实现的标准接口有`open`，`close`，`read`，`write`，`lseek`，`stat`，`ulink`，`rename`，`sync`，`opendir`，`readdir`，`closedir`，`mkdir`，`rmdir`，`dup`，`dup2`，`fcntl`，`ioctl`等功能。

## RAMFS
RAMFS是基于内存的文件系统。在RAMFS中，文件存储在内存中，所有的读/写操作均发生在内存中，由于访问内存比存储器快，从而可以提升文件读写效率。一般可用RAMFS来存储临时文件或者修改频繁的数据。LiteOS的RAMFS基于虚拟文件系统层（VFS)，不能格式化。

### 注意事项

-  RAMFS文件系统支持的操作有：`open`，`close`，`read`，`write`，`lseek`，`stat`，`ulink`，`rename`，`opendir`，`readdir`，`closedir`，`mkdir`。
-  RAMFS只能挂载一次，挂载成功后，后面不能继续挂载到其他目录。
-  RAMFS需要根据设备内存大小来调整挂载的大小，如超出系统剩余内存，会挂载失败。在`targets/bsp/common/fs/fs_init.h`中可以设置RAMFS挂载的大小。
    ```c
    #define RAMFS_SIZE        (2 * 1024)
    ```
-  RAMFS需使能文件系统和RAMFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable RAMFS
   ```
    使能RAMFS后LiteOS会自动初始化并挂载RAMFS。
## SPIFFS

SPIFFS是基于spi flash的嵌入式文件系统，它不需要设备具有很大内存，只需要一定的RAM作为缓存区，这让它可以适用在很多地方。

### 注意事项

-  SPIFFS文件系统支持的操作有：`open`，`close`，`read`，`write`，`lseek`，`stat`，`ulink`，`rename`，`sync`，`opendir`，`readdir`，`closedir`。
-  SPIFFS文件系统目前仅支持STM32F429开发板。
-  SPIFFS需使能文件系统和SPIFFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable SPIFFS
   ```
    使能SPIFFS后LiteOS会自动初始化并挂载SPIFFS。

## FATFS
FAT（File Allocation Table，即文件分配表）文件系统，是一种在Windows操作系统中常用的文件系统。FAT文件系统支持多种介质，在可移动存储介质（U盘、SD卡、移动硬盘等）上广泛使用。它可以使嵌入式设备和Windows、Linux等桌面操作系统保持良好的兼容性，方便用户管理和操作文件。

### 注意事项

- FATFS文件系统支持的操作有：`open`，`close`，`read`，`write`，`lseek`，`stat`，`ulink`，`rename`，`sync`，`opendir`，`readdir`，`closedir`，`mkdir`。
- FATFS文件系统目前仅支持STM32F429和STM32F769开发板。
- FATFS需使能文件系统和FATFS：
   ```
   Components --> FileSystem --> Enable FileSystem
   Components --> FileSystem --> Enable FATFS
   ```
    使能FATFS后LiteOS会自动初始化并挂载FATFS。

## LITELEFS

Littlefs是一种为微控制器设计的，适用于小内存设备的文件系统。它提供了掉电保护机制，如果掉电，文件系统将退回到上一个已知的良好状态。此外，它还提供了动态块的擦写均衡机制，可有效延长flash的使用寿命。

### 注意事项

- LITELEFS文件系统支持的操作有：`open`，`close`，`read`，`write`，`lseek`，`stat`，`ulink`，`rename`，`sync`，`opendir`，`readdir`，`closedir`，`mkdir`。
- LITELEFS文件系统目前仅支持STM32F429开发板。
- LITELEFS需使能文件系统和LITELEFS：
   ```
   Components --> FileSystem --> Enable LITTLEFS
   ```
    使能LITTLEFS后LiteOS会自动初始化并挂载LITTLEFS。

## 文件系统的使用

LiteOS的文件系统提供一套demo来演示文件系统的一些基本操作。

- 对于不同开发板，目前文件系统的支持情况如下：

|  开发板   | RAMFS | FATFS  | SPIFFS | LITTLEFS |
| :-------: | :---: | :----: | :----: | :------: |
| STM32F429 | 支持  |  支持  |  支持  |   支持   |
| STM32F769 | 支持  |  支持  | 不支持 |  不支持  |
| STM32L431 | 支持  | 不支持 | 不支持 |  不支持  |
- 其中STM32F429的FAT文件系统支持两种介质，SPI Flash或SD Card，可以选择，默认为SD Card。
而STM32F769的FAT文件系统目前暂时只支持SD Card。
- SD Card为介质时，需要在开发板上插入SD卡，否则会挂载失败。
- 在STM32F429上，RAMFS、FATFS SD Card类型、SPIFFS或LITTLEFS三种文件系统可同时挂载。但是由于在STM32F429上SPIFFS和LITTLEFS使用相同介质，所以SPIFFS和LITTLEFS不能同时开启。
- 在STM32F769上，可同时挂载RAMFS和FATFS。
- 通过menuconfig使能demo后，编译LiteOS源码，生成系统镜像文件Huawei_LiteOS.bin，并将.bin文件烧写到开发板，复位重启开发板后，demo即启动。

下面介绍各个demo及其使能方法。

### fs_common

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fs_common.c" target="_blank">fs_common.c</a>和<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fs_common.h" target="_blank">fs_common.h</a>文件，封装了一层文件操作函数，方便使用。
#### 写文件
```c
int  write_file(const char *name, char *buff, int len);
```
通过`O_CREAT | O_WRONLY | O_TRUNC`方式`open`，并调用`write`和`close`。

-  name：文件名
-  buff：写入文件的数据
-  len：写入文件的数据长度
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 读文件
```c
int  read_file(const char *name, char *buff, int len);
```
通过`O_RDONLY`方式`open`，并调用`read`和`close`。
-  name：文件名
-  buff：读取到的文件数据
-  len：读取的数据长度
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 打开目录
```c
int  open_dir(const char *name, struct dir **dir);
```
打开一个目录，并把目录结构传给dir结构体指针，如果目录不存在调用mkdir创建目录。
-  name：要打开的目录名称
-  dir：返回的目录结构体指针
   ```c
   struct dir {
       struct mount_point * d_mp;      /* can get private mount data here */
       struct dirent        d_dent;
       off_t                d_offset;
       void               * d_data;
   };
   ```
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 读目录
```c
int  read_dir(const char *name, struct dir *dir);
```
读取目录下的所有文件，并打印出来。

-  name：要读取的目录名称
-  dir：要读取的目录结构体指针
-  返回值：

    **LOS_OK**：成功

    **LOS_NOK**：失败

#### 新建目录
```c
void make_dir(const char *name);
```
新建一个目录。

-  name：新建的目录名称

#### 打印目录
```c
void print_dir(const char *name, int level);
```
打印目录下所有文件及目录，可输出不同的层级。

-  name：要打印的目录名称
-  level：打印的最大目录层级

#### demo接口
```c
void los_vfs_io(char *file_name, char *dir_name);
```
为demo提供的一个测试接口，内部调用了`write_file`，`read_file`，`open_dir`，`read_dir`等函数。
-  file_name：要操作的文件名
-  dir_name：要操作的目录名

### RAMFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/ramfs_demo.c" target="_blank">ramfs_demo.c</a>文件提供了一套ramfs的例程。

#### 使能方法
通过`make menuconfig`打开配置，因为RAMFS demo依赖RAMFS，所以首先使能RAMFS，然后再使能RAMFS demo。
- 使能RAMFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable RAMFS
  ```
- 使能RAMFS demo
  ```
  Demos --> FileSystem Demo --> Enable Ram FileSystem Demo
  ```

### FATFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/fatfs_demo.c" target="_blank">fatfs_demo.c</a>文件提供了一套fatfs的例程。

#### 使能方法
通过`make menuconfig`打开配置，因为FATFS demo依赖FATFS，所以首先使能FATFS，然后再使能FATFS demo。
- 使能FATFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable FATFS
  ```
- 使能FATFS demo
  ```
  Demos --> FileSystem Demo --> Enable Fat FileSystem Demo
  ```

- 在STM32F429上可以选择使用SPI Flash或SD Card为介质
  ```
  Demos --> FileSystem Demo --> Enable Fat FileSystem Demo -->  Choose Fat Hardware Driver
  ```

### SPIFFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/spiffs_demo.c" target="_blank">spiffs_demo.c</a>文件提供了一套spiffs的例程。

#### 使能方法
通过`make menuconfig`打开配置，因为SPIFFS demo依赖SPIFFS，所以首先使能SPIFFS，然后再使能SPIFFS demo。
- 使能SPIFFS
  ```
  Components --> FileSystem --> Enable FileSystem --> Enable SPIFFS
  ```
- 使能SPIFFS demo
  ```
  Demos --> FileSystem Demo --> Enable Spiffs FileSystem Demo
  ```

### LITTLEFS demo

<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/fs/littlefs_demo.c" target="_blank">littlefs_demo.c</a>文件提供了一套littlefs的例程。

#### 使能方法

通过`make menuconfig`打开配置，使能LITTLEFS demo，LITTLEFS demo依赖LITTLEFS组件,使能demo时自动使能LITTLEFS组件。
- 使能LITTLEFS demo
  ```
  Demos --> FileSystem Demo --> Enable Littlefs FileSystem Demo
  ```
- LITTLEFS组件位于
  ```
  Components --> FileSystem --> Enable LITTLEFS
  ```
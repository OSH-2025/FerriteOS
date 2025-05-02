# RHash Demo说明文档
RHash是一个C语言编写的哈希函数库，用于计算和验证磁力链接和各种消息摘要。详情参考<a href="https://github.com/rhash/RHash" target="_blank">RHash</a>。

Rhash支持的哈希函数列表：
|Name|Hash Length (bits)|Format|Specification|
|:----|:----|:----|:----|
|CRC32 |32 |hex |CRC32-IEEE 802.3 |
|CRC32C |32 |hex |RFC 3385 |
|MD4 |128 |hex |RFC 1320 |
|MD5 |128 |hex |RFC 1321 |
|SHA1 |160 |hex |RFC 3174 |
|SHA2 |224/256/384/512 |hex |FIPS 180-4: Secure Hash Standard (SHS) |
|SHA3 |224/256/384/512 |hex| The Keccak sponge function family |
|TIGER |192 |hex |Tiger page |
|TTH |192 |base32 |TTH description |
|BTIH |160 |hex |BitTorrent Specification |
|ED2K |128 |hex |ED2K description |
|AICH |160 |base |32AICH description |
|WHIRLPOOL |512 |hex |ISO/IEC 10118-3:2004 |
|RIPEMD-160 |160 |hex |RIPEMD-160 page |
|GOST R 34.11-94 |256 |hex |RFC 5831 |
|GOST-CRYPTOPRO |256 |hex |RFC 4357 |
|GOST R 34.11-2012 |256/512 |hex |RFC 6986 |
|HAS-160 |160 |hex |HAS-160 specification |
|SNEFRU |128/256 |hex |Ralph C. Merkle, 1990 |
|EDON-R |256/512 |hex| D. Gligoroski, S. Markovski, L.Kocarev, Edon–R, An Inﬁnite Family of Cryptographic Hash Functions, 2006Functions, 2006 |

## HASH
Hash，一般翻译做散列、杂凑，或音译为哈希，是一种将任意长度的输入通过散列算法变换成固定长度的输出的函数，详情参考<a href="https://baike.baidu.com/item/Hash/390310?fr=aladdin" target="_blank">Hash</a>。

## 使能RHash Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能RHash Demo。

    ```
    Demos  --->
        Security Demo  --->
                [*] Enable RHash Demo 
    ```
    使能RHash Demo后会自动使能`RHash`组件。

    保存退出后，会从github上自动下载`RHash`源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。关于在线组件的自动下载请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#" target="_blank">在线组件下载流程</a>。

    **注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行RHash Demo

使能RHash Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`librhash.a`和`librhash_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

以`Cloud_STM32F429IGTx_FIRE`开发板为例来运行`RHash Demo`，其他开发板操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

LiteOS启动后会运行`RHash Demo`，本demo演示了MD5和SHA256两种哈希函数的使用方法。通过计算给定数据的md5和sha256哈希值，与在Linux平台下使用类似算法计算出来的哈希值进行比较。

给定数据为：
```c
#define DEMO_STR               "This is Huawei LiteOS RHash Demo.\n"
```
在linux开发环境下新建文件test.txt，复制上述给定数据内容到test.txt中并保存文件，执行以下linux命令生成md5和sha256的值：
```
$ md5sum test.txt
$ sha256sum test.txt
```
获得的md5和sha256值为：
```c
#define MD5_SUM                "7990beea258031735be06254cea58a89"
#define SHA256_SUM             "d56910304664d3d3a439968b020bb465d5de33715cfb109d71704b91f85db30c"
```

在LiteOS环境中，RHash Demo运行结果如下：
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Aug 12 2021 10:32:28

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
RHash demo start to run.
Original md5:
7990beea258031735be06254cea58a89
RHash calculate md5:
7990beea258031735be06254cea58a89
Original sha256:
d56910304664d3d3a439968b020bb465d5de33715cfb109d71704b91f85db30c
RHash calculate sha256:
d56910304664d3d3a439968b020bb465d5de33715cfb109d71704b91f85db30c
RHash demo finished.

Huawei LiteOS # 

```
通过以上例程可知，使用RHash计算出来的哈希值与linux平台下类似工具的计算结果完全一致。
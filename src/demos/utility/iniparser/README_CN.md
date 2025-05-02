# Iniparser Demo说明文档

Iniparser是针对ini文件（系统或者软件配置文件）的解析库。本Demo实现了对ini文件中Hamburger和Beer属性的解析。

LiteOS中提供了`IniparserDemoTask`来演示如何使用`Iniparser`组件。由于此组件需要较大内存资源，暂时只支持在以下开发板上运行`IniparserDemoTask`。

- STM32F769
- realview-pbx-a9
- qemu-virt-a53


## Iniparser接口说明

### 读取配置文件

```c
dictionary * iniparser_load(const char * ininame);
```

- dictionary：返回一个字典对象
- ininame：被解析的ini文件

### 保存dictionary对象到file 

```c
void iniparser_dump(const dictionary * d, FILE * f);
```

- d：要转储的字典
- f：要转储的文件

### 获取与键关联的字符串，转换为布尔值

```c
int iniparser_getboolean(const dictionary * d, const char * key, int notfound);
```

- d：要搜索的字典
- key：要查找的关键字字符串
- notfound：错误时返回的值

### 获取与键关联的字符串

```c
const char * iniparser_getstring(const dictionary * d, const char * key, const char * def);
```

- d：要搜索的字典
- key：要查找的关键字字符串
- def：如果没有找到key，则返回默认值

### 获取与键关联的字符串，转换为int

```c
int iniparser_getint(const dictionary * d, const char * key, int notfound);
```

- d：要搜索的字典
- key：要查找的关键字字符串
- notfound：错误时返回的值

### 获取与键关联的字符串，转换为double

```c
double iniparser_getdouble(const dictionary * d, const char * key, double notfound);
```

- d：要搜索的字典
- key：要查找的关键字字符串
- notfound：错误时返回的值

### 释放dictionary

```c
void iniparser_freedict(dictionary * d);
```

- 释放dictionary


## 使能Iniparser Demo

在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Iniparser Demo。 

```
Kernel  --->
        [ ] Enable Kernel SMP

Demos  --->
       Utility Demo  --->
               [*] Enable Iniparser Demo (NEW)
```

- 使能Iniparser Demo后会自动使能Iniparser组件和ramfs文件系统。 
- 由于当前只支持单核模式下运行该组件，需要关闭SMP多核功能。 

保存退出后，会从github上自动下载Iniparser源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。 

## 测试结果

在Linux主机的LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在~/Huawei_LiteOS/out/realview-pbx-a9/lib目录下生成组件库文件libiniparser.a和此Demo库文件libiniparser_demo.a，系统镜像文件为Huawei_LiteOS.bin。

执行以下命令即可运行LiteOS工程，更详细的信息可以参考[CORTEX A9在QEMU下运行LiteOS](https://gitee.com/LiteOS/LiteOS/blob/master/targets/realview-pbx-a9/README_CN.md)。

```
~/Huawei_LiteOS$ qemu-system-arm -machine realview-pbx-a9 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```

Demo运行结果如下所示，可以看到此Demo解析了ini文件中的Hamburger和Beer属性。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
Processor  : Cortex-A9 * 4
Run Mode   : SMP
GIC Rev    : GICv1
build time : Mar 13 2021 01:52:47

**********************************

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 2 entering scheduler
cpu 1 entering scheduler
cpu 3 entering scheduler
app init!
Hello, welcome to liteos demo!
Iniparser demo task start to run.

Huawei LiteOS # 
Hamburger:
Lettuce:     [1]
Chickens:    [1]
Seafood:     [0]
Cheese:      [0]

Beer:
Composition: [Wheat]
Year:        [2021]
Country:     [China]
Alcohol:     [10.5]
Iniparser demo task finished.
```

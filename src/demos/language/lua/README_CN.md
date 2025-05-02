# Lua Demo说明文档
Lua是一种C语言编写的一种脚本语言，详情参考<a href="https://github.com/lua/lua" target="_blank">lua</a>。

## 解析Lua Demo

LiteOS中提供了`LuaDemoTask`程序来演示如何使用`Lua`。

在`lua_demo.c`中定义了`luaData`字符串，内容为
```lua
function max(a, b)
    if (a > b) then
        c = a;
    else
        c = b;
    end
    return c;
end
print("the max num is ", max(1, 2))
print("the max num is ", max(4, 3))
```
本Demo使用的文件系统为`ramfs`，把`luaData`写入到`/ramfs/test.lua`文件中，然后调用`lua`入口函数`LuaMain`来执行`/ramfs/test.lua`文件。

## 使能Lua Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

    **注意** 根据`Lua`所需资源，暂时只支持在以下开发板上运行`LuaDemoTask`。
    - STM32F429
    - STM32F407
    - STM32F769
    - realview-pbx-a9
    - qemu-virt-a53

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Lua Demo。

    ```
    Demos  --->
        Language Demo  --->
                [*] Enable Lua Demo
    ```
使能Lua Demo后会自动使能Lua组件和ramfs文件系统。

保存退出后，会从github上自动下载lua源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Lua Demo

使能Lua Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`liblua.a`和`liblua_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。
以QEMU模拟器工程`realview-pbx-a9`为例来运行`Lua Demo`，执行以下命令即可运行LiteOS工程，更详细的信息可以参考[CORTEX A9在QEMU下运行LiteOS](https://gitee.com/LiteOS/LiteOS/blob/master/targets/realview-pbx-a9/README_CN.md)。

```
$ qemu-system-arm -machine realview-pbx-a9 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```
其他开发板操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

启动LiteOS后会运行`Lua Demo`，执行`/ramfs/test.lua`脚本文件，定义了一个判断两数中最大值的`max`函数，调用函数，并输出结果。执行结果如下
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
Processor  : Cortex-A9 * 4
Run Mode   : SMP
GIC Rev    : GICv1
build time : Mar 12 2021 22:43:51

**********************************

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 3 entering scheduler
cpu 2 entering scheduler
cpu 1 entering scheduler
app init!
Hello, welcome to liteos demo!
Lua demo task start to run.
Run lua file.
the max num is 	2
the max num is 	4
Run lua file finished.
Lua demo task finished.

Huawei LiteOS #
```
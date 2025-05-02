# Json Demo说明文档

Json-c是基于C语言的一个轻量级的JSON格式文件编解码器，可以运行在多个硬件平台上。

Json Demo源码文件为：<a href="https://gitee.com/LiteOS/LiteOS/blob/master/demos/utility/json/json_demo.c" target="_blank">demos/utility/json/json_demo.c</a>。
该Demo使用Json解析器将创建的json格式字符串解析为json对象，可以运行在LiteOS已支持的所有开发板上。


## 相关接口介绍

1) 将json格式的字符串转化为json对象：
    ```c
    struct json_object* json_tokener_parse(const char *str);
    ```
2) 将json格式的文本内容转化为json对象：
    ```c
    struct json_object* json_object_from_file(const char *filename);
    ```
3) 将json对象转换为字符串数据：
    ```c
    const char* json_object_get_string(struct json_object *obj);
    ```
4) 从json数据中提取数据：
    ```c
    int json_pointer_get(struct json_object *obj, const char *path, struct json_object **res);
    ```
5) 修改json对象中字段值：
    ```c
    int json_pointer_set(struct json_object **obj, const char *path, struct json_object *value);
    ```
6) 释放json对象的内存：
    ```c
    int json_object_put(struct json_object *jso);
    ```


## 使能Json Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能json组件和Json Demo。

    ```
    Components -->
        Utility  --->
                [*] Enable Json-c
    Demos  --->
        Utility Demo  --->
                [*] Enable Json-c Demo
    ```

保存退出后，LiteOS会从github上自动下载json-c源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。


## 测试结果

在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后会在`out/Cloud_STM32F429IGTx_FIRE/lib`路径下生成`libjson.a`和`libjson_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板中，可以看到如下解析内容。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 01:40:10

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Json-c demo task start to run.
{ "name": "test", "age": 19, "gender": "male", "occupation": "programmer", "conutry": "China", "": 0 }
Json-c parse string successfully.
Json-c test empty string successfully.
Json-c get test successfully.
Json-c demo task finished.

Huawei LiteOS # 

```
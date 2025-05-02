# C-algorithms Demo说明文档

C-algorithms是一个常用算法和数据结构的集合。

LiteOS中提供了`AlgorithmsDemoTask`来演示如何使用`c-algorithms`组件。目前LiteOS支持的开发板上都可以运行`AlgorithmsDemoTask`，例如：

- STM32F769
- STM32F429
- realview-pbx-a9
- qemu-virt-a53

## 解析C-algorithms Demo

c-algorithms算法库集合了`avl-tree`，`binary-heap`，`arraylist`，`hash-table`，`list`，`queue`等一系列算法，下面是对其中一些算法的解析：

### Queue demo算法解析

Queue demo通过一个排队打饭场景展示了队列的插入和取出：

- 将`xiaohong`、`xiaoming`、`xiaoqiang`排到队列尾;

```c
queue_push_tail();
```

| xiaohong | xiaoming | xiaoqiang |
| :------: | :------: | :-------: |

 - 此时`xiaohong`买走一个三明治，轮到`xiaoming`打饭;

```c
queue_pop_head();
```

| xiaoming | xiaoqiang |
| :------: | :-------: |

- 突然`xiaobing`插到`xiaoming`前面，`xiaobing`成为排头。

```c
queue_push_head();
```

| xiaobing | xiaoming | xiaoqiang |
| :------: | :------: | :-------: |

### Arraylist demo算法解析

- 首先向数组队列里追加5和7；

```c
arraylist_append();
```

- 数组队列第二位插入2；

```c
arraylist_insert();
```

- 移走数组队列中的第一位，最后数组队列里剩下2和7。

```c
arraylist_remove();
```

### Hash demo算法解析

- 首先向hash table插入key和对应的value；

```c
hash_table_insert();
```

- 再通过key找到对应的value。

```c
hash_table_lookup();
```

### List Demo算法解析

- 将数组a追加到链表里；

```c
list_append();
```

- 将链表里的数据进行排序；

```c
list_sort();
```

- 移走链表里的指定数据；

```c
list_remove_data();
```

- 将数组a倒序放到链表里；

```c
list_prepend();
```

- 移走链表里第三位数据；

```c
list_remove_entry();
```

- 从链表中查找指定数据。

```c
list_find_data();
```


## 使能C-algorithms Demo

在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能C-algorithms Demo。 

```
Demos  --->
       Utility Demo  --->
               [*] Enable C-algorithms Demo (NEW)
```

- 使能C-algorithms Demo后会自动使能c-algorithms组件。  

保存退出后，会从github上自动下载c-algorithms源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。 

## 编译运行C-algorithms Demo

操作步骤如下：

1. 参照上述使能c-algorithms demo。

2. 在Linux主机的LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在~/Huawei_LiteOS/out/${platform}/lib目录下生成c-algorithms的库文件libc-algorithms.a和此Demo库文件libc-algorithms_demo.a，系统镜像文件为Huawei_LiteOS.bin。以`Cloud_STM32F429IGTx_FIRE/`开发板为例，其目录就是`out/Cloud_STM32F429IGTx_FIRE/`。

3. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul 22 2021 15:15:48

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
C algorithms demo task to run.
Queue demo start to run.
First in line is xiaohong.
Last in line is xiaoqiang.
xiaohong bought a sandwich.
Now xiaoming turn is it to buy rice.
xiaobing cutting to the front change first in line.
Array demo start to run.
position[0]->value[5]
position[1]->value[7]
Insert the number 2 in the second digit
position[0]->value[5]
position[1]->value[2]
position[2]->value[7]
Arraylist remove first num.
position[0]->value[2]
position[1]->value[7]
Hash demo start to run.
one 
List demo start to run.
Array a append to list:
1 3 5 2 4 6 
Sort num :
1 2 3 4 5 6 
After remove array a third num:
1 2 3 4 6 
Find array a second data is [3].
Array a prepend to list:
6 4 2 5 3 1 
After remove, len is [5], list is: 
6 4 5 3 1 
C algorithms demo task to finished.

Huawei LiteOS # 
```

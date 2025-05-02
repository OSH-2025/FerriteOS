# Iperf 使用手册

Iperf 是一类测试TCP或UDP连接性能的网络性能测试工具，适用于TCP和UDP的带宽测试。   

下面以`Cloud_STM32F429IGTx_FRIE`开发板为例，讲解如何在shell命令行内使用`iperf`功能。    

* 目前支持`Iperf`功能的开发板有：   
`Cloud_STM32F429IGTx_FRIE`

## 使能 Iperf
在LiteOS源码根目录下运行`make menuconfig`命令，按如下菜单路径使能`iperf`组件：  
1. 使能`shell`
```
   Debug  ---> [*] Enable a Debug Version ---> [*] Enable Shell
```
2. 使能`iperf`
```
   Components  ---> Network ---> [*] Enable Iperf
```
保存退出后，  在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成组件静态库文件`liblos_iperf.a`，系统镜像文件为Huawei_LiteOS.bin。   

## Iperf Shell命令
1. 进入Shell界面   
   * 通过串口终端工具（比如`xShell`），连接串口。   
   * 选择SERIAL协议，然后配置SERIAL：选择对应的串口号，设置波特率为`115200`。   
   * 连接后重启开发板，系统启动后可以在串口工具中看到“Huawei LiteOS #”的提示符。   

2. 查看`iperf` 命令是否生效：   
使用串口工具，进入shell， 输入`help`命令后， 如果显示`iperf`命令，则表明已经生效。   
```
   Huawei LiteOS # help
   *******************shell commands:*************************
   date        free     help          hwi      iperf    log    
   memcheck    swtmr    systeminfo    task     uname    watch

   Huawei LiteOS # 
```

3. 查看`iperf`使用帮助命令：   
进入shell， 输入`iperf -h`   
```
   Huawei LiteOS # iperf -h
   Usage: Iperf [-s|-c host] [options].
          Iperf [-h|-help|--help].
   Client/Server:
     -i, #[interval]  Pause n seconds between periodic bandwidth reports(default 1 secs).
     -l, #[length]    Length of buffer to read or write, defalut 1470(UDP)/8KB(TCP).
     -u,              Use UDP rather than TCP.
     -p, #[port]      Set server port to listen on/connect to to n (default 5001).
     -V,              Set the domain to IPv6(Must be 1st parameter).
     -S, #[tos]       Set the IP ToS, 0-255, default 0.
     -B, #<host>      Bind to <host>, an unicast address, not support multicast.
     -k               Kill the iperf.
   Client specific:
     -c, #[host]      Run in client mode, connecting to <host>.
     -t, #[time]      Time in seconds to transmit for (default 30 secs).
     -n, #[KM]        Number of bytes to transmit (instead of -t).
     -b, #[KM]        For UDP, bandwidth to send in bits/sec(default 1 Mbits/sec).
   Server specific:
     -s,              Run in server mode.

   Huawei LiteOS # 
```
## Iperf 测试示例

### 测试条件

1. 准备一台PC，环境为`Linux`或`Windows`，且已安装了`Iperf`工具 (版本要求2.0)。   
2. 将PC与开发板进行以太网连接。   
### 安装`Iperf`工具
* `Linux PC`端，在终端执行`sudo apt-get install iperf`，进行安装。   
* `Windows PC`端， 请自行上网搜索`iperf2`工具，进行安装。   
* 本文档的测试环境以内部网络环境，测试PC为`Ubuntu Linux`。   
### ip地址设置
* 在<a href="https://gitee.com/LiteOS/LiteOS/blob/master/targets/Cloud_STM32F429IGTx_FIRE/Src/sys_init.c" target="_blank">sys_init.c</a>文件的`void net_init(void)`函数中设置开发板的IP地址及网关地址，网关地址与路由器中设置的保持一致。   
* 以下使用到的ip地址，`192.168.3.37`为PC端的`ip`地址，`192.168.3.200`为开发板的`ip`地址。   
### 测试步骤
* `iperf`命令的参数说明，请参考`iperf -h`帮助的描述。   
#### TCP带宽测试
以开发板作为`iperf`的服务端, 测试PC作为`iperf`的客户端，测试TCP带宽。   
开发板端执行`iperf -s -p 5001`   
PC端执行`iperf -c 192.168.3.200 -p 5001`   

开发板串口输出结果为测试得到的TCP带宽数据，如下所示：
```
iperf -s -p 5001
Iperf info: TCP-Server-IPV4
SrcIP: 0.0.0.0, DstIP: 0.0.0.0, Port: 5001.
Interval: 1sec, Time: 0sec, Amount: 0, Bandwidth: 0, BufLen: 8192.

Huawei LiteOS # +IPERF:

Interval            Transfer            Bandwidth       
 0.0- 1.0 sec      8.55 KBytes    70.08 Kbits/sec
 1.0- 2.0 sec      5.70 KBytes    46.72 Kbits/sec
 2.0- 3.0 sec      5.70 KBytes    46.72 Kbits/sec
 3.0- 4.0 sec      5.70 KBytes    46.72 Kbits/sec
 4.0- 5.0 sec      5.70 KBytes    46.72 Kbits/sec
 5.0- 6.0 sec      5.70 KBytes    46.72 Kbits/sec
 6.0- 7.0 sec      5.70 KBytes    46.72 Kbits/sec
 7.0- 8.0 sec      5.70 KBytes    46.72 Kbits/sec
 8.0- 9.0 sec      5.70 KBytes    46.72 Kbits/sec
 9.0-10.0 sec      5.70 KBytes    46.72 Kbits/sec
10.0-11.0 sec      5.70 KBytes    46.72 Kbits/sec
11.0-12.0 sec      5.70 KBytes    46.72 Kbits/sec
12.0-13.0 sec      5.70 KBytes    46.72 Kbits/sec
 0.0-13.5 sec     78.42 KBytes    47.61 Kbits/sec
Ok.
```
以开发板作为`iperf`的客户端，测试PC作为`iperf`的服务端，测试TCP带宽。   
开发板端执行`iperf -c 192.168.3.37 -p 5001`   
PC端执行       `iperf -s -p 5001`   

开发板串口输出结果为测试得到的TCP带宽数据，如下所示：   
```
iperf -c 192.168.3.37 -p 5001
Iperf info: TCP-Client-IPV4
SrcIP: 0.0.0.0, DstIP: 192.168.3.37, Port: 5001.
Interval: 1sec, Time: 30sec, Amount: 0, Bandwidth: 0, BufLen: 8192.

Huawei LiteOS # +IPERF:

Interval            Transfer            Bandwidth       
 0.0- 1.0 sec      8.00 KBytes    65.54 Kbits/sec
 1.0- 2.0 sec     16.00 KBytes    131.07 Kbits/sec
 2.0- 3.0 sec      8.00 KBytes    65.54 Kbits/sec
 3.0- 4.0 sec     16.00 KBytes    131.07 Kbits/sec
 4.0- 5.0 sec      8.00 KBytes    65.54 Kbits/sec
 5.0- 6.0 sec     16.00 KBytes    131.07 Kbits/sec
 6.0- 7.0 sec      8.00 KBytes    65.54 Kbits/sec
 7.0- 8.0 sec      8.00 KBytes    65.54 Kbits/sec
 8.0- 9.0 sec     16.00 KBytes    131.07 Kbits/sec
 9.0-10.0 sec      8.00 KBytes    65.54 Kbits/sec
10.0-11.0 sec     16.00 KBytes    131.07 Kbits/sec
11.0-12.0 sec      8.00 KBytes    65.54 Kbits/sec
12.0-13.0 sec     16.00 KBytes    131.07 Kbits/sec
13.0-14.0 sec      8.00 KBytes    65.54 Kbits/sec
14.0-15.0 sec      8.00 KBytes    65.54 Kbits/sec
15.0-16.0 sec     16.00 KBytes    131.07 Kbits/sec
16.0-17.0 sec      8.00 KBytes    65.54 Kbits/sec
17.0-18.0 sec     16.00 KBytes    131.07 Kbits/sec
18.0-19.0 sec      8.00 KBytes    65.54 Kbits/sec
19.0-20.0 sec     16.00 KBytes    131.07 Kbits/sec
20.0-21.0 sec      8.00 KBytes    65.54 Kbits/sec
21.0-22.0 sec      8.00 KBytes    65.54 Kbits/sec
22.0-23.0 sec     16.00 KBytes    131.07 Kbits/sec
23.0-24.0 sec      8.00 KBytes    65.54 Kbits/sec
24.0-25.0 sec     16.00 KBytes    131.07 Kbits/sec
25.0-26.0 sec      8.00 KBytes    65.54 Kbits/sec
26.0-27.0 sec     16.00 KBytes    131.07 Kbits/sec
27.0-28.0 sec      8.00 KBytes    65.54 Kbits/sec
28.0-29.0 sec      8.00 KBytes    65.54 Kbits/sec
29.0-30.0 sec     16.00 KBytes    131.07 Kbits/sec
Ok.

 0.0-30.5 sec    352.00 KBytes    94.56 Kbits/sec
```

#### UDP带宽测试
以开发板作为`iperf`的服务端，测试PC作为`iperf`的客户端，测试UDP带宽。   
开发板端执行`iperf -u -s -p 5001`   
PC端执行     `iperf -u -c 192.168.3.200 -p 5001`   

开发板串口输出结果为测试得到的UDP带宽数据，如下所示：
```
iperf -u -s -p 5001
Iperf info: UDP-Server-IPV4
SrcIP: 0.0.0.0, DstIP: 0.0.0.0, Port: 5001.
Interval: 1sec, Time: 0sec, Amount: 0, Bandwidth: 0, BufLen: 1470.

Huawei LiteOS # +IPERF:

Interval            Transfer            Bandwidth       
 0.0- 1.0 sec     21.53 KBytes    176.40 Kbits/sec
 1.0- 2.0 sec     14.36 KBytes    117.60 Kbits/sec
 2.0- 3.0 sec     14.36 KBytes    117.60 Kbits/sec
 3.0- 4.0 sec     14.36 KBytes    117.60 Kbits/sec
 4.0- 5.0 sec     14.36 KBytes    117.60 Kbits/sec
 5.0- 6.0 sec     14.36 KBytes    117.60 Kbits/sec
 6.0- 7.0 sec     14.36 KBytes    117.60 Kbits/sec
 7.0- 8.0 sec     14.36 KBytes    117.60 Kbits/sec
 8.0- 9.0 sec     14.36 KBytes    117.60 Kbits/sec
 9.0-10.0 sec     14.36 KBytes    117.60 Kbits/sec
 0.0-10.5 sec    152.17 KBytes    118.80 Kbits/sec
Ok.
```
以开发板作为`iperf`的客户端，测试PC作为`iperf`的服务端，测试UDP带宽。   
开发板端执行 `iperf -u -c 192.168.3.37 -p 5001`   
PC端执行`iperf -u -s -p 5001`   

开发板串口输出结果为测试得到的UDP带宽数据，如下所示：   
```
iperf -u -c 192.168.3.37 -p 5001
Iperf info: UDP-Client-IPV4
SrcIP: 0.0.0.0, DstIP: 192.168.3.37, Port: 5001.
Interval: 1sec, Time: 30sec, Amount: 0, Bandwidth: 1048576, BufLen: 1470.

Huawei LiteOS # +IPERF:

Interval            Transfer            Bandwidth       
 0.0- 1.0 sec    127.76 KBytes    1.05 Mbits/sec
 1.0- 2.0 sec    127.76 KBytes    1.05 Mbits/sec
 2.0- 3.0 sec    129.20 KBytes    1.06 Mbits/sec
 3.0- 4.0 sec    127.76 KBytes    1.05 Mbits/sec
 4.0- 5.0 sec    129.20 KBytes    1.06 Mbits/sec
 5.0- 6.0 sec    127.76 KBytes    1.05 Mbits/sec
 6.0- 7.0 sec    129.20 KBytes    1.06 Mbits/sec
 7.0- 8.0 sec    127.76 KBytes    1.05 Mbits/sec
 8.0- 9.0 sec    129.20 KBytes    1.06 Mbits/sec
 9.0-10.0 sec    127.76 KBytes    1.05 Mbits/sec
10.0-11.0 sec    129.20 KBytes    1.06 Mbits/sec
11.0-12.0 sec    127.76 KBytes    1.05 Mbits/sec
12.0-13.0 sec    129.20 KBytes    1.06 Mbits/sec
13.0-14.0 sec    127.76 KBytes    1.05 Mbits/sec
14.0-15.0 sec    129.20 KBytes    1.06 Mbits/sec
15.0-16.0 sec    129.20 KBytes    1.06 Mbits/sec
16.0-17.0 sec    127.76 KBytes    1.05 Mbits/sec
17.0-18.0 sec    129.20 KBytes    1.06 Mbits/sec
18.0-19.0 sec    127.76 KBytes    1.05 Mbits/sec
19.0-20.0 sec    129.20 KBytes    1.06 Mbits/sec
20.0-21.0 sec    127.76 KBytes    1.05 Mbits/sec
21.0-22.0 sec    129.20 KBytes    1.06 Mbits/sec
22.0-23.0 sec    127.76 KBytes    1.05 Mbits/sec
23.0-24.0 sec    129.20 KBytes    1.06 Mbits/sec
24.0-25.0 sec    127.76 KBytes    1.05 Mbits/sec
25.0-26.0 sec    129.20 KBytes    1.06 Mbits/sec
26.0-27.0 sec    127.76 KBytes    1.05 Mbits/sec
27.0-28.0 sec    129.20 KBytes    1.06 Mbits/sec
28.0-29.0 sec    127.76 KBytes    1.05 Mbits/sec
29.0-30.0 sec    129.20 KBytes    1.06 Mbits/sec
Ok.

 0.0-29.6 sec      3.77 MBytes    1.07 Mbits/sec
```

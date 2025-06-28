# 性能测试与分析报告

[TOC]

## LLM 部署相关性能指标列表

| 序号 | 性能指标          | 含义                                                         | 合理性                                                     |
| ---- | ----------------- | ------------------------------------------------------------ | ---------------------------------------------------------- |
| 1    | 输出速度          | 模型每秒能生成的 token 数量                                  | 衡量模型吞吐量和效率最直观的指标                           |
| 2    | 首 token 生成延迟 | 从用户发送请求到模型返回第一个 token 所需要的时间            | 直接关系到用户对响应速度的感知                             |
| 3    | 总生成延迟        | 从用户发送请求到模型完成所有 token 生成并返回完整输出所需要的时间 | 决定用户工作流效率的核心因素                               |
| 4    | 显存占用          | 模型在运行时所占用的 GPU 显存量                              | 直接影响到单个 GPU 上可以部署的模型实例数量                |
| 5    | 困惑率            | 表示模型对给定文本序列的“不确定性”或“困惑程度”               | 语言模型预测能力的指标，数值越低表示模型对文本的预测越准确 |



## 测试任务设计

本次实验利用 llama.cpp 自带的 llama-bench 工具和系统的 time 命令对以上性能指标中的 **输出速度** 和 **首 token 生成延迟** 进行测试。

### 1. 输出速度

在 llama.cpp 项目根目录下，具体测试命令为

```bash
./build/bin/llama-bench -m ./models/Llama-3.2-3B-Instruct-Q6_K.gguf #-p N -n N
```

其中 `-p` 后面可以输入一次测试处理的 prompt 数；`-n` 可以输入一次测试生成的 token 数；`-t` 后面输入测试线程数；`-b` 后面输入一次批处理数量。

### 2. 首 token 生成延迟

在 llama.cpp 目录下，具体测试命令为

```bash
time ./build/bin/llama-cli -m ./models/Llama-3.2-3B-Instruct-Q6_K.gguf -n 1 #-p PROMPT -ngl N
```

`-n` 后为一次交互生成的 token 数（此处为测首 token 生成延迟而固定为1）; `-p` 后接具体 prompt 内容；`-ngl` 为下放给 GPU 加速执行的模型层数；`-temp` 为控制生成多样性的采样温度。



## 测试与优化结果分析

### 1. 测试结果

**输出速度**：

- 文本生成：

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p 0 -n 128,256,512
```

| model         |     size | params | backend |  ngl |  test |          t/s |
| ------------- | -------: | -----: | ------- | ---: | ----: | -----------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 | tg128 | 79.20 ± 0.46 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 | tg256 | 77.73 ± 0.12 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 | tg512 | 75.51 ± 0.33 |

- 不同批处理规模下的提示词处理：

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p 1024 -n 0 -b 64,128,256,512,1024
```

| model         |     size | params | backend |  ngl | n_batch |   test |             t/s |
| ------------- | -------: | -----: | ------- | ---: | ------: | -----: | --------------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |      64 | pp1024 | 1990.50 ± 17.68 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |     128 | pp1024 | 2516.43 ± 23.75 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |     256 | pp1024 | 2755.36 ± 14.32 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |     512 | pp1024 | 2824.38 ± 32.41 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |    1024 | pp1024 | 2835.21 ± 37.32 |

- 不同线程数

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -n 0 -n 16 -p 64 -t 1,2,4,8,16,32
```

| model         |     size | params | backend |  ngl | threads | test |             t/s |
| ------------- | -------: | -----: | ------- | ---: | ------: | ---: | --------------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       1 | pp64 | 1623.44 ± 87.99 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       1 | tg16 |    43.41 ± 3.78 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       2 | pp64 | 1581.78 ± 45.78 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       2 | tg16 |    46.21 ± 2.73 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       4 | pp64 | 1570.09 ± 25.45 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       4 | tg16 |    45.85 ± 3.09 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       8 | pp64 | 1575.34 ± 51.89 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |       8 | tg16 |    45.66 ± 3.48 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |      16 | pp64 | 1591.30 ± 34.35 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |      16 | tg16 |    45.75 ± 3.61 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |      32 | pp64 | 1607.89 ± 23.48 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   99 |      32 | tg16 |    45.93 ± 3.37 |

- 将不同数量的模型层下放给 GPU 加速执行

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -ngl 10,20,30,31,32,33,34,35
```

| model         |     size | params | backend |  ngl |  test |              t/s |
| ------------- | -------: | -----: | ------- | ---: | ----: | ---------------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   10 | pp512 |   828.26 ± 19.56 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   10 | tg128 |     15.67 ± 1.45 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   20 | pp512 | 1002.68 ± 106.12 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   20 | tg128 |     27.26 ± 2.31 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   30 | pp512 | 1924.99 ± 154.00 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   30 | tg128 |     47.06 ± 5.92 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   31 | pp512 | 1930.55 ± 176.58 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   31 | tg128 |     43.94 ± 0.14 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   32 | pp512 | 1903.55 ± 185.61 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   32 | tg128 |     47.55 ± 7.74 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   33 | pp512 | 1954.74 ± 154.01 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   33 | tg128 |     44.74 ± 0.13 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   34 | pp512 | 1939.74 ± 184.26 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   34 | tg128 |     47.08 ± 6.34 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   35 | pp512 | 1929.25 ± 188.28 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA    |   35 | tg128 |     43.93 ± 0.12 |

**首 token 生成延迟**

每次测试的 real 时间为用户按下回车到进程结束的总时间。我们决定采用相同命令进行五次测试取平均值的方式来确定每次测试的首token生成延迟。

- 基本测试

```bash
time ./bin/llama-cli -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p "What is love?" -n 1
```

| 1      | 2      | 3      | 4      | 5      |
| ------ | ------ | ------ | ------ | ------ |
| 2.934s | 2.663s | 2.704s | 2.964s | 2.606s |

平均值为 2.7742s

- 模型层数影响测试

```bash
time ./bin/llama-cli -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p "What is love?" -n 1 -ngl 10
```

| 1      | 2      | 3      | 4      | 5      |
| ------ | ------ | ------ | ------ | ------ |
| 3.387s | 3.046s | 3.003s | 3.156s | 3.092s |

平均值为3.1368s

### 2. 优化分析

该部分分析依据即为上方“测试结果”部分的测试结果，因此不在此处再做粘贴。

通过以上数据可以看出：当批处理数量增大或线程数增加，模型的生成速度(token/s) 都会得到增加，而当模型层数增加时，生成速度和首 token 返回延迟都会得到显著改善。

总结以下结果较为显著的优化方式：

- 增大 **批处理数量** `-b`

  原因分析：一次性将更多的输入序列送入 GPU 进行处理，可以充分地利用 GPU 大量的计算核心并分摊固定开销

- 增大 **线程数** `-t`

  原因分析：增大线程数允许 CPU 在数据预处理、后处理或部分模型计算时进行并行操作，从而加速整体流程

- 增加 **模型层数** `-ngl`

  将更多层放在 GPU 上能显著减少 CPU 和 GPU 之间的数据传输，并利用 GPU 的高效并行处理能力

而我们还可以从以上数据看出，随着这些参数的调整，输出速度往往会进入正常的瓶颈，我们分析原因如下：

- **GPU 显存限制**：当 batch-size 过大时，显存无法容纳所有数据，导致性能下降或程序崩溃。此外，模型的每一层都需要占用显存，当加载的层数过多时，GPU 的计算能力也会达到饱和。
- **CPU 核心数量有限**：当线程数超过核心数时，线程间的频繁切换和同步开销反而会降低效率。



## RPC 分布式部署性能测试及分析

### 性能测试

在主机上运行相应测试命令，并加上 `--rpc` 参数针对分布式部署进行性能测试：

- 输出速度测试

```sh
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p 0 -n 128,256,512 --rpc 127.0.0.1:50052;127.0.0.1:50053
```

| model         |     size | params | backend  |  ngl |   test |          t/s |
| ------------- | -------: | -----: | -------- | ---: | -----: | -----------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |  tg128 | 14.39 ± 1.21 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |  tg256 | 13.73 ± 0.06 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |  tg512 | 13.35 ± 0.06 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 | tg1024 | 12.84 ± 0.06 |

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p 64 -n 0 -n 16 -t 1,2,4,8,16,32 --rpc 127.0.0.1:50052,127.0.0.1:50053
```

| model         |     size | params | backend  |  ngl | threads | test |          t/s |
| ------------- | -------: | -----: | -------- | ---: | ------: | ---: | -----------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       1 | pp64 | 58.47 ± 4.26 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       1 | tg16 | 16.73 ± 2.76 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       2 | pp64 | 60.57 ± 4.27 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       2 | tg16 | 17.64 ± 3.40 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       4 | pp64 | 57.07 ± 4.25 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       4 | tg16 | 17.48 ± 3.43 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       8 | pp64 | 59.06 ± 4.19 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |       8 | tg16 | 17.24 ± 3.09 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |      16 | pp64 | 58.83 ± 2.37 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |      16 | tg16 | 16.95 ± 2.32 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |      32 | pp64 | 59.36 ± 3.67 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   99 |      32 | tg16 | 16.32 ± 1.49 |

```bash
./bin/llama-bench -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -ngl 5,10,15,20,25,30,35,40 --rpc 127.0.0.1:50052,127.0.0.1:50053
```

| model         |     size | params | backend  |  ngl |  test |          t/s |
| ------------- | -------: | -----: | -------- | ---: | ----: | -----------: |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |    5 | pp512 | 42.97 ± 1.03 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |    5 | tg128 | 14.51 ± 0.33 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   10 | pp512 | 46.15 ± 1.31 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   10 | tg128 | 12.55 ± 0.36 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   15 | pp512 | 50.66 ± 1.16 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   15 | tg128 | 12.22 ± 0.62 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   20 | pp512 | 54.53 ± 1.84 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   20 | tg128 | 13.42 ± 0.75 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   25 | pp512 | 59.29 ± 1.13 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   25 | tg128 | 13.26 ± 1.33 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   30 | pp512 | 62.20 ± 1.64 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   30 | tg128 | 14.25 ± 1.67 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   35 | pp512 | 62.21 ± 1.62 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   35 | tg128 | 14.30 ± 1.46 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   40 | pp512 | 62.20 ± 1.94 |
| llama 3B Q6_K | 2.45 GiB | 3.21 B | CUDA,RPC |   40 | tg128 | 14.37 ± 1.49 |

- 首 token 生成延迟测试

```bash
time ./bin/llama-cli -m ../models/Llama-3.2-3B-Instruct-Q6_K.gguf -p "What is love?" -n 1
```

| 1      | 2      | 3      | 4     | 5     |
| ------ | ------ | ------ | ----- | ----- |
| 3.229s | 2.412s | 2.657s | 2.924 | 2.816 |

### 性能分析

RPC 分布式部署允许我们将多个设备的算力与内存结合起来，实现分布式推理。我们期望这种方式可以最大化利用手中的硬件资源，实现算力和性能的提升。

然而我们通过以上测试结果可以看出，RPC 分布式部署得到的生成速度与单机版部署相比更低，这其实与我们的预期不符，原因分析如下：

- **通信开销**：分布式系统需要通过网络传输数据和控制信息。数据的序列化、传输、反序列化以及 RPC 协议自身的开销，都会消耗时间，导致整体计算速度变慢。
- **异构设备性能不匹配**：当模型任务被分割到速度差异悬殊的设备上时，整体性能由最慢的设备决定。
- **调度开销**：主控端需要花费时间来协调和分配模型层到不同的设备上。这种任务分配和同步的额外管理工作本身会带来计算负担，可能会使效率降低。

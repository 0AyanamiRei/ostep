
# Overview

程序```process-run.py```,允许你看到一个进程状态如何在 CPU 上运行时改变.如章节中所述，进程可以处于几种不同的状态：

```sh
RUNNING - the process is using the CPU right now
READY   - the process could be using the CPU right now
          but (alas) some other process is
BLOCKED - the process is waiting on I/O
          (e.g., it issued a request to a disk)
DONE    - the process is finished executing
```

在这个```homework```中,我们会看到当一共程序运行的时候,进程状态是如何变化的.

```sh
prompt> python3 process-run.py -h

Usage: process-run.py [参数]

Options:
  -h, --help                                显示帮助信息,然后退出程序

  -s xxx, --seed=xxx                        设置随机种子,你可以输入-s 123或者是-s --seed=123

  -l xxx, --processlist=xxx                 设置要运行的程序,按逗号分隔,每个程序有参数X:Y
                                            X表示进程运行的指令数量,Y指令使用CPU或者发出I/O的概率(0~100)

  -L xxx, --iolength=xxx                    设置I/O操作的持续事件

  -S xxx, --switch=xxx                      设置切换进程的时机, SWITCH_ON_IO,SWITCH_ON_END
                                            分别表示在I/O操作时切换和进程结束时切换
                                                               
  -I xxx, --iodone=xxx                      设置I/O结束时的行为类型,IO_RUN_LATER,IO_RUN_IMMEDIATE
                                            分别表示稍后运行和立即运行

  -c                                        输出答案

  -p, --printstats                          结束的时候打印统计信息,仅在使用-c时有用
```

```-l xxx```参数准确的指定了每个正在运行的进程将做什么,一个进程由指令组成,每个指令只能做下面的其中一件事:

- use the CPU  使用CPU
- issue an IO (and wait for it to complete)  进行I/O,并且等待它完成

当一共进程使用CPU且不进行I/O操作时,它会简单的在CPU上在状态```RUNNING```和```READY```之间切换, 下面是一个具体的例子：

```sh
prompt> python3 process-run.py -l 5:100 
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)

prompt> 
```

我们指定```-l```的参数为```5:100```这表示这个进程包含5条指令，百分百使用CPU

```sh
prompt> python3 process-run.py -l 5:100 -c
Time     PID: 0        CPU        IOs
  1     RUN:cpu          1
  2     RUN:cpu          1
  3     RUN:cpu          1
  4     RUN:cpu          1
  5     RUN:cpu          1
```

使用```-c```选项查看,能够看到进程在```RUN:cpu```状态下使用CPU,且没有进行I/O操作,下面我们运行一个更复杂一点的进程：

```sh
prompt> python3 process-run.py -l 5:100,5:100
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Process 1
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  Scheduler will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

在这个情况下，有两个不同的进程在运行，分别是```Process0,Process1```, 当操作系统运行它们的时候就会发生一些有趣的事情：

```sh
prompt> python3 process-run.py -l 5:100,5:100 -c
Time     PID: 0     PID: 1        CPU        IOs
  1     RUN:cpu      READY          1
  2     RUN:cpu      READY          1
  3     RUN:cpu      READY          1
  4     RUN:cpu      READY          1
  5     RUN:cpu      READY          1
  6        DONE    RUN:cpu          1
  7        DONE    RUN:cpu          1
  8        DONE    RUN:cpu          1
  9        DONE    RUN:cpu          1
 10        DONE    RUN:cpu          1
```

这很容易理解,首先运行PID=0的进程，然后PID=1的进程处于READY状态，当PID=0的进程运行结束，另一个进程就进入RUN状态
我们再来一个带了I/O的例子：
以下参数的含义```-l 3:0```运行三条指令，不使用CPU(那么就是进行I/O操作), ```-L 5```设置每次I/O的时间为5s

```sh
prompt> python3 process-run.py -l 3:0 -L 5
Produce a trace of what would happen when you run these processes:
Process 0
  io
  io_done
  io
  io_done
  io
  io_done

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

进一步查看运行的信息

```sh
prompt> python3 process-run.py -l 3:0 -L 5 -c
Time    PID: 0       CPU       IOs
  1         RUN:io             1
  2        BLOCKED                           1
  3        BLOCKED                           1
  4        BLOCKED                           1
  5        BLOCKED                           1
  6        BLOCKED                           1
  7*   RUN:io_done             1
  8         RUN:io             1
  9        BLOCKED                           1
 10        BLOCKED                           1
 11        BLOCKED                           1
 12        BLOCKED                           1
 13        BLOCKED                           1
 14*   RUN:io_done             1
 15         RUN:io             1
 16        BLOCKED                           1
 17        BLOCKED                           1
 18        BLOCKED                           1
 19        BLOCKED                           1
 20        BLOCKED                           1
 21*   RUN:io_done             1
```

解释一下输出的信息，进程进行了3个I/O, 每个I/O操作用时5个time，(可以看到处在```BLOCKED```状态的time跨度)
我们使用```-p```参数查看一下统计信息：

```sh
Stats: Total Time 21
Stats: CPU Busy 6 (28.57%)
Stats: IO Busy  15 (71.43%)
```

三条信息表明：
这个进程运行了21个时钟周期
使用CPU的时钟周期为6，占比百分之28.57
进行I/O操作的周期为15，占比百分之71.43

OK 开始你的作业吧！
```EI PSY CONGROO```

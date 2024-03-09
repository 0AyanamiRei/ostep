# Chapter-4

**Q1:以参数```-l 5:100,5:100```运行, CPU利用率应该为多少?**

A1:由于只有一个CPU,所以在运行第一个5:100的时候应该是百分比的占用CPU,此时第二个进程处于READY状态, 然后第一个进程结
   束, 处于DONE状态, 所以一共用了10个时钟周期, CPU处于忙碌的时钟周期是10, 那么利用率就是100.

```sh
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5        RUN:cpu         READY             1
  6           DONE       RUN:cpu             1
  7           DONE       RUN:cpu             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1

Stats: Total Time 10
Stats: CPU Busy 10 (100.00%)
Stats: IO Busy  0 (0.00%)
```

**Q2:以参数```-l 4:100,1:0```运行, 完成这两个进程需要多长时间?**

A2:(这里测试了一下```-L```参数的默认值是5, 也就是花费5个时钟周期去完成I/O)第一个进程会占用4个时钟周期,此时第二个
   进程应该可以并行的进行I/O操作, but, 请试着运行一下以下命令```python3 ./process-run.py -l 1:0 -c -L 1```你会看到

```sh
Time        PID: 0           CPU           IOs
  1         RUN:io             1
  2        BLOCKED                           1
  3*   RUN:io_done             1
```

说明对于一个I/O操作,进程会向cpu发送```I/O:发起```和```I/O:完成```这两个信息,所以我觉得是内核->CPU->进程来调度进程的状态
所以在这里, 第一个进程占用了CPU后,内核无法通过CPU向第二个进程发起开始I/O的命令,第二个进度只能等待,所以这里的运行情况应该是
进程1运行4个时钟周期, 然后进程2发起I/O请求,执行I/O命令,结束I/O请求, 消费1,5,1个时钟周期, 那么一共需要11个时钟周期,CPU忙碌了6时钟周期.

```sh
Time        PID: 0        PID: 1           CPU           IOs
  1        RUN:cpu         READY             1
  2        RUN:cpu         READY             1
  3        RUN:cpu         READY             1
  4        RUN:cpu         READY             1
  5           DONE        RUN:io             1
  6           DONE       BLOCKED                           1
  7           DONE       BLOCKED                           1
  8           DONE       BLOCKED                           1
  9           DONE       BLOCKED                           1
 10           DONE       BLOCKED                           1
 11*          DONE   RUN:io_done             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

**Q3:以参数```-l 1:0,4:100```运行, 完成这两个进程需要多长时间?并解释一下和Q2的差异**

A3:有了上面的分析之后就好很多了, 这里第一次发起I/O请求占用CPU一个时钟周期,紧跟着的I/O 5个周期中4个周期进程2并行的
   使用CPU
   所以一共只需要1+5+1,也就是进程2的完整时钟周期:7, 最大的差异就是参数为```4:100```的进程是否能在CPU空闲的时候使用以达到并行的目的.

```sh
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

**Q4:以参数```-l 1:0,4:100 -S SWITCH_ON_END```运行, 解释这会发生什么.**

A4: ```-S SWITCH_ON_END```的含义查文档后是:```在进程结束时切换```, 猜测意思就是不让并行?也就是从进程0切换到进程
    1必须要等待进程1结束才行,猜测跟Q2是一样的

```sh
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED         READY                           1
  3        BLOCKED         READY                           1
  4        BLOCKED         READY                           1
  5        BLOCKED         READY                           1
  6        BLOCKED         READY                           1
  7*   RUN:io_done         READY             1
  8           DONE       RUN:cpu             1
  9           DONE       RUN:cpu             1
 10           DONE       RUN:cpu             1
 11           DONE       RUN:cpu             1

Stats: Total Time 11
Stats: CPU Busy 6 (54.55%)
Stats: IO Busy  5 (45.45%)
```

**Q5:以参数```-l 1:0,4:100 -S SCHED_SWITCH_ON_IO```运行, 解释这会发生什么.**

A5:```-S SWITCH_ON_IO```表示在I/O操作时切换, 此时应该跟Q3相同,在进程0进行I/O操作的时候CPU空闲出来给进程1使用.

```sh
Time        PID: 0        PID: 1           CPU           IOs
  1         RUN:io         READY             1
  2        BLOCKED       RUN:cpu             1             1
  3        BLOCKED       RUN:cpu             1             1
  4        BLOCKED       RUN:cpu             1             1
  5        BLOCKED       RUN:cpu             1             1
  6        BLOCKED          DONE                           1
  7*   RUN:io_done          DONE             1

Stats: Total Time 7
Stats: CPU Busy 6 (85.71%)
Stats: IO Busy  5 (71.43%)
```

**Q6:以参数```-l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER```运行, 解释这会发生什么.**

A6: 解析一下参数吧, 创建4个子进程,第一个执行3次I/O, 后面3个子进程执行5次CPU操作,设置为在I/O操作数切换进程
```-I IO_RUN_LATER```意味着当I/O结束后进程会稍后执行, 具体的意思是说, 在进程进行完I/O任务后的```RUN:io_done```
的行为(这会占用CPU)不会立刻运行,而是```LATER```, 具体是等到多久,实验发现会等到所有进程结束后才进行(有可能不对)
所以我觉得会在第一个进程```RUN:io```后执行I/O操作,并行地紧跟着依次执行15条CPU指令,最后再执行```RUN:io_done```,然后剩下两次
I/O操作由于是在同一个进程发生的,所以执行完一次I/O以后不会说挂起```RUN:io_done```这个指令.

也就是说进程1的5次CPU指令和I/O操作同时发生了,所以时钟周期应该是17, 然后进程1继续完成两次I/O,用时14个时钟周期
一共用了31个时钟周期,其中CPU在前17次应该是时刻处于忙碌状态,后14次中只有4次```RUN:io```和```RUN:io_done```处于忙碌
那么CPU使用率 $=\frac{17+4}{31}$=67.74%, 我们来check一下:

```sh
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1
  2        BLOCKED       RUN:cpu         READY         READY             1             1
  3        BLOCKED       RUN:cpu         READY         READY             1             1
  4        BLOCKED       RUN:cpu         READY         READY             1             1
  5        BLOCKED       RUN:cpu         READY         READY             1             1
  6        BLOCKED       RUN:cpu         READY         READY             1             1
  7*         READY          DONE       RUN:cpu         READY             1
  8          READY          DONE       RUN:cpu         READY             1
  9          READY          DONE       RUN:cpu         READY             1
 10          READY          DONE       RUN:cpu         READY             1
 11          READY          DONE       RUN:cpu         READY             1
 12          READY          DONE          DONE       RUN:cpu             1
 13          READY          DONE          DONE       RUN:cpu             1
 14          READY          DONE          DONE       RUN:cpu             1
 15          READY          DONE          DONE       RUN:cpu             1
 16          READY          DONE          DONE       RUN:cpu             1
 17    RUN:io_done          DONE          DONE          DONE             1
 18         RUN:io          DONE          DONE          DONE             1
 19        BLOCKED          DONE          DONE          DONE                           1
 20        BLOCKED          DONE          DONE          DONE                           1
 21        BLOCKED          DONE          DONE          DONE                           1
 22        BLOCKED          DONE          DONE          DONE                           1
 23        BLOCKED          DONE          DONE          DONE                           1
 24*   RUN:io_done          DONE          DONE          DONE             1
 25         RUN:io          DONE          DONE          DONE             1
 26        BLOCKED          DONE          DONE          DONE                           1
 27        BLOCKED          DONE          DONE          DONE                           1
 28        BLOCKED          DONE          DONE          DONE                           1
 29        BLOCKED          DONE          DONE          DONE                           1
 30        BLOCKED          DONE          DONE          DONE                           1
 31*   RUN:io_done          DONE          DONE          DONE             1

Stats: Total Time 31
Stats: CPU Busy 21 (67.74%)
Stats: IO Busy  15 (48.39%)
```

**Q7:以参数```-l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE```运行, 解释跟Q6有什么差异,并说明为什么运行一个刚刚完成I/O的进程是一个好主意?**

A7: 分析一下Q6中不足,进程0后两次I/O操作时CPU完全就是在空闲的,系统资源没有被有效的利用,如果我们选择I/O操作完成后立刻继续执行,那么可以估计的
三次执行I/O操作时的CPU空档期应该全部能够并行运行进程1,2,3, 那么时钟周期完全由进程0决定-----21, 且CPU利用率应该是百分之百,check一下:

```sh
Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
  1         RUN:io         READY         READY         READY             1
  2        BLOCKED       RUN:cpu         READY         READY             1             1
  3        BLOCKED       RUN:cpu         READY         READY             1             1
  4        BLOCKED       RUN:cpu         READY         READY             1             1
  5        BLOCKED       RUN:cpu         READY         READY             1             1
  6        BLOCKED       RUN:cpu         READY         READY             1             1
  7*   RUN:io_done          DONE         READY         READY             1
  8         RUN:io          DONE         READY         READY             1
  9        BLOCKED          DONE       RUN:cpu         READY             1             1
 10        BLOCKED          DONE       RUN:cpu         READY             1             1
 11        BLOCKED          DONE       RUN:cpu         READY             1             1
 12        BLOCKED          DONE       RUN:cpu         READY             1             1
 13        BLOCKED          DONE       RUN:cpu         READY             1             1
 14*   RUN:io_done          DONE          DONE         READY             1
 15         RUN:io          DONE          DONE         READY             1
 16        BLOCKED          DONE          DONE       RUN:cpu             1             1
 17        BLOCKED          DONE          DONE       RUN:cpu             1             1
 18        BLOCKED          DONE          DONE       RUN:cpu             1             1
 19        BLOCKED          DONE          DONE       RUN:cpu             1             1
 20        BLOCKED          DONE          DONE       RUN:cpu             1             1
 21*   RUN:io_done          DONE          DONE          DONE             1

Stats: Total Time 21
Stats: CPU Busy 21 (100.00%)
Stats: IO Busy  15 (71.43%)
```

**Q7:运行一些随机生成的过程, 例如```-s 1 -l 3:50,3:50, -s 2 -l 3:50,3:50 -s 3 -l 3:50,3:50```改变```-I```和```-S```的参数看看会发生什么?尝试着去做预测**

A7: 这看起来有一点困难, ```-s x```是设置种子为x, 回忆一下```-l X:Y```X是指令的数量,Y可以看作有百分之Y的概率是CPU指令, 否则为I/O指令
分析结果的话,我觉得只能够得到一个范围,简单试着去分析一下:

- 考虑6个进程都运行CPU指令, 那么不管```-I```和```-S```参数如何设置,都应该是百分百的CPU利用率, 时钟周期为18
- 考虑6个进程都运行I/O, 是选择I/O操作时切换进程呢,还是进程结束后? 当I/O结束后,是延迟```RUN:io_done```到最后,还是选择立刻执行呢?
  切换进程在I/O时切换,可以并行的运行```RUN:io```,I/O结束后立刻运行```RUN:io_done```的话也可以在其他进程运行I/O的时候并行运行.
- 考虑更一般的情形,既有CPU指令又有I/O指令,首先切换进程肯定是要在I/O时切换的,这是毫无置疑的, I/O结束后立刻执行```RUN:io_done```对于"执行完当
  前这条I/O后就没有进程会执行I/O了"的情况,是没有影响的,因为它始终会占据一个时刻的cpu, 然而如果下一条指令依旧是I/O,那就可以并行运行了.

总结下来, 不管是如何,我们都要设置```I/O时切换进程```与```I/O结束后立刻执行RNN:is_done```才能提高CPU的利用率,下面我们测试一下两两组合的效果:

```sh
I/0时不切换+延迟执行     I/0时切换+延迟执行      I/0时不切换+立刻执行      I/0时切换+立刻执行
Total Time 18           Total Time 15         Total Time 18            Total Time 15
CPU Busy 8 (44.44%)     CPU Busy 8 (53.33%)   CPU Busy 8 (44.44%)      CPU Busy 8 (53.33%)
IO Busy  10 (55.56%)    IO Busy  10 (66.67%)  IO Busy  10 (55.56%)     IO Busy  10 (66.67%)

Total Time 30           Total Time 16         Total Time 30            Total Time 16
CPU Busy 10 (33.33%)    CPU Busy 10 (62.50%)  CPU Busy 10 (33.33%)     CPU Busy 10 (62.50%)
IO Busy  20 (66.67%)    IO Busy  14 (87.50%)  IO Busy  20 (66.67%)     IO Busy  14 (87.50%)

Total Time 24           Total Time 18         Total Time 24            Total Time 17
CPU Busy 9 (37.50%)     CPU Busy 9 (50.00%)   CPU Busy 9 (37.50%)      CPU Busy 9 (52.94%)
IO Busy  15 (62.50%)    IO Busy  11 (61.11%)  IO Busy  15 (62.50%)     IO Busy  11 (64.71%)
```

可以看到无论何时,I/O时切换进程+I/O结束后立刻执行指令的CPU,IO利用率都是最高的,总的时钟周期也是最少的.

ps: 后面如果像这样的数据分析多的话,应该考虑写一个脚本按上面格式打印输出了
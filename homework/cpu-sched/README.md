
# Overview

这个程序，scheduler.py，允许你看到在调度指标如响应时间、周转时间和总等待时间下，不同的调度器的性能如何。有三种调度器被"实现"：先进先出（FIFO），最短作业优先（SJF）和轮转（RR）。

运行程序有两个步骤。

首先，不带-c标志运行：这会显示你要解决的问题，但不会显示答案。例如，如果你想使用FIFO策略计算三个作业的响应、周转和等待时间，运行这个：

```sh
prompt> ./scheduler.py -p FIFO -j 3 -s 100
```

If that doesn't work, try this:
```sh
prompt> python ./scheduler.py -p FIFO -j 3 -s 100
```

这指定了三个作业的FIFO策略，并且重要的是，指定了一个特定的随机种子100。如果你想看到这个确切问题的解决方案，你必须再次指定这个完全相同的随机种子。让我们运行它看看会发生什么。你应该看到的是：

```sh
prompt> ./scheduler.py -p FIFO -j 3 -s 100
ARG policy FIFO
ARG jobs 3
ARG maxlen 10
ARG seed 100

Here is the job list, with the run time of each job: 
  Job 0 (length = 1)
  Job 1 (length = 4)
  Job 2 (length = 7)
```

计算每个作业的周转时间、响应时间和等待时间。当你完成后，再次运行这个程序，使用相同的参数，但是带上-c，这将为你提供答案。你可以使用-s 或你自己的作业列表（例如-l 10,15,20）为自己生成不同的问题。

从这个例子中你可以看到，生成了三个作业：长度为1的作业0，长度为4的作业1，长度为7的作业2。如程序所述，你现在可以使用这个来计算一些统计数据，看看你是否掌握了基本概念。

一旦你完成，你可以使用相同的程序来"解决"问题，看看你是否正确完成了你的工作。要做到这一点，使用"-c"标志。输出：

```sh
prompt> ./scheduler.py -p FIFO -j 3 -s 100 -c
ARG policy FIFO
ARG jobs 3
ARG maxlen 10
ARG seed 100

Here is the job list, with the run time of each job: 
  Job 0 (length = 1)
  Job 1 (length = 4)
  Job 2 (length = 7)

** Solutions **

Execution trace:
  [time   0] Run job 0 for 1.00 secs (DONE)
  [time   1] Run job 1 for 4.00 secs (DONE)
  [time   5] Run job 2 for 7.00 secs (DONE)

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 1.00  Wait 0.00
  Job   1 -- Response: 1.00  Turnaround 5.00  Wait 1.00
  Job   2 -- Response: 5.00  Turnaround 12.00  Wait 5.00

  Average -- Response: 2.00  Turnaround 6.00  Wait 2.00
```

从图中你可以看到，-c标志显示了发生了什么。作业0首先运行了1秒，作业1第二个运行了4秒，然后作业2运行了7秒。并不太难；毕竟，这是FIFO！执行跟踪显示了这些结果。

最后的统计数据也很有用：它们计算了"响应时间"（作业在首次运行前等待的时间），"周转时间"（从首次到达到完成作业所需的时间），和总的"等待时间"（任何在就绪但未运行的时间）。统计数据按作业显示，然后计算所有作业的平均值。当然，你应该在使用"-c"标志运行之前计算这些东西！

如果你想尝试相同类型的问题，但是输入不同，试着改变作业数量或随机种子，或者两者都改变。不同的随机种子基本上给你提供了一种为自己生成无数不同问题的方式，"-c"标志让你检查自己的工作。继续这样做，直到你觉得你真正理解了这些概念。

另一个有用的标志是"-l"（这是一个小写的L），它让你指定你希望看到的确切作业。例如，如果你想找出SJF在有三个长度为5、10和15的作业时的表现，你可以运行：

```sh
prompt> ./scheduler.py -p SJF -l 5,10,15
ARG policy SJF
ARG jlist 5,10,15

Here is the job list, with the run time of each job: 
  Job 0 (length = 5.0)
  Job 1 (length = 10.0)
  Job 2 (length = 15.0)
...
```

然后你可以再次使用-c来解决它。注意，当你指定确切的作业时，无需指定随机种子或作业数量：作业长度是从你的逗号分隔的列表中获取的。

当然，当你使用SJF（最短作业优先）或者RR（轮转）调度器时，会发生更有趣的事情。试试看吧！

你总是可以运行

```sh
prompt> ./scheduler.py -h
```

来获取完整的标志和选项列表（包括设置RR调度器的时间量子等选项）。

# ```Q&A&Notes```

## Chapter-7

对操作系统中运行的进程的假设

- 1：每个进程运行相同的时间
- 2：所有进程同时到达
- 3：所有进程一旦开始,便保持运行到结束
- 4：所有进程都使用CPU
- 5：每个工作的运行时间是已知的

在不断的放宽假设中，我们能看到不同的```调度策略Sheduling Policy```

### 条件(1,2,3,4,5): ```FIFO``` 先进先出

### 条件(2,3,4,5): ```SJF``` 最短任务优先

### 条件(4,5): ```STCF``` 最短完成时间优先

### 条件(1,2,3,4,5): ```FIFO``` 先进先出


### Q: ```任务的周转时间定义为任务完成时间减去任务到达系统的时间```, 其中任务到达系统消耗的时间是指什么?

- ```A```:猜测是排队等待的时间,比如在前面的模拟器实验中可以看到当```cpu```只有一个的时候, 我们对多个进程对```cpu```的请求就需要排队,如果将计算机启动的时间作为起始,那么作业```完成时间```-```到达时间```应该就是完成这个作业所需的最短时长.

### Q: 为什么计算平均周转时间用的是全局的时间?


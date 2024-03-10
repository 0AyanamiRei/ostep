# Linux Programmer's Manual---the wait function

```WAIT(2)```表明这是一个系统调用的函数

```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *wstatus, int options);
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

```wait(&wstatus)```与```waitpid(-1, &wstatus, 0);```是相等的,所以我们只看```waitpid```和```waitid```部分即可

## ```waitpid(pid_t pid, int *wstatus, int options)

```pid```的值说明了父进程调用后```wait set```的不同,具体来说有以下值

- ```pid <- 1```, 表明了组成等待集合的子进程所属的进程组等于```|pid|```
- ```pid == -1```,等待所有的子进程
- ```pid == 0```,假设父进程的进程组id等于1001,那么pid=0表示等待子集由所有进程组id等于1001的的子进程组成
- ```pid > 0```,等待进程ID为```pid```的子进程

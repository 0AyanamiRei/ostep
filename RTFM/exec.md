# Linux Programmer's Manual ----  the exec() family

```EXEC(3)```这是一个库函数, 使用之前请包含```#include <unistd.h>```

## ```-l```,```-v```,```-e```,```-p```不同变种之间的差距

- l
  - execl  ```int execl(const char *pathname, const char *arg, ... /* (char  *) NULL */);```
  - execlp ```int execlp(const char *file, const char *arg, ... /* (char  *) NULL */);```
  - execle ```int execle(const char *pathname, const char *arg, ... /*, (char *) NULL, char *const envp[] */);```
- v
  - execv ```int execv(const char *pathname, char *const argv[]);```
  - execvp ```int execvp(const char *file, char *const argv[]);```
  - execvpe ```int execvpe(const char *file, char *const argv[], char *const envp[]);```, 有一点特别,在使用这个函数之前要定义
  ```_GNU_SOURCE```宏

```l系列```和```v系列```函数区别在于前者接受一个```参数列表```后者接收一个```字符串数组```作为参数, 而参数列表和字符串数组的区别是,让我们用以```execl```和```execv```作为例子来说明:

```c
char *args[] = {"program_name", "arg1", "arg2", NULL};

/* 使用execv: */ execv("program_name", args);
/* 使用execl: */ execl("program_name", "program_name", "arg1", "arg2", NULL);
```

对于带有```e```系列的函数,允许使用```envp```这个二级指针参数作为新进程的环境,而非```e```系列的函数则使用当前进程的环境

```c
extern char **environ;
int main(int argc, char **argv, char **envp){
    char *myargv[] = {"program_name", "arg1", "arg2", NULL};
    char *myenvp[] = {"VAR1=value1", "VAR2=value2", NULL};

    execl ("program_name", "arg1", "arg2", );
    execle("program_name", "arg1", "arg2", environ);
    execle("program_name", "arg1", "arg2", envp);
    /* 这三者是等价的 */
}
```

全局变量```environ```就存储了当前进程的环境```envp```.

对于```p```系列的函数,大概差异是在函数如何寻找可执行文件时, 这一点不做详细说明,一般都带上就好了.


## ```execve()```用的更多的exec函数

```int execve(const char *pathname, char *const argv[], char *const envp[]);```

详细的参数可以在CSAPP上翻阅,因本人做shlab已经解除很多,比较熟悉了.

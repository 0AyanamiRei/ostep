#include "whatUneed.h"

/* 使用pipe将一个子进程的输出连接到另一个子进程的输入 */
/* 如何使用pipe？？

int fd[2];
pipe(fd): 将会让fd[0]变成读端， fd[1]变成写端, 如果再将fd[1]重定位到STDOUT_FILENO, fd[0]重定位到STDIN_FILENO
那就可以实现让调用写端的进程的printf的内容变成调用读端程序scanf、gets、fgets的输入内容了





*/
#define MAXLINE 128
#define READ_END	0
#define WRITE_END	1

int main()
{
    char write_msg[MAXLINE] = "Hello Makisa Kurusi!";
    char read_msg[MAXLINE];

    int fd[2];
    pid_t pid0;

    pipe(fd); /* Pipe is very cool!*/
    pid0 = fork();


    if(pid0) {
        pid_t pid1;
        pid1 = fork();

        if(!pid1){
            printf("PID1 = %d\n", getpid());
            close(fd[READ_END]);
            /* do something */ 
            dup2(fd[WRITE_END], STDOUT_FILENO);
            printf("%d", getpid());
            close(fd[WRITE_END]);
        }   
    }

    if(!pid0) {
        printf("PID0 = %d\n", getpid());
        close(fd[WRITE_END]);
        /* do something */
        dup2(fd[READ_END], STDIN_FILENO);
        pid_t pid;
        scanf("%d", &pid);
        printf("pid = %d\n", pid);
        close(fd[READ_END]);
    }
}
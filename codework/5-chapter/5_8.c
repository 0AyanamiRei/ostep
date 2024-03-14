#include "/home/refrain/ostep/codework/whatUneed.h"

/* 使用pipe将一个子进程的输出连接到另一个子进程的输入 */
/* 如何使用pipe？？

int fd[2];
pipe(fd): 将会让fd[0]变成读端， fd[1]变成写端, 如果再将fd[1]重定位到STDOUT_FILENO, fd[0]重定位到STDIN_FILENO
那就可以实现让调用写端的进程的printf的内容变成调用读端程序scanf、gets、fgets的输入内容了





*/
#define READ_END	0
#define WRITE_END	1
#define MAXLINE 128

int Fork() {
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "Usage: fork failure...\n");
        return -1;
    }
    return pid;
}

int
main(int argc, char *argv[])
{
    if(argc > 1){
        fprintf(2, "Usage: grep pattern [file ...]\n");
        exit(1);
    }

    int fd_Parent_2_Child[2];
    int fd_Child_2_Parent[2];
    pipe(fd_Parent_2_Child);
    pipe(fd_Child_2_Parent);
    char buf[MAXLINE];
    const char *ping = "ping";
    const char *pong = "pong";

    int pid = Fork();
    int ppid;

    if(!pid) { /* child */
        ppid = getpid();

        close(fd_Parent_2_Child[WRITE_END]);
        read(fd_Parent_2_Child[READ_END], buf, sizeof(ping)+1);
        printf("<%d>: received %s\n", ppid, buf);
        close(fd_Parent_2_Child[READ_END]);

        close(fd_Child_2_Parent[READ_END]);
        write(fd_Child_2_Parent[WRITE_END],pong, sizeof(pong)+1);
        close(fd_Child_2_Parent[WRITE_END]);



    } else { /* parent */
        /* you can do something here */
        ppid = getpid();

        close(fd_Parent_2_Child[READ_END]);
        write(fd_Parent_2_Child[WRITE_END],ping, sizeof(ping)+1);
        close(fd_Parent_2_Child[WRITE_END]);

        close(fd_Child_2_Parent[WRITE_END]);
        read(fd_Child_2_Parent[READ_END], buf, sizeof(pong)+1);
        printf("<%d>: received %s\n", ppid, buf);
        close(fd_Child_2_Parent[READ_END]);

        wait(pid);
    }
    
    exit(0);
}

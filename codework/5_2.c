#include <stdio.h>
#include <unistd.h> // for fork()
#include <fcntl.h>  // for open()
#include <sys/stat.h>  // for S_IRUSR and S_IWUSR
#include <string.h>

void write_something(const char * context) {
    int fd = open("test_for_5_2.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    write(fd, context, strlen(context));
    close(fd);
}


int main()
{
    pid_t pid = 0;
    //int fd;

    if((pid = fork()) == 0) {/* child */
        write_something("Makisa!");
    }else { /* parent */
        wait(pid);
        write_something("Kurusi!");
    }

    return 0;
}
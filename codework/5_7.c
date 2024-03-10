#include "whatUneed.h"

int main()
{
    pid_t pid;
    if((pid = fork()) == 0) {/* child */
       close(STDOUT_FILENO);
       printf("child!\n");
    }else { /* parent */
        printf("parent!\n");
    }
}
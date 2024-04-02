/*
    "不可控的调度"的一个例子
    你会看到很多不一样的结果:
    main: begin (counter = 0)
    A: begin
    B: begin
    A: done
    B: done
    main: done with both (counter = 10422300)

    main: begin (counter = 0)
    A: begin
    B: begin
    B: done
    A: done
    main: done with both (counter = 11298032)

    .......

    尝试使用锁去保证结果的可控性.

    在尝试后, 输出变的稳定了
    main: begin (counter = 0)
    A: begin
    A: done
    B: begin
    B: done
    main: done with both (counter = 20000000)

*/
#include <stdio.h>
#include <pthread.h>
#include <assert.h>


static volatile int counter = 0;
int rc;
pthread_mutex_t lock;

void *mythread(void *arg)
{
    pthread_mutex_lock(&lock);

    printf("%s: begin\n", (char *)arg);
    int i;
    for (i = 0; i < 1e7; i++)
    {
        counter = counter + 1;
    }
    printf("%s: done\n", (char *)arg);

    pthread_mutex_unlock(&lock);
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t p1, p2;

    rc = pthread_mutex_init(&lock, NULL);
    assert(rc == 0);

    printf("main: begin (counter = %d)\n", counter);

    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("main: done with both (counter = %d)\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}
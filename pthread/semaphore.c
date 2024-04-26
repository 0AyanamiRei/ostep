#include <semaphore.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define wait pthread_cond_wait
#define signal pthread_cond_signal
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define create pthread_create
#define join pthread_join


/*
        初始化的信号量   暂时默认为0:多线程共享     初始化值
sem_init (sem_t *__sem, int __pshared, unsigned int __value)

sem_wait (sem_t *__sem)
    如果信号量≥1, 那么立刻返回, 且让信号量减1
    如果信号量=0, 那么让线程挂起

sem_post (sem_t *__sem)
    让信号量的值加1,如果有等待线程,唤醒其中一个
*/

sem_t m2;
void *
thread_2(void *arg)
{
    printf("Thread\n");
    sem_post(&m2);
    return 0;
}


/* 二值信号量 X=1*/
void 
test1(int X)
{
    printf("test1:\n");
    sem_t m1;
    sem_init(&m1, 0, X);
    
    sem_wait(&m1);
    /* 临界区 */
    sem_post(&m1);
}

/* 信号量作为条件变量 */
void
test2(int X)
{
    printf("test2:\n");
    sem_init(&m2, 0, X);
    printf("begin\n");
    pthread_t c;
    create(&c, NULL, thread_2, NULL);
    sem_wait(&m2);
    printf("end\n");
}


/* 使用信号量来解决生产者消费者问题 */
/* 当MAXN=2的时候 看一看他是如何做到的 */

#define MAXN 5
int buffer[MAXN], fill = 0, use = 0;
sem_t empty, full, mutex;

void
put(int v)
{
    buffer[fill] = v;
    fill = (fill + 1) % MAXN;
}

int
get()
{
    int tmp = buffer[use];
    use = (use + 1) % MAXN;
    return tmp;
}

void*
P(void *arg)
{
    int i;
    for(i = 0; i < 5; i ++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        put(i);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void*
V(void *arg)
{
    int i, tmp = 0;
    for(i = 0; i < 5; i ++) {
        sem_wait(&full);
        sem_wait(&mutex);
        tmp = get();
        sem_post(&mutex);
        sem_post(&empty);
        printf("value is %d\n", tmp);
    }
}

void
test3()
{
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, MAXN);sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1); // 二值信号量=锁
    pthread_t prdr, csr;
    create(&prdr, NULL, P, NULL);
    create(&csr, NULL, V, NULL);
    join(prdr, NULL);
    join(csr, NULL);
}



int
main()
{
    // test1(1);
    // test2(0);
    test3();
    return 0;
}
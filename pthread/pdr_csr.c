#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define wait pthread_cond_wait
#define signal pthread_cond_signal
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock

int buffer;
int flag;
pthread_mutex_t lk;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int n;

void
put(int v)
{
    assert(flag == 0);
    flag = 1;
    buffer = v;
}

int
get()
{
    assert(flag == 1);
    flag = 0;
    return buffer;
}

void
*producer(void *arg)
{
    int i;
    int loop = *(int*)arg;
    for(i = 0; i < loop ; i ++){
        lock(&lk);
        while(flag == 1) {
            wait(&cond, &lk);
        }
        put(i+99);
        signal(&cond);
        unlock(&lk);
    }
}


void
*consumer(void *arg)
{
    int i;
    for(i = 0; i < n ; i ++){
        lock(&lk);
        while(flag == 0){
            wait(&cond, &lk);
        }
        int tmp = get();
        signal(&cond);
        unlock(&lk);
        printf("%d\n", tmp);
    }
}


int
main()
{
    flag = 0;
    pthread_t prdr1, prdr2, cser1, cser2;
    pthread_mutex_init(&lk, NULL);

    n = 5;
    pthread_create(&prdr1, NULL, producer, &n);
   // pthread_create(&prdr2, NULL, producer, &n);
    pthread_create(&cser1, NULL, consumer, &n);
    //pthread_create(&cser2, NULL, consumer, NULL);


    pthread_join(prdr1, &n);
   // pthread_join(prdr2, &n);
    pthread_join(cser1, &n);
    //pthread_join(cser2, NULL);

    return 0;
}

/* 生产者消费者问题
    如果光是加锁, 可以看到也无法阻止消费者会先执行的问题

    所以我们用上条件变量, 让队列中没东西的时候wait()
    BUG1: 如果有多个消费者,1个生产者
    BUG2: 如果有多个生成者,1个消费者
*/
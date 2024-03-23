# 线程

## phread_create

```c
#include <pthread.h>
pthread_t thread;
void* start_routine(void *);

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *( *start_routine) (void *), void *arg);

int return_value = pthread_create(&thread, NULL, start_routine, NULL);

if(return_value == 0)... // 成功
else if(return_value == EAGAIN)...
else if(return_value == EINVAL)...
else if(return_value == EPERM)...
```

函数```pthread_create()```在调用进程中以属性```attr->pthread_attr_t```和参数```arg```启动一个新线程, 在```start_routine()```中运行.

线程的返回方式:

- 从```start_routine()```中返回, 等同于```pthread_exit(3)```
- 调用```pthread_exit()```, 指定一个退出状态.
- 调用```pthread_cancel()```取消该线程
- 所属进程中的任何线程调用```exit()```, 或主线程从```main()```中返回,导致进程中所有线程终止.

在返回之前，```pthread_create()```的成功调用会将新线程的ID存储在由```thread```指向的```缓冲区```中;此标识符用于在对其他pthreads函数的后续调用中引用线程.

The new thread inherits copies  of  the  calling  thread's  ```capability  sets```  (see  capabilities(7))  and  ```CPU  affinity  mask``` (see
sched_setaffinity(2)).

## phread_join

```c
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);

pthread_t thread;
pthread_createo(&thread, ...); // 线程的返回值类型是(void *)

data_type retval;
pthread_join(thread, (void **)&retval); //指向void*变量的指针
```

```pthread_join()```函数等待由```thread```指定的线程终止, 如果该线程已经终止,那么```pthread_join()```立即返回.由```thread```指定的线程必须是```可连接的```.

如果```retval```不为```NULL```,那么```pthread_join()```将目标线程的退出状态（即:目标线程提供给```pthread_exit()```的值）复制到```retval```指向的位置,如果目标线程被取消,那么```PTHREAD_CANCELED```将被放置在```retval```指向的位置.

如果多个线程同时尝试与同一线程连接，结果是未定义的.

如果调用```pthread_join()```的线程被取消，那么目标线程将保持可连接状态.

成功时,```pthread_join()```返回0, 错误时, 它返回一个错误号.

## pthread_mutex_lock

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

**互斥量**有四种, 每种类型的互斥量在尝试重新锁定```Relock```或者在非所有者尝试解锁```Unlock When Not Owner```时的行为都不同

- NORMAL
- ERRORCHECK
- RECURSIVE
- DEFAULT

pthread_mutex_trylock与pthread_mutex_lock类似,但是前者如果互斥量已经被锁定, 那么就会立刻返回
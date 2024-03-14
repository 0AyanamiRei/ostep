/*
    尝试着模拟一下MLFQ, 体会一下不断的改进中优化了什么
    just for fun !
*/
#include "/home/refrain/ostep/codework/whatUneed.h"

#define chunk 2     // 为每一级队列每一个进程分配的一个时间配额
#define N 8         // MLFQ划分的等级数量
#define S 12        // 经过S时间后,boost一次工作的等级

int chunkLv[N];     // 改进: 为每一级队列都定义不同的时间配额

typedef struct Job
{
    pid_t pid;
    int priLv;
    int wkTime;
    int leftTime;
}Job;

typedef struct Node {
    Job job;
    struct Node *next;
} Node;


/******************链式队列*********************/
typedef struct Queue {
    Node *front;
    Node *rear;
} Queue;

Queue*
initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void
enqueue(Queue* q, Job value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->job = value;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

Job 
dequeue(Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty.\n");
        Job emptyJob;
        emptyJob.pid = -1;
        emptyJob.priLv = -1;
        return emptyJob;
    }
    Node* temp = q->front;
    Job value = temp->job;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return value;
}

int
isEmpty(Queue* q) {
    return q->front == NULL;
}
/******************链式队列*********************/

/******************MLFQ*********************/

Queue **
initMLFQ(int n) {
    Queue * MLFQ[n];
    for(int i = 0; i < n; ++i)
        MLFQ[i] = initQueue();
    return MLFQ;
}


int
main()
{
    Queue **MLFQ = initMLFQ(N);
    
    exit(0);
}

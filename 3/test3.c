#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define N 100
#define producerNum  15
#define consumerNum  5
#define true 1
#define sleepTime 2

typedef int semaphore;//信号量类型为int
typedef int item;//产品类型为int
item buffer[N] = {0};
int in = 0;
int out = 0;
int proCount = 0;
semaphore mutex = 1, empty = N, full = 0, proCmutex = 1;

void * producer(void * a){
    while(true){
        while(proCmutex <= 0);//等待生产的信号量
        proCmutex--;
        proCount++;
        printf("生产一个产品ID%d, 缓冲区位置为%d\n",proCount,in);
        proCmutex++;

        while(empty <= 0){
            printf("缓冲区已满！\n");
        }
        empty--;

        while(mutex <= 0);//等待操作缓冲区的信号量
        mutex--;

        buffer[in] = proCount;//生产者将产品放入缓冲区内
        in = (in + 1) % N;//产品数量加1

        mutex++;
        full++;
        sleep(sleepTime);//使生产者和消费者的速度同步
    }
}

void * consumer(void *b){
    while(true){
        while(full <= 0){
            printf("缓冲区为空！\n");
        }
        full--;

        while(mutex <= 0);//等待缓冲区的使用
        mutex--;

        int nextc = buffer[out];//消费者从缓冲区中拿出产品
        buffer[out] = 0;//消费完将缓冲区设置为0

        out = (out + 1) % N;

        mutex++;
        empty++;

        printf("\t\t\t\t消费一个产品ID%d,缓冲区位置为%d\n", nextc,out);
        sleep(sleepTime);//使消费者和生产者的速度同步
    }
}

int main()
{
    pthread_t threadPool[producerNum+consumerNum];
    int i;
    for(i = 0; i < producerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, producer, NULL) == -1){
            printf("ERROR, fail to create producer%d\n", i);
            exit(1);
        }
        threadPool[i] = temp;
    }//创建生产者进程放入线程池


    for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, consumer, NULL) == -1){
            printf("ERROR, fail to create consumer%d\n", i);
            exit(1);
        }
        threadPool[i+producerNum] = temp;
    }//创建消费者进程放入线程池


    void * result;
    for(i = 0; i < producerNum+consumerNum; i++){
        if(pthread_join(threadPool[i], &result) == -1){
            printf("fail to recollect\n");
            exit(1);
        }
    }//运行线程池
    return 0;
}

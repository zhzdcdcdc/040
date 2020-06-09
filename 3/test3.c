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

typedef int semaphore;//�ź�������Ϊint
typedef int item;//��Ʒ����Ϊint
item buffer[N] = {0};
int in = 0;
int out = 0;
int proCount = 0;
semaphore mutex = 1, empty = N, full = 0, proCmutex = 1;

void * producer(void * a){
    while(true){
        while(proCmutex <= 0);//�ȴ��������ź���
        proCmutex--;
        proCount++;
        printf("����һ����ƷID%d, ������λ��Ϊ%d\n",proCount,in);
        proCmutex++;

        while(empty <= 0){
            printf("������������\n");
        }
        empty--;

        while(mutex <= 0);//�ȴ��������������ź���
        mutex--;

        buffer[in] = proCount;//�����߽���Ʒ���뻺������
        in = (in + 1) % N;//��Ʒ������1

        mutex++;
        full++;
        sleep(sleepTime);//ʹ�����ߺ������ߵ��ٶ�ͬ��
    }
}

void * consumer(void *b){
    while(true){
        while(full <= 0){
            printf("������Ϊ�գ�\n");
        }
        full--;

        while(mutex <= 0);//�ȴ���������ʹ��
        mutex--;

        int nextc = buffer[out];//�����ߴӻ��������ó���Ʒ
        buffer[out] = 0;//�����꽫����������Ϊ0

        out = (out + 1) % N;

        mutex++;
        empty++;

        printf("\t\t\t\t����һ����ƷID%d,������λ��Ϊ%d\n", nextc,out);
        sleep(sleepTime);//ʹ�����ߺ������ߵ��ٶ�ͬ��
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
    }//���������߽��̷����̳߳�


    for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, consumer, NULL) == -1){
            printf("ERROR, fail to create consumer%d\n", i);
            exit(1);
        }
        threadPool[i+producerNum] = temp;
    }//���������߽��̷����̳߳�


    void * result;
    for(i = 0; i < producerNum+consumerNum; i++){
        if(pthread_join(threadPool[i], &result) == -1){
            printf("fail to recollect\n");
            exit(1);
        }
    }//�����̳߳�
    return 0;
}

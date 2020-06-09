#include<stdio.h>

#include <stdlib.h>

#include<signal.h>

#include<unistd.h>

#include<sys/types.h>

#include<sys/wait.h>

#include<sys/stat.h>

#include <errno.h>

int wait_mark;                           //�����ź�wait_mark,Ϊ0ʱ����
int p1,p2;


void stop()

{

    wait_mark=0;

}

void killer()

{

    wait_mark=0;

    kill(p1,16);            	  //��p1�����ж��ź�16

    kill(p2,17);         	  //��p2�����ж��ź�17

}

void main()
{

    

    if(p1=fork())             		  //�����ӽ���p1

    {

        if(p2=fork())                     //�����ӽ���p2
                           
        {  //������ִ�еĴ���                               

            wait_mark=1;

	    

            signal(SIGINT,killer);          //���յ�^c�źţ�תstop

	    signal(SIGINT,SIG_IGN);        //���Լ��ж�^c�ź�

	    signal(SIGQUIT,SIG_IGN);       //�����ж�quit���ź�

	    //pause();

            wait(NULL);              	  //ͬ�����ȴ�����p1��p2ִ�����

            wait(NULL);

            printf("parent(pid=%d) process is killed!\n",getpid());

            exit(0);

        }

        else                              

        {   //P2����ִ�еĴ���

            wait_mark=1;

            signal(17,stop);

            signal(SIGINT,SIG_IGN);        //���Լ��ж�^c�ź�

	    signal(SIGQUIT,SIG_IGN);       //�����ж�quit���ź�

            while (wait_mark!=0);

            lockf(1,1,0);		   //��ȡ��

            printf("child process2(pid=%d ,ppid=%d) is killed by parent!\n",getpid(),getppid());

            lockf(1,0,0);                  //�ͷ���

            exit(0);

        }

    }

    else

    {   //P1����ִ�еĴ���

    wait_mark=1;

    signal(16,stop);			   //�յ����ж��źź�ִ��stop

    signal(SIGINT,SIG_IGN);                //���Լ�^c�ź�

    signal(SIGQUIT,SIG_IGN);  		   //�����ж�quit���ź�

    while (wait_mark!=0);

    lockf(1,1,0);			   //��ȡ��

    printf("child process1(pid=%d ,ppid=%d) is killed by parent!\n",getpid(),getppid());

    lockf(1,0,0);			   //�ͷ���

    exit(0);

    }

}








#include<stdio.h>

#include <stdlib.h>

#include<signal.h>

#include<unistd.h>

#include<sys/types.h>

#include<sys/wait.h>

#include<sys/stat.h>

#include <errno.h>

int wait_mark;                           //触发信号wait_mark,为0时触发
int p1,p2;


void stop()

{

    wait_mark=0;

}

void killer()

{

    wait_mark=0;

    kill(p1,16);            	  //向p1发软中断信号16

    kill(p2,17);         	  //向p2发软中断信号17

}

void main()
{

    

    if(p1=fork())             		  //创建子进程p1

    {

        if(p2=fork())                     //创建子进程p2
                           
        {  //父进程执行的代码                               

            wait_mark=1;

	    

            signal(SIGINT,killer);          //接收到^c信号，转stop

	    signal(SIGINT,SIG_IGN);        //忽略键中断^c信号

	    signal(SIGQUIT,SIG_IGN);       //忽略中断quit键信号

	    //pause();

            wait(NULL);              	  //同步，等待进程p1和p2执行完成

            wait(NULL);

            printf("parent(pid=%d) process is killed!\n",getpid());

            exit(0);

        }

        else                              

        {   //P2进程执行的代码

            wait_mark=1;

            signal(17,stop);

            signal(SIGINT,SIG_IGN);        //忽略键中断^c信号

	    signal(SIGQUIT,SIG_IGN);       //忽略中断quit键信号

            while (wait_mark!=0);

            lockf(1,1,0);		   //获取锁

            printf("child process2(pid=%d ,ppid=%d) is killed by parent!\n",getpid(),getppid());

            lockf(1,0,0);                  //释放锁

            exit(0);

        }

    }

    else

    {   //P1进程执行的代码

    wait_mark=1;

    signal(16,stop);			   //收到软中断信号后执行stop

    signal(SIGINT,SIG_IGN);                //忽略键^c信号

    signal(SIGQUIT,SIG_IGN);  		   //忽略中断quit键信号

    while (wait_mark!=0);

    lockf(1,1,0);			   //获取锁

    printf("child process1(pid=%d ,ppid=%d) is killed by parent!\n",getpid(),getppid());

    lockf(1,0,0);			   //释放锁

    exit(0);

    }

}








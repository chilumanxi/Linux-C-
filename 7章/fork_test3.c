/*************************************************************************
	> File Name: fork_test3.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月23日 星期四 09时54分40秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<grp.h>
#include<pwd.h>
#include<limits.h>
#include<time.h>
#include<dirent.h>

int main(void){
    pid_t pid;

    pid = fork();
    switch(pid){
        case 0:
            while(1){
                printf("A background process, PID:%d, ParentID: %d\n", getpid(), getppid());
                sleep(3);
            }
        case -1:
            perror("Process creation failed\n");
            exit(-1);
        default :
            printf("I am parent process, mypid is %d\n", getpid());
            exit(0);
    }

    return 0;
}

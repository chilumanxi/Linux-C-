/*************************************************************************
	> File Name: fork_test2.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月23日 星期四 09时11分19秒
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
    char *msg;
    int k;

    printf("Process Creation Study\n");
    pid = fork();
    switch(pid){
        case 0:
            msg = "Child process is running";
            k = 3;
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
            msg = "Parent process is running";
            k = 5;
            break;
    }

    while(k > 0){
        puts(msg);
        sleep(1);
        k --;
    }

    exit(0);

}


/*
 * 运行结果：
 * Process Creation Study
 * Parent process is running
 * Child process is running
 * Parent process is running
 * Child process is running
 * Child process is running
 * Parent process is running
 * Parent process is running
 * Parent process is running
 *
 * 解释：子进程和父进程交替执行。
 * 另外：fork创建进程失败通常是父进程拥有的子进程的个数超过了规定的限制。此时errno值为EAGAIN。如果可供使用的内存不足也会导致进程创建失败。此时errno值为ENOMEM
 */

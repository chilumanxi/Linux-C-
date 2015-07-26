/*************************************************************************
	> File Name: solve_zombie_by_fork_twice.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 14时35分50秒
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


int main(){
    pid_t pid;
    pid = fork();

    if(pid < 0){
        perror("fork error");
        exit(1);
    }
    //第一个子进程
    else if(pid == 0){
        printf("i am the first child process pid:%d\tppid:%d\n", getpid(), getppid());
        //子进程再次创建进程 
        pid = fork();
        if(pid < 0){
            perror("fork error");
            exit(1);
        }
        //第一个子进程退出
        else if(pid > 0){
            printf("first process is existed\n");
            exit(0);
        }
        //第二个子进程睡眠3s保证第一个子进程退出，这样第二个子进程的父进程就是init
        sleep(3);
        printf("i am the second child process.pid : %d\tppid:%d\n", getpid(), getppid());
        exit(0);
    }
    //父进程处理第一个子进程退出
    if(waitpid(pid, NULL, 0) != pid){
        perror("waitpid error");
        exit(1);
    }
    exit(0);
    return 0;
}

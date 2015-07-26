/*************************************************************************
	> File Name: solve_zombie_by_signal.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 14时22分02秒
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
#include <signal.h>

static void sig_child(int signo);

int main(){
    pid_t pid;
  //  signal(SIGCHLD, sig_child);
    pid = fork();
    if(pid < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid == 0){
        printf("i am child process, pid id :%d  i am existing\n", getpid());
        exit(0);
    }

    printf("i am father process. i will sleep two seconds\n");

    sleep(2);

    system("ps -o pid,ppid,state,tty,command");
    printf("father process is exiting\n");
    return 0;
}

static void sig_child(int signo){
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    printf("child %d terminated.\n", pid);
}

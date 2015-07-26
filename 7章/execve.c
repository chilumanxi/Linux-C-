/*************************************************************************
	> File Name: execve.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 19时06分11秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <limits.h>
#include <time.h>
#include <dirent.h>

int main(int argc, char *argv[], char **environ){
    pid_t pid;
    int stat_val;

    printf("Exec example!\n");
    pid = fork();
    switch(pid){
        case -1:
            perror("Process Creation failed\n");
            exit(1);
        case 0:
            printf("Child process is runnig\n");
            printf("My pid = %d, parentpid = %d\n", getpid(), getppid());
            printf("uid = %d, gid = %d\n", getuid(), getgid());
            execve("processimage", argv, environ);
            printf("process never go to here!\n");
            exit(0);
        default:
            printf("parent process is running\n");
            break;
    }
    wait(&stat_val);
    exit(0);
}

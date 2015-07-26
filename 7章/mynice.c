/*************************************************************************
	> File Name: mynice.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月25日 星期六 14时54分21秒
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
#include <sys/resource.h>
#include <sys/wait.h>

int main(void){
    pid_t pid;
    int stat_val = 0;
    int oldpri, newpri;

    printf("nice study\n");

    pid = fork();
    switch(pid){
        case 0:
            printf("Child is running, Curpid is %d, Parentpid is %d\n", getpid(), getppid());
            
            oldpri = getpriority(PRIO_PROCESS, 0);
            printf("Old priority = %d\n", oldpri);

            newpri = nice(2);
            printf("New priority = %d\n", newpri);

            exit(0);
        case -1:
            perror("Process creation failed\n");
            break;
        default:
            printf("Parent is running, ChildPid is %d, ParentPid is %d\n", pid, getpid());
            printf("Parent priority is %d\n", getpriority(PRIO_PROCESS, getpid()));
            break;
    }

    wait(&stat_val);
    exit(0);
}


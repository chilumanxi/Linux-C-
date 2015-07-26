/*************************************************************************
	> File Name: getpid.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月25日 星期六 11时08分30秒
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

int main(void){
    pid_t pid;
    if((pid = fork()) == -1){
        printf("perror error!\n");
        exit(1);
    }

    if(pid == 0)
        printf("getpid return %d\n", getpid());

    exit(0);
}

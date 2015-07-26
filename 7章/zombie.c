/*************************************************************************
	> File Name: zombie.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 14时14分07秒
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

    while(1){
        pid = fork();
        if(pid < 0){
            perror("fork error");
            exit(1);
        }
        else if(pid == 0){
            printf("i am a child process\n i am exiting\n");
            exit(0);
        }
        else{
            sleep(20);
            continue;
        }
    }
    return 0;
}

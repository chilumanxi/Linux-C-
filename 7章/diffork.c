/*************************************************************************
	> File Name: diffork.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月23日 星期四 19时06分39秒
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

int globVar = 5;

int main(void){
    pid_t pid;
    int var = 1, i;
    
    printf("fork is different with vfork\n");

    //pid = fork();
    pid = vfork();
    switch(pid){
        case 0:
            i = 3;
            while(i -- > 0){
                printf("Child process is running\n");
                globVar ++;
                var ++;
                sleep(1);
            }
            printf("Child's globVar = %d, var = %d\n", globVar, var);
            break;
        case -1:
            perror("process creation failes\n");
            exit(0);
        default :
            i = 5;
        while(i -- > 0){
            printf("Parent process is running\n");
            globVar ++;
            var ++;
            sleep(1);
        }
        printf("Parent's globVar = %d, var = %d\n", globVar, var);
    }
    exit(0);
}

/*
 * 运行结果：
 * pid = fork();
 * fork is different with vfork
 * Parent process is running
 * Child process is running
 * Parent process is running
 * Child process is running
 * Child process is running
 * Parent process is running
 * Child's globVar = 8, var = 4
 * Parent process is running
 * Parent process is running
 * Parent's globVar = 10, var = 6
 *
 *
 * pid = vfork()
 * fork is different with vfork
 * Child process is running
 * Child process is running
 * Child process is running
 * Child's globVar = 8, var = 4
 * Parent process is running
 * Parent process is running
 * Parent process is running
 * Parent process is running
 * Parent process is running
 * Parent's globVar = 13, var = 32770
 */

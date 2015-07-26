/*************************************************************************
	> File Name: processimage.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 21时55分58秒
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
    int i;

    printf("i am a process image!\n");
    printf("My pid = %d, Parentpid = %d\n", getpid(), getppid());
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    for(i = 0; i < argc; i ++){
        printf("argv[%d]:%s\n", i, argv[i]);
    }
}

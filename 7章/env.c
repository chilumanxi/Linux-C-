/*************************************************************************
	> File Name: env.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月24日 星期五 17时32分15秒
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
#include <malloc.h>

extern char **environ;

int main(int argc, char *argv[]){
    int i;
    printf("Argument :\n");
    for(i = 0; i < argc; i ++){
        printf("argv[%d] is %s\n", i, argv[i]);
    }

    printf("Environment :\n");

    for(i = 0; environ[i] != NULL; i ++){
        printf("%s\n", environ[i]);
    }

    return 0;
}

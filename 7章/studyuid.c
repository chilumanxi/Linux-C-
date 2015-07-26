/*************************************************************************
	> File Name: studyuid.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年07月25日 星期六 11时22分44秒
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

extern int errno;

int main(){
    int fd;

    printf("uid study:\n");
    printf("Process's uid = %d, euid = %d\n", getuid(), geteuid());

    //strerror 函数获取指定错误码信息

    if((fd = open("test.c", O_RDWR)) == -1){
        printf("Open failure, errno is %d :%s\n", errno, strerror(errno));
    }
    else{
        printf("Open successfully!\n");
    }
    close(fd);
    exit(0);
    return 0;

}

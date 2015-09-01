/*************************************************************************
	> File Name: test1.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年08月07日 星期五 20时56分56秒
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
    /*
    struct stat buf;
    lstat("testtext", &buf);
    if(buf.st_size == 0)
        printf("test ok \n");
    
    printf("%d\n", buf.st_size);*/
    FILE *fp;
    char buf[1024];
    fp = fopen("testtext", "rb+");
    fseek(fp, 0, SEEK_END);
    //if(ftell(fp) == 0)    
    //printf("test ok\n");
    /*
    fread(buf, 1024, 1, fp);
    if(!strcmp(buf, ""))
        printf("test ok!");
    printf("%s", buf);
    */
    if(feof(fp))
        printf("test ok!\n");
    
    return 0;
}

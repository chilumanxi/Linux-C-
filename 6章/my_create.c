/*************************************************************************
    > File Name: my_create.c
    > Author: zhanghaoran
    > Mail: 467908670@qq.com 
    > Created Time: 2015年05月25日 星期一 20时49分30秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int fd;
	if((fd = open("example_62.c", O_CREAT|O_EXCL, S_IRUSR| S_IWUSR)) == -1){
		printf("open:%s with errno:%d\n",strerror(errno),errno);
		//perror("open");
		exit(1);
	}
	else{
		printf("create file success\n");
	}
	close(fd);
	return 0;
}

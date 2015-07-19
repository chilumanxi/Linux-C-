/*************************************************************************
    > File Name: my_mv.c
    > Author: zhanghaoran
    > Mail: chilumanxi@gmail.com 
    > Created Time: 2015年07月19日 星期日 19时02分58秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
	if(argc < 3){
		printf("my_mv <old file> <new file>\n");
		exit(0);
	}
	if(rename(argv[1], argv[2]) < 2){
		perror("rename");
		exit(1);
	}
	return 0;
}

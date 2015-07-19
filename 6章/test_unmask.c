/*************************************************************************
    > File Name: test_unmask.c
    > Author: zhanghaoran
    > Mail: chilumanxi@gmail.com 
    > Created Time: 2015年07月19日 星期日 17时43分11秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(){
	umask(0);
	if(creat("example_681.test", S_IRWXU|S_IRWXO) < 0) {
		perror("creat");
		exit(1);
	}
	
	umask(S_IRWXO);
	if(creat("example_682.test", S_IRWXU|S_IRWXO) < 0){
		perror("creat");
		exit(1);
	}

	return 0;
}

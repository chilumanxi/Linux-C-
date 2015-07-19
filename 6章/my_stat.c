/*************************************************************************
    > File Name: my_stat.c
    > Author: zhanghaoran
    > Mail: chilumanxi@gmail.com 
    > Created Time: 2015年07月19日 星期日 17时05分21秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
int main(int argc, char **argv){
	struct stat buf;
	if(argc < 2){
		printf("Usage: my_stat <filename>\n");
		exit(0);
	}

	if(stat(argv[1], &buf) == -1){
		perror("stat:");
		exit(1);
	}

	printf("device is: %d\n", buf.st_dev);
	printf("inode is; %d\n", buf.st_ino);
	printf("mode is: %d\n", buf.st_mode);
	printf("number is: %d\n", buf.st_nlink);
	printf("user ID of owner is: %d\n", buf.st_uid);
	printf("group ID of owner is: %d\n", buf.st_gid);
	printf("device type (if inode device) is: %d\n", buf.st_rdev);

	printf("total size, in bytes is: %d\n",buf.st_size);
	printf("blocksize for filesystem I/O is: %d\n", buf.st_blksize);
	printf("number of blocks allocated is: %d\n", buf.st_blocks);
	
	printf("time of last access is: %s", ctime(&buf.st_atime));
	printf("time of last modification is: %s", ctime(&buf.st_mtime));
	printf("time of last change is: %s", ctime(&buf.st_ctime));
	
	return 0;
}

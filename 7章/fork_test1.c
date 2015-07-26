/*************************************************************************
    > File Name: fork_test1.c
    > Author: zhanghaoran
    > Mail: chilumanxi@gmail.com 
    > Created Time: 2015年07月22日 星期三 11时33分18秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main(void)
{
	pid_t pid;

	printf("Process Creation Study\n");
	
	pid = fork();
	switch(pid){
		case 0: 
			printf("Child process is running, Curpid is %d, ParentPid is %d\n", pid, getppid());
			break;
		case -1:
			perror("Process creation failed\n");
			break;
		default:
			printf("Parent process is running , ChildPid is %d, ParentPid is %d\n", pid, getpid());
			break;
	}
	exit(0);
    
}



/*
 * 运行结果:
 * Process Creation Study
 * Parent process is running , ChildPid is 8658, ParentPid is 8657
 * Child process is running, Curpid is 0, ParentPid is 8657
 *
 * 解释:
 * 进程创建成功以后,fork返回两次,一次返回为0,代表子进程正在运行,通过函数getpid得到其父进程的ID为8657,子进程为8658.父进程正在进行
 */

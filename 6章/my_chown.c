/*************************************************************************
    > File Name: my_chown.c
    > Author: zhanghaoran
    > Mail: chilumanxi@gmail.com 
    > Created Time: 2015年07月19日 星期日 10时42分41秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
int main(int argc, char **argv){
	struct passwd *npasswd = NULL;
	struct group *ngroup = NULL;
	char username[30];
	char groupname[30];
	int i, j;
	int gid, uid;
	if(argc < 3){
		printf("%s <user>:<group> <filename>\n", argv[0]);
		exit(0);
	}
	for(i = 0; argv[1][i] != ':'; i ++){
		username[i] = argv[1][i];
	}
	username[i] = '\0';
	i ++;
	for(j = 0; argv[1][i] != '\0'; i ++, j ++){
		groupname[j] = argv[1][i];
	}
	groupname[j] = '\0';
	if((npasswd = getpwnam(username)) == NULL){
		printf("username dose not exist!\n");
		exit(0);
	}
	if((ngroup = getgrnam(groupname)) == NULL){
		printf("groupname dose not exist!\n");
	}
	if(access(argv[2], F_OK) != 0){
		printf("file %s does not exist!\n", argv[2]);
	}
	uid = npasswd -> pw_uid;
	gid = ngroup -> gr_gid;
	if(chown(argv[2], uid, gid) < 0){
		perror("chown error");
		exit(1);
	}
	printf("the user id and group id of the file has been changed successfully\n");
	return 0;
	
	
}

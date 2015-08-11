#ifndef __SERVER_H
#define __SERVER_H

#include"lib.h"
#include<pthread.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define BACKLOG 100
#define MAXPEOPLE 100
#define NAMELEN 10
#define PWDLEN 20
#define SAVEMOST 1024
#define PATHLEN 128
#define FD_ADDR_CLR(a) ((a)->fd) = -1;\
		(a)->addr.sin_addr.s_addr = 0;\
		 memset((a)->user_name, 0, 10);\
		(a)->room_flag = 0;
		
#define DEBUG_VERSION

typedef struct 
{
	struct sockaddr_in addr;
	char user_name[NAMELEN];
	int fd;
	int room_flag;   //flag=1:in chatroom; flag=2:in private chat
}fd_addr;


struct info
{
	char name[NAMELEN];
	char pwd[PWDLEN];
    char age[3];
    char phonenum[PWDLEN];
    char friends[MAXPEOPLE][NAMELEN];
    char friendsnum[4];
	struct info* next;
};

fd_addr client[MAXPEOPLE];
struct info* file_info;
struct timeval timeout = {3,0};
pthread_mutex_t sock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

void server();
void* msg_processor(void*);
int login(fd_addr*);
void chat_with_all(fd_addr*);
//void chat_with_one(fd_addr*);
void send_to_all(char* ,fd_addr*);
void load_info();
void save_info(struct info*);
void add_in_list(struct info *, const char *, const char *, const char *, const char *);
void add_friend(fd_addr *);
void send_invatation(fd_addr*, const char *);
void chat_with_one(fd_addr *);
#endif

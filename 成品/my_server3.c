/*************************************************************************
	> File Name: my_server3.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年08月04日 星期二 09时47分42秒
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
#include <arpa/inet.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>

typedef struct login{
    char username[30];
    char ip[30];
    struct login *prev;
    struct login *next;
}Linklist;

pthread_mutex_t sock_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t serve_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

Linklist *head = NULL;

Linklist *createlist(){
    return (Linklist *)calloc(1, sizeof(Linklist));
}

int init_head(Linklist *head){
    if(head == NULL)
        return 1;
    head -> prev = head;
    head -> next = head;
    return 0;
}

int len_list(Linklist *head){
    int len = 0;
    Linklist *p;
    if(head == NULL)
        return 0;
    p = head -> next;
    while(p != head){
        len ++;
        p = p -> next;
    }
    return len;
}

int insert_end(Linklist *head, Linklist *p){
    if(p == NULL || head == NULL)
        return -1;
    p -> prev = head -> prev;
    p -> next = head;
    head -> prev -> next = p;
    head -> prev = p;
    return 0;
}

int clear_list(Linklist *head){
    Linklist *temp = NULL;
    Linklist *p = NULL;
    if(head == NULL)
        return 1;
    temp = head -> next;
    while(temp != head){
        p = temp -> next;
        free(temp);
        temp = p;
    }
    head -> prev = head;
    head -> next = head;
    return 0;
}

Linklist *find_username_exist(Linklist *head, char names[30]){
    Linklist *p = NULL;
    if(head == NULL)
        return NULL;
    p = head -> next;
    while(p != head){
        if(strcmp(p -> username, names) == 0)
            return p;
        p = p -> next;
    }
    return NULL;
}

int del_list(Linklist *head, char names[30]){
    Linklist *p = NULL;
    p = find_username_exist(head ,names);
    if(head == NULL || p == NULL)
        return -1;
    p -> prev -> next = p -> next;
    p -> next -> prev = p -> prev;
    free(p);
    return 0;
}


int write_in(){
    int fd;
    Linklist *p = NULL;
    if(head == NULL)
        return -1;
    if((fd = open("data.txt", O_CREAT|O_TRUNC|O_RDWR))== -1){
        perror("open");
        exit(1);
    }
    p = head -> next;
    if(lseek(fd, 0, SEEK_SET) == -1){
            perror("lseek");
            exit(1);
        }
    while(p != head){
        if(write(fd, p, sizeof(Linklist)) == -1){
        perror("write");
        exit(1);
        }
        p = p -> next;
    }
    close(fd);

    return 0;
}


int main(){
    Linklist *p;
    head = createlist(head);
    p = createlist(p);
    createlist(head);
    init_head(head);
    strcpy(p -> username, "zhr");
    strcpy(p -> ip, "192.168.20.117");
    insert_end(head, p);
    write_in();
}



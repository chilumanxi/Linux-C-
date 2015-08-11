/*************************************************************************
	> File Name: my_client.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年08月07日 星期五 10时59分47秒
 ************************************************************************/

#include "client.h"
#include <signal.h>

int sock_fd;


void client(int argc, char *argv[]){
    int maxfd;
    char sendbuf[BUFLEN] = {0};
    char recvbuf[BUFLEN] = {0};
    struct sockaddr_in serve_addr;
    fd_set readfds;

    if(argc != 2){
        printf("you need input server's address\n");
        exit(1);
    }

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    memset(&serve_addr, 0, sizeof(struct sockaddr_in));
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVPORT);
    serve_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sock_fd, (struct sockaddr*)&serve_addr, sizeof(serve_addr)) == -1){
        perror("connect");
        exit(1);
    }
    
    FD_ZERO(&readfds);
    while(1){
        FD_SET(0, &readfds);
        FD_SET(sock_fd, &readfds);
        maxfd = sock_fd + 1;

        select(maxfd, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(sock_fd, &readfds)){
            memset(recvbuf, 0, sizeof(recvbuf));
            if(recv(sock_fd, recvbuf, BUFLEN, 0) < 0){
                perror("recv");
                continue;
            }
            if(!strcmp(recvbuf, "EXIT"))
                break;
            printf("%s", recvbuf);
        }

        if(FD_ISSET(0, &readfds)){
            memset(sendbuf, 0, sizeof(sendbuf));
            fgets(sendbuf, BUFLEN, stdin);
            send(sock_fd, sendbuf, strlen(sendbuf), 0);
        }

        FD_CLR(sock_fd, &readfds);
    }

    close(sock_fd);
}

int main(int argc, char **argv){
    client(argc, argv);
    return 0;
}

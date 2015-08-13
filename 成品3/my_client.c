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
    char buffer[BUFLEN] = {0};
    char file_name[1024] = {0};
    int len = 0;
    int times = 0;
    int i;
    char ch;
    struct sockaddr_in serve_addr;
    FILE *fp;
    fd_set readfds;
    sigset_t newmask, oldmask;
    
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

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
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    
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
            else if(!strncmp(recvbuf, "getfile@", 8)){
                printf("Your friend wanna to send you a file,(receive or not('y' or 'n'))\n");
                /*if((ch = getchar()) != 'y'){
                    send(sock_fd, "no", BUFLEN, 0);
                    continue;
                }*/
                recv(sock_fd, &times, sizeof(times), MSG_WAITALL);
                printf("haha\n");
                for(i = 8; recvbuf[i] != '\0' && recvbuf[i] != '\n'; i ++){
                    file_name[i - 8] = recvbuf[i];
                }
                file_name[i - 8] = '\0';
                fp = fopen(file_name, "w");
                if(fp == NULL){
                    printf("open file %s wrong\n", file_name);
                    continue;
                }
                memset(buffer, 0, sizeof(buffer));
                printf("times :%d\n", times);
                while(times --){
                    len = recv(sock_fd, buffer, BUFLEN, 0);
                    fwrite(buffer, sizeof(char), len, fp);
                    memset(buffer, 0, sizeof(buffer));
                }
                printf("fdsfds\n");
                /*
                while((len = recv(sock_fd, buffer, BUFLEN, 0)) > 0){
                    if(fwrite(buffer, sizeof(char), len, fp) < len){
                        printf("file write error!\n");
                        break;
                    }
                    memset(buffer, 0, sizeof(buffer));
                    if(len < BUFLEN)
                        break;
                }
                */
                printf("receive file :%s successfully!\n", file_name);
                fclose(fp);
            }
            else{
                printf("%s", recvbuf);
            }
        }

        if(FD_ISSET(0, &readfds)){
            memset(sendbuf, 0, sizeof(sendbuf));
            fgets(sendbuf, BUFLEN, stdin);
            if(!strncmp(sendbuf, "sendfile@", 9)){
                send(sock_fd, sendbuf, strlen(sendbuf), 0);
                for(i = 9; sendbuf[i] != '@'; i ++)
                    file_name[i - 9] =  sendbuf[i];   
                file_name[i - 9] = '\0';
                fp = fopen(file_name, "r");
                fseek(fp, 0, SEEK_END);
                len = ftell(fp);
                if(len % BUFLEN){
                    times = len / BUFLEN + 1;
                }
                else 
                    times = len / BUFLEN;
                send(sock_fd, &times, sizeof(times), 0);
                fclose(fp);
                printf("%s\n", file_name);
                fp = fopen(file_name, "r");
                if(fp == NULL){
                    printf("file open error!\n");
                    continue;
                }
                else{
                    printf("--------文件发送中-------\n");
                    sleep(1);
                    memset(buffer, 0, sizeof(buffer));
                    while((len = fread(buffer, sizeof(char), BUFLEN, fp)) > 0){
                        if(send(sock_fd, buffer, len, 0) < 0){
                            printf("Send file wrong!\n");
                            break;
                        }
                        memset(buffer, 0, sizeof(buffer));
                    }
                    fclose(fp);
                    printf("File :%s Transfer successfully!\n", file_name);
                }
            }
            else{
                send(sock_fd, sendbuf, strlen(sendbuf), 0);
            }
        }

        FD_CLR(sock_fd, &readfds);
    }

    close(sock_fd);
}

int main(int argc, char **argv){
    client(argc, argv);
    return 0;
}

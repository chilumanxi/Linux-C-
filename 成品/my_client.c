/*************************************************************************
	> File Name: my_client.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年08月07日 星期五 10时59分47秒
 ************************************************************************/

#include "client.h"
#include <signal.h>

int sock_fd;


void client(int argc, char *argv[]){            //客户端的除了根据输入和输出的特殊字符串以外特殊处理以外，其他的都是以select监视标准输入文件和服务器发来的信息
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
    serve_addr.sin_addr.s_addr = inet_addr(argv[1]);                   //写入需要连接的服务器的IP地址

    if(connect(sock_fd, (struct sockaddr*)&serve_addr, sizeof(serve_addr)) == -1){
        perror("connect");
        exit(1);
    }
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);            //屏蔽掉Ctrl C
    
    FD_ZERO(&readfds);
    while(1){
        FD_SET(0, &readfds);
        FD_SET(sock_fd, &readfds);
        maxfd = sock_fd + 1;
        select(maxfd, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(sock_fd, &readfds)){                              //当服务器给客户端发来信息
            memset(recvbuf, 0, sizeof(recvbuf));
            if(recv(sock_fd, recvbuf, BUFLEN, MSG_WAITALL) < 0){    
                perror("recv");
                continue;
            }
            if(!strcmp(recvbuf, "EXIT"))
                break;
            else if(!strncmp(recvbuf, "getfile@", 8)){              //特殊处理接收文件
                recv(sock_fd, &times, sizeof(times), MSG_WAITALL);
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
                usleep(200);
                while(times --){
                    len = recv(sock_fd, buffer, BUFLEN, 0);
                    fwrite(buffer, sizeof(char), len, fp);
                    memset(buffer, 0, sizeof(buffer));
                }
                if(times == 0)
                    printf("receive file :%s successfully!\n", file_name);
                fclose(fp);
            }
            else{
                printf("%s", recvbuf);       //非特殊则直接打印在终端
            }
        }

        if(FD_ISSET(0, &readfds)){                                  //当客户端向终端写入信息
            memset(sendbuf, 0, sizeof(sendbuf));
            fgets(sendbuf, BUFLEN, stdin);
            if(!strncmp(sendbuf, "sendfile@", 9)){                  //特殊处理发送文件
                send(sock_fd, sendbuf, BUFLEN, 0);
                for(i = 9; sendbuf[i] != '@' && sendbuf[i] != '\0' && sendbuf[i] != '\n'; i ++)
                    file_name[i - 9] =  sendbuf[i];   
                file_name[i - 9] = '\0';
                fp = fopen(file_name, "r");
                if(fp == NULL){
                    printf("file open error\n");

                }
                fseek(fp, 0, SEEK_END);
                len = ftell(fp);
                if(len % BUFLEN){
                    times = len / BUFLEN + 1;
                }
                else 
                    times = len / BUFLEN;
                usleep(200);
                printf("%d\n", send(sock_fd, &times, sizeof(times), 0));
                
                usleep(200);
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
                send(sock_fd, sendbuf, strlen(sendbuf), 0);    //非特殊则直接发送至服务器
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

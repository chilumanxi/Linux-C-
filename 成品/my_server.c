/*************************************************************************
	> File Name: my_server.c
	> Author: Zhanghaoran0
	> Mail: chiluamnxi@gmail.com
	> Created Time: 2015年08月07日 星期五 00时24分07秒
    >
    >
    >仍然存在的问题:
    >1.在客户端群聊界面下输入ctrl c强制退出会刷屏
    >2.输入密码或者账户时，ctrl c强制退出会导致服务器也挂掉

    >需要补充的：
    >1.密码加密
    >2.聊天记录加密
    >3.时间
    >4.ctrl c的问题
 ************************************************************************/

#include "server.h"

void load_info(){
    FILE *fp;
    struct info *node;

    if((fp = fopen("client_information", "rb+")) == NULL){
        if((fp = fopen("client_information", "wb+")) == NULL){
            perror("fopen");
            return ;
        }
    }

    while(1){
        node = malloc(sizeof(struct info));
        node -> next = NULL;
        if(fread(node, sizeof(struct info), 1, fp) == 1){
            node -> next = file_info;
            file_info = node;
        }
        else if(feof(fp))
            break;
        else{
            perror("read file error");
            exit(1);
        }
    }

    fclose(fp);
}


void save_info(struct info* const pnode){
    FILE *fp;

    pthread_mutex_lock(&file_mutex);
    if((fp = fopen("client_information", "ab")) == NULL){
        perror("fopen");
        exit(1);
    }
    fwrite(pnode, sizeof(struct info), 1, fp);
    fclose(fp);
    pthread_mutex_unlock(&file_mutex);
}

void update_info(struct info* const pnode){
    FILE *fp;
    struct info * node;
    pthread_mutex_lock(&file_mutex);
    if((fp = fopen("client_information", "w")) == NULL){
        perror("fopen");
        exit(1);
    }
    while(node){
        fwrite(node, sizeof(struct info), 1, fp);
        node = node -> next;
    }
    fclose(fp);
    pthread_mutex_unlock(&file_mutex);
}


void add_in_list(struct info *p, const char *tempname, const char *temppwd, const char *tempage, const char *tempphonenum){
    int i = 0;
    p = malloc(sizeof(struct info));
    p -> next = NULL;
    strcpy(p -> name, tempname);
    strcpy(p -> pwd, temppwd);
    strcpy(p -> age, tempage);
    strcpy(p -> phonenum, tempphonenum);
    for(; i < MAXPEOPLE; i ++){
        p -> friends[i][0] = '\0';
    }
    p -> friendsnum[0] = '0';
    p -> friendsnum[1] = '\0';
    p -> next = file_info;
    file_info = p;
    save_info(p);
}
void init_client(fd_addr client[]){
    int i;
    for(i = 0; i < MAXPEOPLE; i ++){
        client[i].fd = -1;
        memset(client[i].user_name, 0, NAMELEN);
        client[i].room_flag = 0;
    }
}

void server(){
    int sock_fd;
    struct sockaddr_in serve_addr;
    int addr_len = 0;
    int c_num = 0;
    int opt = 1;
    int i;
    int len = sizeof(opt);
    pthread_t thread_id;

    init_client(client);
    memset(&serve_addr, 0, sizeof(struct sockaddr_in));
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_port = htons(SERVPORT);
    serve_addr.sin_addr.s_addr = INADDR_ANY;
    addr_len = sizeof(struct sockaddr_in);
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    if(bind(sock_fd, (struct sockaddr*)&serve_addr, sizeof(serve_addr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(sock_fd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    while(1){
        if((client[c_num].fd = accept(sock_fd, (struct sockaddr*)&client[c_num].addr, &addr_len)) == -1){
            perror("accept");
            continue;
        }

        printf("received a connection from: %s\nfd: %d\nc_num:%d\n", inet_ntoa(client[c_num].addr.sin_addr), client[c_num].fd, c_num);

        pthread_create(&thread_id, NULL, &msg_processor, (void *)&client[c_num]);
        for(i = 0; i < MAXPEOPLE; i ++){
            if(client[i].fd == -1){
                c_num = i;
                break;
            }
        }
    }

    close(sock_fd);
}


void *msg_processor(void *arg){
    fd_set readfds;
    struct info* pnode;
    struct info* knode;
    char buf[BUFLEN] = {0};
    int numbytes = 0;
    int i;
    char tempbuf[BUFLEN];
    

    fd_addr* client_p = (fd_addr*)arg;
    
    if(!login(client_p))
        return ;
    
    write(client_p -> fd, "\n\t************************************************\n", BUFLEN);
    write(client_p -> fd, "             Welcom to chatroom !                   \n", BUFLEN);
    write(client_p -> fd, "      Input \"quit\" to quit the chat!              \n", BUFLEN);
    write(client_p -> fd, "\n", BUFLEN);
    while(1){
        write(client_p -> fd, "             ① chat with all!                       \n", BUFLEN);
        write(client_p -> fd, "             ② chat with your friends!              \n", BUFLEN);
        write(client_p -> fd, "             ③ Your friends                         \n", BUFLEN);
        write(client_p -> fd, "             ④ Get a friends                        \n", BUFLEN);
        write(client_p -> fd, "             ⑤ friend invatation                    \n", BUFLEN);
        write(client_p -> fd, "             ⑥ Exit                                 \n", BUFLEN);
        write(client_p -> fd, "\n", BUFLEN);
        write(client_p -> fd, "             Input your choice                      \n", BUFLEN);

        FD_SET(client_p -> fd, &readfds);
        

        memset(buf, 0, BUFLEN);
        //pthread_mutex_lock(&sock_mutex);
        if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == 1){
            perror("recv in send_all_user");
            close(client_p -> fd);
            return;
        }

        //pthread_mutex_unlock(&sock_mutex);
        buf[-- numbytes] = '\0';
        printf("recv:%s\n", buf);

        if(strlen(buf) != 1){
            write(client_p -> fd, "select error\n", BUFLEN);
            sleep(1);
            continue;
        }

        if(buf[0] == '1'){
            printf("%s add to chatroom\n", inet_ntoa(client_p -> addr.sin_addr));
            send_to_all("-----come in---------", client_p);

            client_p -> room_flag = 1;
            chat_with_all(client_p);
            sleep(1);
        }
        else if(buf[0] == '2'){
            chat_with_one(client_p);
        }
        else if(buf[0] == '3'){
            pnode = file_info;
            while(pnode){
                if(!strcmp(client_p -> user_name, pnode -> name))
                    break;
                pnode = pnode -> next;
            }
            write(client_p -> fd, "these are your friends\n", BUFLEN);

            for(i = 0; pnode -> friends[i][0] != '\0'; i ++){
                write(client_p -> fd, pnode -> friends[i], BUFLEN);
                write(client_p -> fd, "\n", BUFLEN);
            }

            for(i = 0; i < 3; i ++){
                printf("%s\n", pnode -> friends[i]);
            }
            while(1){    
                write(client_p -> fd, "input your friends name to view the information:\n", BUFLEN);
                write(client_p -> fd, "you can input exit to quit\n", BUFLEN);
                memset(buf, 0, sizeof(buf));
                if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                    perror("recv");
                    pthread_exit(NULL);
                }
                buf[--numbytes] = '\0';
                if(!strcmp(buf, "exit"))
                    break;
                for(i = 0; pnode -> friends[i][0] != '\0'; i ++){
                    if(!strcmp(buf, pnode -> friends[i])){
                        knode = file_info;
                        while(knode){
                            if(!strcmp(knode -> name, buf)){
                                printf("knode -> name:%s\n", knode -> name);
                                printf("haha\n");
                                break;
                            }
                            knode = knode -> next;
                        }
                        printf("%s\n", buf);
                        printf("%s\n", knode -> name);
                        strcpy(tempbuf, knode -> name);
                        strcat(tempbuf, "\n");
                        strcat(tempbuf, knode -> age);
                        strcat(tempbuf, "\n");
                        strcat(tempbuf, knode -> phonenum);
                        strcat(tempbuf, "\n\n");
                        write(client_p -> fd, "This friend's information is that:\n", BUFLEN);
                        write(client_p -> fd, tempbuf, BUFLEN);
                        break;
                    }
                }
                if(pnode -> friends[i][0] =='\0')
                    write(client_p -> fd, "You don't have that friend!\n", BUFLEN);
            }
        }
        else if(buf[0] == '4'){
            add_friend(client_p);
        }
        else if(buf[0] == '5'){
            get_invatation(client_p);
        }
        else if(buf[0] == '6'){
            write(client_p -> fd, "EXIT", BUFLEN);
            close(client_p -> fd);
            pthread_mutex_lock(&client_mutex);
            FD_ADDR_CLR(client_p);
            pthread_mutex_unlock(&client_mutex);
            return NULL;
        }

        FD_CLR(client_p ->fd, &readfds);

    }
    close(client_p -> fd);
    pthread_mutex_lock(&client_mutex);
    FD_ADDR_CLR(client_p);
    pthread_mutex_unlock(&client_mutex);
    return NULL;

}

void chat_with_one(fd_addr* const client_p){
    char buf[BUFLEN];
    int numbytes = 0;
    char pathname[PATHLEN];
    char tempbuf[BUFLEN];
    char send_name[NAMELEN];
    struct info* pnode;
    FILE *fp;
    int i, j;
    int t = 0;
    int flag = 0;
    fd_set readfds;
    
    memset(pathname, 0, sizeof(pathname));
    strcpy(pathname, client_p -> user_name);
    strcat(pathname, "/file_message");
    fp = fopen(pathname, "rb");
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0)
        t = 1;
    fseek(fp, 0, SEEK_SET);
    while(t == 0){
        if(fread(tempbuf, BUFLEN, 1, fp) == 1){
            write(client_p -> fd, tempbuf, BUFLEN);
            write(client_p -> fd, "\n", BUFLEN);
            //这里是输出离线的消息但是还没有加换行，再下面离线消息的时候手动加换行符！
        }
        else if(feof(fp)) {
            break;
        }
        else{
            perror("read file error");
            return ;
        }
    }
    fclose(fp);
    fp = fopen(pathname, "w");
    fclose(fp);
    write(client_p -> fd, "Usage : name(the person you wanna to send message)@message\n", BUFLEN);
    FD_ZERO(&readfds);
    while(1){
        FD_SET(client_p -> fd, &readfds);
        select((client_p -> fd) + 1, &readfds, NULL, NULL, NULL);
        //printf("select guo!\n");
        if(FD_ISSET(client_p -> fd, &readfds)){
            //printf("come in!\n");
            memset(buf, 0, sizeof(buf));
            memset(send_name, 0, sizeof(send_name));
            memset(tempbuf, 0, sizeof(tempbuf));
            if((numbytes = recv(client_p -> fd, tempbuf, BUFLEN, 0)) == -1){
                perror("recv");
                return ;
            }
            tempbuf[--numbytes] = '\0';
            if(!strcmp(tempbuf, "exit")){
                return;
            }
            strcpy(buf, client_p -> user_name);
            for(i = 0; tempbuf[i] != '@' && tempbuf[i] != '\0'; i ++){
                send_name[i] = tempbuf[i];
            }
            if(tempbuf[i] == '\0'){
                write(client_p -> fd, "wrong input, try again\n", BUFLEN);
                continue;
            }
            send_name[i] = '\0';
            pnode = file_info;
            while(pnode){
                if(!strcmp(pnode -> name, client_p -> user_name)){
                    for(i = 0; pnode -> friends[i][0] != '\0'; i ++){
                        if(!strcmp(pnode -> friends[i], send_name)){
                            break;
                        }
                    }
                    if(pnode -> friends[i][0] =='\0'){
                        write(client_p -> fd, "This one is not your friend, you should add him as your friend at first!\n", BUFLEN);
                    flag = 1;
                    break;
                    }
                    break;
                }
                pnode = pnode -> next;
            }
            if(flag == 1)
                continue;
            strcat(buf, " tell to you: ");
            i ++;
            int len = strlen(buf);
            if(tempbuf[i] == '\0'){
                write(client_p -> fd, "You can not input empty message!\n", BUFLEN);
                continue;
            }
            for(j = 0; tempbuf[i] != '\0'; i ++, j ++){
                buf[len + j] = tempbuf[i]; 
            }
            buf[i + 14] = '\0';
            printf("%s\n", send_name);
            printf("%s\n", buf);
            for(i = 0; client[i].fd != -1; i ++){
                printf("%s\n", client[i].user_name);
                if(!strcmp(client[i].user_name, send_name)){
                    write(client[i].fd, buf, BUFLEN);
                    write(client[i].fd, "\n", BUFLEN);
                    break;
                }
            }
            if(client[i].fd == -1){
                write(client_p -> fd, "Your friend is not online, you will send a offline message!\n", BUFLEN);
                memset(pathname, 0, sizeof(pathname));
                strcpy(pathname, send_name);
                strcat(pathname, "/file_message");
                fp = fopen(pathname, "a+");
                fwrite(buf, BUFLEN, 1, fp);
                fclose(fp);
            }
        }
        //printf("FD_ISSET guo!\n");
        FD_CLR(client_p -> fd, &readfds);
    }

}

int login(fd_addr* const client_p){
    char buf[BUFLEN];
    int numbytes = 0;
    char pathname[PATHLEN];
    struct info* pnode;
    char tempname[NAMELEN];
    char temppwd[PWDLEN];
    char tempage[3];
    char tempphonenum[PWDLEN];
    int flag0;
    FILE *fp;
    int i;

    pnode = file_info;

    while(1){
        load_info();
        write(client_p -> fd, "\n\t********************************************\n", BUFLEN);
        write(client_p -> fd, "                 ① Login                        \n", BUFLEN);
        write(client_p -> fd, "                 ② Regist                       \n", BUFLEN);
        write(client_p -> fd, "                 ③ Exit                         \n", BUFLEN);
        write(client_p -> fd, "\n\n", BUFLEN);
        write(client_p -> fd,"                   Input your choice:            \n", BUFLEN);

        memset(buf, 0, sizeof(buf));
        if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
            perror("recv in login");
            pthread_exit(NULL);
        }
        buf[-- numbytes] = '\0';

        printf("from:%s : %s\n", inet_ntoa(client_p -> addr.sin_addr), buf);
        if(strlen(buf) != 1){
            write(client_p -> fd, "\njust choose a number!\n", BUFLEN);
            continue;
        }

        if(buf[0] == '1'){
            write(client_p -> fd, "\ninput your user name:\n", BUFLEN);
            memset(buf, 0, BUFLEN);
            if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                perror("recv in login");
                pthread_exit(NULL);
            }
            buf[-- numbytes] = '\0';
            strcpy(tempname, buf);

            pnode = file_info;
            
            while(pnode){
                if(!strcmp(buf, pnode -> name)){
                    strncpy(tempname, buf, NAMELEN - 1);
                    break;
                }
                pnode = pnode -> next;
            }

            if(!pnode){
                write(client_p -> fd, "the username dose not exist!\n", BUFLEN);
                sleep(1);
                continue;
            }

            write(client_p -> fd, "\ninput your password:\n", BUFLEN);
            memset(buf, 0, sizeof(buf));
            if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                perror("recv in login");
                pthread_exit(NULL);
            }
            buf[--numbytes] = '\0';

            printf("from:%s :%s\n", inet_ntoa(client_p -> addr.sin_addr), buf);
            printf("node -> name:%s\n", pnode -> name);
            //printf("client_p -> user_name:%s///strlen:%d\n", client_p -> user_name, strlen(client_p -> user_name));

            if(!strcmp(buf, pnode -> pwd)){
                for(i = 0; i < MAXPEOPLE; i ++){
                    if((client[i].fd != -1) && !strcmp(tempname, client[i].user_name)){
                        break;
                    }
                }

                if(i != MAXPEOPLE){
                    write(client_p -> fd, "this user name have already login!\n", BUFLEN);
                    sleep(1);
                    continue;
                }
                else{
                    strcpy(client_p -> user_name, tempname);
                    printf("%s login success\n", client_p -> user_name);

                    return 1;
                }
            }
            else{
                write(client_p -> fd, "password not match!!\n", BUFLEN);
                sleep(1);
            }
        }
        else if(buf[0] == '2'){
            write(client_p -> fd, "input your use name:\n", BUFLEN);
            memset(buf, 0, sizeof(buf));
            if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                perror("recv in regist");
                pthread_exit(NULL);
            }
            buf[-- numbytes] = '\0';
            strcpy(tempname, buf);
            pnode = file_info;

            while(pnode){
                if(!strcmp(buf, pnode -> name)){
                    write(client_p -> fd, " this user has already exist\n", BUFLEN);
                    sleep(1);
                    break;
                }   
                pnode = pnode -> next;
            }

            if(!pnode){
                write(client_p -> fd, "input the password:\n", BUFLEN);
                memset(buf, 0, sizeof(buf));
                if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                    perror("recv in regist");
                    pthread_exit(NULL);
                }
                buf[--numbytes] = '\0';
                strcpy(temppwd, buf);

                write(client_p -> fd, "input the password again:\n", BUFLEN);
                memset(buf, 0, sizeof(buf));
                if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                    perror("recv in regist");
                    pthread_exit(NULL);
                }

                buf[--numbytes] = '\0';

                if(!strcmp(temppwd, buf)){
                    write(client_p -> fd, "regist success!\n", BUFLEN);
                    flag0 = mkdir(tempname, 0755);
                    strcpy(pathname, tempname);
                    strcat(pathname, "/file_invatation");
                    fp = fopen(pathname, "wb");
                    memset(pathname, 0, sizeof(pathname));
                    strcpy(pathname, tempname);
                    strcat(pathname, "/file_message");
                    fp = fopen(pathname, "wb");
                    sleep(1);

                  //printf("regist %s : %s\n", pnode -> name, pnode -> pwd);
                    write(client_p -> fd, "please input your age:\n", BUFLEN);
                    memset(buf, 0, sizeof(buf));
                    while(1){
                        if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) > 3){
                            write(client_p -> fd, "age doesn't be in the limition! Input again\n", BUFLEN);
                            continue;
                        }
                        else break;
                    }
                    buf[-- numbytes] = '\0';
                    strcpy(tempage, buf);

                    write(client_p -> fd, "input your phone number:\n", BUFLEN);
                    memset(buf, 0, sizeof(buf));

                    if((numbytes = recv(client_p -> fd, buf, BUFLEN,0)) == -1){
                        perror("recv");
                        pthread_exit(NULL);
                    }

                    buf[-- numbytes] = '\0';
                    strcpy(tempphonenum, buf);
                    
                    add_in_list(pnode, tempname, temppwd, tempage, tempphonenum);


                }
                else{
                    write(client_p -> fd, "password not match!\n", BUFLEN);
                    sleep(1);
                }
            }
                
        }

        else if(buf[0] == '3'){
            write(client_p -> fd, "EXIT", BUFLEN);
            close(client_p -> fd);
            pthread_mutex_lock(&client_mutex);
            FD_ADDR_CLR(client_p);
            pthread_mutex_unlock(&client_mutex);
            pthread_exit(NULL);
        }   
    }
}


void send_to_all(char *buf, fd_addr* const client_p){
    int i = 0;
    char add_buf[BUFLEN] = {0};

    strcpy(add_buf, "<<");
    strcat(add_buf, client_p -> user_name);
    strcat(add_buf, ">>\n");
    strcat(add_buf, buf);
    strcat(add_buf, "\n\n");

    for(i = 0; i < MAXPEOPLE; i ++){
        if((client[i].fd != -1) && (client[i].fd != client_p -> fd) && (client[i].room_flag == 1)){

            printf("send to %s\n", inet_ntoa(client[i].addr.sin_addr));

            if(send(client[i].fd, add_buf, strlen(add_buf), 0) == -1){
                perror("sendto error");
                perror((char*)inet_ntoa(client[i].addr.sin_addr));
                continue;
            }
        }
    }

}

void chat_with_all(fd_addr * const client_p){
    fd_set readfds;
    char buf[BUFLEN] = {0};
    int numbytes = 0;
    int i = 0;
    
    write(client_p -> fd, "\n                 Welcom to chatroom\n\n", BUFLEN);
    FD_ZERO(&readfds);

    while(1){
        FD_SET(client_p -> fd, &readfds);
        select((client_p -> fd) + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(client_p -> fd, &readfds)){
            memset(buf, 0, BUFLEN);
            if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
                perror("recv in send_to_all");
                return ;
            }
            buf[--numbytes] = '\0';

            if(!strcmp(buf, "quit")){
                send_to_all("-----go out-------------", client_p);
                printf("%s Quit\n", inet_ntoa(client_p -> addr.sin_addr));
                client_p -> room_flag = 0;
                return;
            }
            send_to_all(buf, client_p);
        }
        FD_CLR(client_p -> fd, &readfds);
    }
}

void add_friend(fd_addr * const client_p){
    struct info *pnode;
    pnode = file_info;
    char buf[BUFLEN];
    char tempbuf[BUFLEN];
    int numbytes = 0;
    int i = 0;
    write(client_p -> fd, "input the name you wanna to add:\n", BUFLEN);
    if((numbytes = recv(client_p -> fd, buf, BUFLEN, 0)) == -1){
        perror("recv");
        pthread_exit(NULL);
    }

    buf[--numbytes] = '\0';
    if(!strcmp(buf, client_p -> user_name)){
        write(client_p -> fd, "You can't add yourself as a friend!\n", BUFLEN);
        sleep(1);
        return ;
    }
    while(pnode){
        if(!strcmp(pnode -> name, buf))
            break;
        pnode = pnode -> next;
    }
    if(!pnode)
        write(client_p -> fd, "no that person! input again\n", BUFLEN);
    else{
        for(; pnode -> friends[i][0] != '\0'; i ++){
            if(!strcmp(pnode -> friends[i], client_p -> user_name)){
                write(client_p -> fd, "You have already have this friends!\n", BUFLEN);
                sleep(1);
                return ;
            }
        }
        strcpy(tempbuf, "Are you sure to add");
        strcat(tempbuf, buf);
        strcat(tempbuf, "?('y' or 'n')\n");
        if(send(client_p -> fd, tempbuf, strlen(tempbuf), 0) == -1){
            perror("send");
            return ;
        }
        while(1){
            if((numbytes = recv(client_p -> fd, tempbuf, BUFLEN, 0)) == -1){
                pthread_exit(NULL);
            }
            tempbuf[--numbytes] = '\0';
            if(tempbuf[0] == 'y' || tempbuf[0] == 'Y'){
                send_invatation(client_p, buf);
                break;
            }
            else if(tempbuf[0] == 'n' || tempbuf[0] == 'N'){
                break;
            }
            else {   
                write(client_p -> fd, "wrong input, input again!\n", BUFLEN);
                continue;
            }

        }
    }

}

void send_invatation(fd_addr *const client_p, const char *accname){
    FILE *fp;
    char pathname[PATHLEN];
    strcpy(pathname, accname);
    strcat(pathname, "/file_invatation");
    printf("%s\n", pathname);
    fp = fopen(pathname, "a+");
    if(fp == NULL){
        perror("fopen");
        return ;
    }
    
    printf("%d\n",fwrite(client_p -> user_name, NAMELEN, 1, fp));
    printf("%s\n", client_p -> user_name);
    printf("send ok!\n");
    fclose(fp);
}

int get_invatation(fd_addr *const client_p){
    FILE *fp;
    char pathname[PATHLEN];
    char tempname[MAXPEOPLE][NAMELEN];
    char buf[BUFLEN];
    int i = 0, j, t = 0;
    struct info* pnode;
    int numbytes = 0;
    strcpy(pathname, client_p -> user_name);
    strcat(pathname, "/file_invatation");
    fp = fopen(pathname, "rb+");
    fseek(fp , 0, SEEK_END);
    if(ftell(fp) == 0){
        write(client_p -> fd, "You have no invatation!\n", BUFLEN);
        return 0;
    }
    fclose(fp);
    fp = fopen(pathname, "rb+");
    pnode = file_info;
    while(pnode){
        if(!strcmp(pnode -> name, client_p -> user_name)){
            printf("haha\n");    
            break;
        }
        pnode = pnode -> next;
    }
    printf("11\n");
    i  = 0;
    while(1){
        if(fread(tempname[i], NAMELEN, 1, fp) == 1){
           i ++; 
        }
        else if(feof(fp)){
            break;
        }
        else{
            perror("fread");
            return -1;
        }

    }
    for(j = 0; j < i; j ++){
        printf("%d\n", i);
        memset(buf, 0, sizeof(buf));
        strcpy(buf, tempname[j]);
        strcat(buf, " wants you to be his friend, do you agree?('y' or 'n')\n");
        if(send(client_p -> fd, buf, strlen(buf), 0) == -1){
            perror("send");
            return -1;
        }
        memset(buf, 0, sizeof(buf));
        numbytes = recv(client_p -> fd, buf, BUFLEN, 0);
        buf[--numbytes] = '\0';
        if(!strcmp("y", buf)){
            while(pnode -> friends[t][0] != '\0'){
                t ++;
            }
            printf("%s\n", pnode -> name);
            strcpy(pnode -> friends[t], tempname[j]);
            pnode = file_info;
            while(pnode){
                if(!strcmp(pnode -> name, tempname[j]))
                    break;
                pnode = pnode -> next;
            }
            t = 0;
            printf("%s\n", pnode -> name);
            while(pnode -> friends[t][0] != '\0')
                t ++;
            strcpy(pnode -> friends[t], client_p -> user_name);
        }
        else if(strcmp("n", buf)){
            continue;
        }
        else{
            printf("3\n");
            write(client_p -> fd, "wrong input\n", BUFLEN);
            j --;
        }
    }
    update_info(file_info);
    fclose(fp);
    fp = fopen(pathname, "w");
    fclose(fp);
    return 1;
}

/*
void send_file(fd_addr * client_p, , char *pathname){
    char buf[BUFLEN];
    int numbytes = 0;
    FILE *fp;
    int i;

    for(i = 0; client[i].fd != -1; i ++){
        if(!strcmp(client[i].user_name , name)){
            fp = fopen(pathname, "r");
            if(fp == NULL){
                printf("File :%s not found\n", file_name);
            }
            else{
                memset(buf, 0, sizeof(buf));
                while((numbytes = fread(buf, sizeof(char), BUFLEN, fp)) > 0){
                    if(send)
                }
            }
        }
    }
    
    
}
*/
int main(){
    load_info();
    server();
    return 0;
}

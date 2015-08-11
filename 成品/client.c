#include<signal.h>
#include"client.h"

int ctr_c_flag = 0;
int sock_fd = -1;
void sigact()
{
	//ctr_c_flag = 1;
	//send(sock_fd, "EXIT\n", 6, 0);
	//close(sock_fd);
	//exit(0);
}

void client(int argc, char** argv)
{
	int maxfd;
	char sendbuf[BUFLEN] = {0};
	char recvbuf[BUFLEN] = {0};
	struct sockaddr_in serv_addr;
	fd_set rfds;

	if(argc != 2)
	{
		printf("parameter should have server's address\n");
		exit(1);
	}

	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVPORT);
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&(serv_addr.sin_zero), 8);

	if(connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		perror("connect");
		exit(1);
	}

	signal(SIGINT, sigact);

	FD_ZERO(&rfds);
	while(1)
	{
		FD_SET(0, &rfds);
		FD_SET(sock_fd, &rfds);
		maxfd = MAX(0, sock_fd) + 1;

		select(maxfd, &rfds, NULL, NULL, NULL);
		if(FD_ISSET(sock_fd, &rfds))
		{
			bzero(recvbuf, BUFLEN);
			recv(sock_fd, recvbuf, BUFLEN, 0);
			if(!strcmp(recvbuf, "EXIT"))
				break;
			printf("%s", recvbuf);
			fflush(stdout);
		}

		if(FD_ISSET(0, &rfds))
		{
			bzero(sendbuf, BUFLEN);
			fgets(sendbuf, BUFLEN, stdin);
			send(sock_fd, sendbuf, strlen(sendbuf), 0);
		}
		
		//if(ctr_c_flag)
		//{
		//	close(sock_fd);
		//	exit(0);
		//}

		FD_CLR(sock_fd, &rfds);
	}

	close(sock_fd);
}

int main(int argc, char** argv)
{
	client(argc, argv);
	return 0;
}

//client
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define N 32
#define ip "192.168.1.106"   
#define port 8888

#define R 1 //user - register
#define L 2 //user - login
#define Q 3 //user - query
#define H 4 //user - history

//定义通信双方的信息结构体
typedef struct 
{
    int type;
    char name[N];
    char data[256];
}MSG;


//注册
int do_register(int sockfd, MSG* msg)
{
    msg->type = R;

    printf("Input name:");
    scanf("%s", msg->name);
    getchar();

	printf("Input passwd:");
	scanf("%s", msg->data);

	if(send(sockfd, msg, sizeof(MSG),0) < 0)
	{
		printf("fail to send.\n");
		return -1;
	}

	if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		printf("Fail to recv.\n");
		return -1;
	}

	printf("%s\n", msg->data);

	return 0;

}

//登录
int do_login(int sockfd, MSG* msg)
{
	msg->type = L;

	printf("Input name:");
	scanf("%s", msg->name);
	getchar();

	printf("Input passwd:");
	scanf("%s", msg->data);

	if(send(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		printf("fail to send.\n");
		return -1;
	}

	if(recv(sockfd, msg, sizeof(MSG), 0) < 0)
	{
		printf("Fail to recv.\n");
		return -1;
	}

	if(strncmp(msg->data, "OK", 3) == 0)
	{
		printf("Login ok!\n");
		return 1;
	}
	else
	{
		printf("%s\n", msg->data);

	}

	return 0;
}

//查询
int do_query(int sockfd, MSG* msg)
{
	msg->type = Q;
	puts("-----------------");

	while(1)
	{
		printf("Input word:");
		scanf("%s", msg->data);
		getchar();

		//客户端，输入#号，返回上一级菜单
		if(strncmp(msg->data, "#", 1) == 0)
		{
			printf("Fail to send.\n");
			return -1;
		}

		//将要查询的单词发送给服务器
		if(send(sockfd, msg, sizeof(MSG), 0) < 0)
		{
			printf("Fail to send.\n");
			return -1;
		}

		printf("%s\n", msg->data);
	}

	return 0;

}

//历史记录
int do_history(int sockfd, MSG* msg)
{
	msg->type = H;

	send(sockfd, msg, sizeof(MSG), 0);

	//接受服务器，传递回来的历史记录信息
	while(1)
	{
		recv(sockfd, msg, sizeof(MSG), 0);

		if(msg->data[0] == '\0')
			break;
		
		//输出历史记录信息
		printf("%s\n", msg->data);

	}

	return 0;

}

int main(int argc, const char* argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	int n;
	MSG msg;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sockfd)
	{
		perror("socket error\n");
		return -1;
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	serveraddr.sin_port = htons(port);

	//连接服务端
	if (0 > connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)))
	{
		perror("connect error\n");
		return -1;
	}

	while (1)
	{
		printf("*****************************************************************\n");
		printf("* 1.register          2.login              3.quit               *\n");
		printf("*****************************************************************\n");
		printf("Please choose:");

		scanf("%d", &n); //读取\n之前的数据
		getchar(); //清除\n，因为输入回车时有\n

		switch (n)
		{
		case 1:  //注册
			do_register(sockfd, &msg);
			break;

		case 2:  //登录
			if (do_login(sockfd, &msg) == 1)
			{
				goto next;
			}
			break;

		case 3:  //退出
			close(sockfd);
			exit(0);
			break;

		default: //错误指令
			printf("Invalid data cmd.\n");

		}

	}

next:
	while (1)
	{
		printf("*****************************************************\n");
		printf("* 1.query_word   2.history_record   3.quit          *\n");
		printf("*****************************************************\n");
		printf("Please choose:");
		scanf("%d", &n);
		getchar();

		switch (n)
		{
		case 1:  //查询
			do_query(sockfd, &msg);
			break;

		case 2:  //历史记录
			do_history(sockfd, &msg);
			break;

		case 3: //退出
			close(sockfd);
			exit(0);
			break;

		default: //错误指令
			printf("Invalid data cmd.\n");
		}
	}

	return 0;
}
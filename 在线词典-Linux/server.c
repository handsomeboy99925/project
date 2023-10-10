//server
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>

#define  R  1  //注册
#define  L  2  //登录
#define  Q  3  //退出
#define  H  4  //历史记录

#define  DATABASE  "my.db"

#define ip "192.168.1.106"   
#define port 8888

#define N 32
//定义通信双方的信息结构体
typedef struct
{
	int type;
	char name[N];
	char data[256];
}MSG;

//处理客户端信息
int do_client(int acceptfd, sqlite3* db)
{
	MSG msg;
	//读取网络数据
	while (0 < recv(acceptfd, &msg, sizeof(msg), 0))
	{
		printf("type:%d\n", msg.type);
		//打印数据类型
		switch (msg.type)
		{
		case R:
			do_register(acceptfd, &msg, db);
			break;

		case L:
			do_login(acceptfd, &msg, db);
			break;

		case Q:
			do_query(acceptfd, &msg, db);
			break;

		case H:
			do_history(acceptfd, &msg, db);
			break;

		default:
			printf("Invalid data msg.\n");
				
		}
	}

	printf("client exit.\n");
	close(acceptfd);
	exit(0);

	return 0;
}

//注册
int do_register(int acceptfd, MSG* msg, sqlite3* db)
{
	char* errmsg;
	char sql[128];

	sprintf(sql, "insert into usr values('%s', %s);", msg->name, msg->data);
	printf("%s\n", sql);

	//用于执行sql语句
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		strcpy(msg->data, "usr name already exist.");
	}
	else
	{
		printf("client register ok!\n");
		strcpy(msg->data, "ok");
	}

	//用于发送数据
	if (send(acceptfd, msg, sizeof(MSG), 0) < 0)
	{
		perror("send error");
		return -1;
	}

	return 0;
}

//登录
int do_login(int acceptfd, MSG* msg, sqlite3* db)
{
	char sql[128] = {};
	char* errmsg;
	int nrow;   //行
	int ncloumn;   //列
	char **resultp;

	sprintf(sql, "select * from usr where name = '%s' and pass = '%s';", msg->name, msg->data);
	printf("%s\n", sql);

	//获取company表里的信息
	if (sqlite3_get_table(db, sql, &resultp, &nrow, &ncloumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
		return -1;
	}
	else
	{
		printf("get_table ok\n");
	}

	//查询成功，数据库有此用户
	if (nrow == 1)
	{
		strcpy(msg->data, "OK");
		send(acceptfd, msg, sizeof(MSG), 0);
		return 1;
	}

	//密码或用户名错误
	if (nrow == 0)
	{
		strcpy(msg->data, "usr/passwd wrong.");
		send(acceptfd, msg, sizeof(MSG), 0);
	}

	return 0;

}

//查询
int do_query(int acceptfd, MSG* msg, sqlite3* db)
{
	char word[64];
	int found = 0;
	char date[128] = {};
	char sql[128] = {};
	char* errmsg;

	//拿出msg结构体中，要查询的单词
	strcpy(word, msg->data);
	//查询单词
	found = do_searchword(acceptfd, msg, word);
	printf("查询一个单词完毕.\n");

	//表示找到了单词，将用户名，时间，单词，插入到历史记录表中
	if (found == 1)
	{
		//获取系统时间
		get_date(date);

		sprintf(sql, "insert into record values('%s', '%s', '%s')", msg->name, date, word);

		if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("%s\n", errmsg);
			return -1;
		}
		else
		{
			printf("Insert record done.\n");
		}
	}
	else  //没找到
	{
		strcpy(msg->data, "Not found!");
	}

	//将查询的结果，发送给客户端
	send(acceptfd, msg, sizeof(MSG), 0);

	return 0;

}


//查询单词
int do_searchword(int acceptfd, MSG* msg, char word[])
{
	FILE* fp;
	int len = 0;
	char temp[512] = {};
	int result;
	char* p;

	//打开文件，读取文件，进行对比
	if ((fp = fopen("dict.txt", "r")) == NULL)
	{
		perror("fopen error.\n");
		strcpy(msg->data, "Failed to open dict.txt");
		send(acceptfd, msg, sizeof(MSG), 0);
		return -1;
	}

	//打印出客户端要查询的单词
	len = strlen(word);
	printf("%s, len = %d\n", word, len);

	//读文件，查询单词
    while (fgets(temp, 512, fp) != NULL) 
    //获取一行的内容，长度包含了\0,作为字符串结束符，输出一个空格
    {
        //查找单词
        result = strncmp(temp, word, len);
        /* temp = word 返回0；temp > word 返回大于0的值
        temp < word 返回小于0的值 */

        //查找失败情况一
        if (0 > result)  //没找到要查询的单词
        {
            continue;  //跳出当前循环，执行下一次循环，查找下一行
        }

        //查找失败情况二，注释tem[len] != ' ';表示获取的长度里不包含\0 
        if (0 < result || ((result == 0) && (temp[len] != ' ')))
        {
            break;  //打断循环，执行下一条语句
        }

        p = temp + len;  //找到要查的单词，把单词长度放进数组里
        //移动到注释的位置
        while(*p == ' ')
        {
            p++;
        }

        //找到注释，跳跃过所有空格
        strcpy(msg->data, p);
        printf("found word:%s\n", msg->data);

        fclose(fp);
        return 1;
    }

    fclose(fp);

    return 0;

}


//得到查询结果，并且将历史记录发送给客户端
int history_callback(void* arg, int f_num, char** f_value, char** f_name)
{
    int acceptfd;
    MSG msg;

    acceptfd = *((int *)arg);

    sprintf(msg.data, "%s, %s", f_value[1], f_value[2]);

    send(acceptfd, &msg, sizeof(MSG), 0);

    return 0;
}


//历史记录
int do_history(int acceptfd, MSG* msg, sqlite3* db)
{
    char sql[128] = {};
    char *errmsg;

    sprintf(sql, "select * from record where name = '%s'", msg->name);

    //查询数据库
    if(sqlite3_exec(db, sql, history_callback, (void *)&acceptfd, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("Query record done.\n");
    }

    //所有的记录查询发送完毕之后，给客户端发出一个结束信息
    msg->data[0] = '\0';

    send(acceptfd, msg, sizeof(MSG), 0);

    return 0;

}


//获取时间
int get_date(char* date)
{
    time_t t;
    struct tm *tp;

    time(&t); //系统调用time（）用于获取当前时间，秒为单位

    //进行时间格式转换
    //把time（）得到的秒数变成一个结构体所表示的时间，对应本地时间
    tp = localtime(&t);

    sprintf(date, "%d-%d-%d %d:%d:%d", tp->tm_year + 1900, tp->tm_mon+1, tp->tm_mday,
            tp->tm_hour, tp->tm_min, tp->tm_sec);
    printf("get date:%s\n", date);

    return 0;
}

int main(int argc, const char* argv[])
{
	int sockfd;
	int acceptfd;
	struct sockaddr_in serveraddr;
	pid_t pid;
	sqlite3 *db;
	MSG* msg;

	//打开数据库
	//打开并创建一个DATABASE的数据库，&db指向sqlite3 *db的地址
	if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
		return -1;
	}
	else
	{
		printf("open DATABASE success.\n");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	//AF_INET 是IPv4协议，SOCK_STREAM用于TCP协议，0代表默认协议
	if (0 > sockfd)
	{
		perror("socket error\n");
		return -1;
	}
	
	bzero(&serveraddr, sizeof(serveraddr));//结构体清零
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port); //端口号

	//将套接字与IP地址、端口号进行绑定
	if (0 > bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)))
	{
		perror("bind error\n");
		close(sockfd);
		return -1;
	}

	//设为监听模式,5为等待连接队列能够达到的最大值
	if (0 > listen(sockfd, 5))
	{
		perror("listen error\n");
		close(sockfd);
		return -1;
	}

	//处理僵尸进程
	//僵尸进程是由于客户端退出连接后，服务器并不知道，还维持一个僵尸进程

	signal(SIGCHLD, SIG_IGN);
	//这个是交给系统init去回收僵尸进程，SIG_IGN表示忽略信号

	while (1)
	{
		//获取客户端的连接请求并建立连接
		acceptfd = accept(sockfd, NULL, NULL);
		//后面两个NULL表示不获取客户端的IP地址和端口号
		if (0 >(acceptfd))
		{
			perror("accept1 error\n");
			close(acceptfd);
			return -1;
		}

		//创建子进程
		if (0 > (pid = fork()))
		{
			perror("fork error");
			close(acceptfd);
			do_client(acceptfd, db);
		}
		else if(pid == 0)  //子进程创建成功，返回0
		{
			close(sockfd);   //关闭服务端套接字
			do_client(acceptfd, db);  //调用子函数，与客户端通信
		}
		else    // 父亲进程,用来接受客户端的请求的
		{
			close(acceptfd);  
			//关闭服务端与客户端连接的套接字，再次执行循环体，重新建立acceptfd套接字，以免冲突
		}

	}

	return 0;
}
#include "video.h"


static char movie_buf[1024 * 600] = {0}; // 播放视频命令的存储数组
static int fifo_fd;                      // 管道文件的文件描述符
static int movie_flag = 0;               // 视频标志位


/*********************************************************************************************/
/**************************************视频播放************************************************/
/********************************************************************************************/
// 1、创建并打开管道文件
int open_movie_pipe(void)
{
    // a、检查是否有管道文件，没有即创建
    if (access("/tmp/fifo", F_OK))
    {
        int ret = mkfifo("/tmp/fifo", 0777);
        {
            perror("mkfifo error!");
            return -1;
        }
    }
    // b、打开管道文件
    int fifo_fd = open("/tmp/fifo", O_RDWR);
    if (fifo_fd == -1)
    {
        perror("open fifo error!");
        return -2;
    }
}

// 7、关闭管道文件
int close_movie_pipe(void)
{
    close(fifo_fd);
    return 0;
}

// 2、打开视频(指定哪个视频)
int start_movie(char *moviepath, int lcd_x, int lcd_y, int length, int width)
{
    if (movie_flag == 0)
    {
        open_movie_pipe();                   // 1、打开管道文件
        usleep(500000);                      // 延时0.5秒
        bzero(movie_buf, sizeof(movie_buf)); // 清空字符串
        sprintf(movie_buf, "mplayer -slave -quiet -input file=/tmp/fifo -geometry %d:%d -zoom -x %d -y %d %s ",
                lcd_x, lcd_y, length, width, moviepath); // 拼接字符串
        system(movie_buf);                 // 执行字符串里的shell命令
        movie_flag = 1;
    }
    return 0;
}

// 3、发送控制命令到管道中(可以指定哪个命令)
int send_cmd_pipe(int cmd)
{

    open_movie_pipe();                   // 1、打开管道文件
    usleep(500000);                      // 延时0.5秒
    bzero(movie_buf, sizeof(movie_buf)); // 清空字符串
    if (cmd == 1)                        // 1、视频退出
    {
        // strcpy复制字符串函数

        strcpy(movie_buf, "quit 0\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
        usleep(500000); // 延时0.5秒
        movie_flag = 0;
        close_movie_pipe();
    }
    if (cmd == 2) // 2、视频暂停/继续
    {
        // strcpy复制字符串函数
        strcpy(movie_buf, "pause\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
    }
    if (cmd == 3) // 3、增加音量
    {
        // strcpy复制字符串函数
        strcpy(movie_buf, "volume 10\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
    }
    if (cmd == 4) // 4、减少音量
    {
        // strcpy复制字符串函数
        strcpy(movie_buf, "volume -10\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
    }
    if (cmd == 5) // 5、快进5秒
    {
        // strcpy复制字符串函数
        strcpy(movie_buf, "seek -5\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
    }
    if (cmd == 6) // 6、快退5秒
    {
        // strcpy复制字符串函数
        strcpy(movie_buf, "seek 5\n");
        write(fifo_fd, movie_buf, strlen(movie_buf));
        // strlen算出字符串的长度
    }
    return 0;
}
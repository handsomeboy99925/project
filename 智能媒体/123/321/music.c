#include "music.h"


static char music_buf[128] = {0};

/*********************************************************************************************/
/**************************************音乐播放************************************************/
/********************************************************************************************/
//1、开始音乐(能够指定哪首)
int start_music(char *musicpath)
{
	bzero(music_buf, sizeof(music_buf));				//清空字符串
	sprintf(music_buf, "mplayer %s &", musicpath);		//拼接字符串
	system(music_buf);									//执行字符串里的shell命令
	return 0;
}
//2、结束音乐
int exit_music(void)
{
	system("killall -9 mplayer &");							
	return 0;
}
//3、暂停音乐
int stop_music(void)
{
	system("killall -STOP mplayer &");							
	return 0;
}

//4、继续音乐
int cont_music(void)
{
	system("killall -CONT mplayer &");							
	return 0;
}
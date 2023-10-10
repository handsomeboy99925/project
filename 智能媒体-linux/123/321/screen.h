#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/input.h>

int open_lcd_screen(void);
int close_lcd_screen(void);

extern int fds; // lcd屏文件描述符

#endif
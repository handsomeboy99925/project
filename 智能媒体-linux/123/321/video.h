#ifndef _VIDEO_H_
#define _VIDEO_H_

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

int open_movie_pipe(void);
int close_movie_pipe(void);
int start_movie(char *moviepath, int lcd_x, int lcd_y, int length, int width);
int send_cmd_pipe(int cmd);

#endif
#ifndef _TS_H_
#define _TS_H_

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

int open_ts(void);
int get_ts_xy(int *ts_x, int *ts_y);
int close_ts(void);



#endif
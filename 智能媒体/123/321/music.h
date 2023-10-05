#ifndef _MUSIC_H_
#define _MUSIC_H_

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

int start_music(char *musicpath);
int exit_music(void);
int stop_music(void);
int cont_music(void);

#endif
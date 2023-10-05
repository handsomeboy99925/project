#include "screen.h"

int screen_size;
unsigned long line_length;
int width;
int height;
unsigned short *screen_base = NULL;

 int fds; // lcd屏文件描述符

/*********************************************************************************************/
/**************************************lcd屏控制**********************************************/
/********************************************************************************************/

// 打开lcd屏
int open_lcd_screen(void)
{
    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;

    if (0 > (fds = open("/dev/fb0", O_RDWR)))
    {
        perror("open error1");
        exit(-1);
    }

    ioctl(fds, FBIOGET_VSCREENINFO, &fb_var);
    ioctl(fds, FBIOGET_FSCREENINFO, &fb_fix);

    screen_size = fb_fix.line_length * fb_var.yres;
    line_length = fb_fix.line_length;
    width = fb_var.xres;
    height = fb_var.yres;

    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fds, 0);
    if (MAP_FAILED == (void *)screen_base)
    {
        perror("mmap error1");
        close(fds);
        exit(-1);
    }

    memset(screen_base, 0xFF, screen_size);

    return 0;
}

// 关闭lcd屏
int close_lcd_screen(void)
{
    munmap(screen_base, screen_size);
    close(fds);
    return 0;
}

#include "ts.h"

static int fdt; // 触摸屏文件描述符

/*********************************************************************************************/
/**************************************触摸屏控制**********************************************/
/********************************************************************************************/

// 打开触摸屏
int open_ts(void)
{

    fdt = open("/dev/input/event1", O_RDONLY);
    if (0 > fdt)
    {
        perror("ts open error1");
        exit(-1);
    }

    return 0;
}

// 获取触摸屏坐标
int get_ts_xy(int *ts_x, int *ts_y)
{
    struct input_event in_ev;

    while (1)
    {
        if (sizeof(struct input_event) != read(fdt, &in_ev, sizeof(struct input_event)))
        {
            perror("ts read error2");
            exit(-1);
        }

        if (in_ev.type == EV_ABS && in_ev.code == ABS_X)
        {
            *ts_x = in_ev.value;
            printf("x = %d\n", in_ev.value);
        }

        if (in_ev.type == EV_ABS && in_ev.code == ABS_Y)
        {
            *ts_y = in_ev.value;
            printf("y = %d\n", in_ev.value);
        }

        if (in_ev.type == EV_KEY && in_ev.code == BTN_TOUCH)
        {
            printf("touch = %d\n", in_ev.value);

            if (in_ev.value == 0)
            {
                break;
            }
        }
    }
}

// 关闭触摸屏
int close_ts(void)
{
    close(fdt);
    return 0;
}
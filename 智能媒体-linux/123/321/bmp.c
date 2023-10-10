#include "bmp.h"
#include "screen.h"

/**** 位图信息头数据结构 ****/
typedef struct
{
    unsigned int size;        // 位图信息头大小
    int width;                // 图像宽度
    int height;               // 图像高度
    unsigned short planes;    // 位面数
    unsigned short bpp;       // 像素深度
    unsigned int compression; // 压缩方式
    unsigned int image_size;  // 图像大小
    int x_pels_per_meter;     // 像素/米
    int y_pels_per_meter;     // 像素/米
    unsigned int clr_used;
    unsigned int clr_omportant;
} __attribute__ ((packed)) bmp_info_header;

/**** BMP文件头数据结构 ****/
typedef struct
{
    unsigned char type[2];    // 文件类型
    unsigned int size;        // 文件大小
    unsigned short reserved1; // 保留字段1
    unsigned short reserved2; // 保留字段2
    unsigned int offset;      // 到位图数据的偏移量
} __attribute__ ((packed)) bmp_file_header;

extern int screen_size;
extern unsigned long line_length;
extern int width;
extern int height;
extern unsigned short *screen_base;
extern int fds; // lcd屏文件描述符
/*********************************************************************************************/
/**************************************显示bmp图**********************************************/
/********************************************************************************************/

// 显示图像
int show_bmp(char *bmpbath)
{
    bmp_file_header file_h;
    bmp_info_header info_h;
    unsigned long *line_buf = NULL;
    unsigned long line_bytes;
    unsigned int min_h, min_bytes;
    int fd = -1;
    int j;

    if (0 > (fd = open(bmpbath, O_RDONLY)))
    {
        perror("bmp open error1");
        return -1;
    }

    if (sizeof(bmp_file_header) != read(fd, &file_h, sizeof(bmp_file_header)))
    {
        perror("bmp read error1");
        close(fd);
        return -1;
    }

    if (0 != memcmp(file_h.type, "BM", 2))
    {
        fprintf(stderr, "it's not a BMP file\n");
        close(fd);
        return -1;
    }

    if (sizeof(bmp_info_header) != read(fd, &info_h, sizeof(bmp_info_header)))
    {
        perror("bmp read error2");
        close(fd);
        return -1;
    }

    if (-1 == lseek(fd, file_h.offset, SEEK_SET))
    {
        perror("bmp lseek error1");
        close(fd);
        return -1;
    }

    line_bytes = info_h.width * info_h.bpp / 8;
    line_buf = malloc(line_bytes);
    if (NULL == line_buf)
    {
        fprintf(stderr, "bmp malloc error1\n");
        close(fd);
        return -1;
    }

    if (line_length > line_bytes)
        min_bytes = line_bytes;
    else
        min_bytes = line_length;

    if (0 < info_h.height)
    {
        if (info_h.height > height)
        {
            min_h = height;
            lseek(fd, (info_h.height - height) * line_bytes, SEEK_CUR);
            screen_base += width * (height - 1);
        }
        else
        {
            min_h = info_h.height;
            screen_base += width * (info_h.height - 1);
        }

        for (j = min_h; j > 0; screen_base -= width, j--)
        {
            read(fd, line_buf, line_bytes);
            memcpy(screen_base, line_buf, min_bytes);
        }
    }
    else
    {
        int temp = 0 - info_h.height;
        if (temp > height)
            min_h = height;
        else
            min_h = temp;

        for (j = 0; j < min_h; j++, screen_base += width)
        {
            read(fd, line_buf, line_bytes);
            memcpy(screen_base, line_buf, min_bytes);
        }
    }

    close(fd);
    free(line_buf);
    screen_base = mmap(NULL, screen_size, PROT_WRITE, MAP_SHARED, fds, 0);

    return 0;
}
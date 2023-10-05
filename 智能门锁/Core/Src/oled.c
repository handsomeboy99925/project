#include "stdlib.h"
#include "oled.h"
#include "oledfont.h"
#include "main.h"

/*
 * OLED的显存
 * 每个字节表示8个像素, 128,表示有128列, 8表示有64行, 高位表示第行数.
 * 比如:OLED_GRAM[0][0],包含了第一列,第1~8行的数据. OLED_GRAM[0][0].0,即表示坐标(0,0)
 * 类似的: OLED_GRAM[1][0].1,表示坐标(1,1), OLED_GRAM[10][1].2,表示坐标(10,10),
 *
 * 存放格式如下(高位表示高行数).
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
 */
static uint8_t OLED_GRAM[128][8];



//反显函数
void OLED_ColorTurn(uint8_t i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
        }
    }

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
            OLED_WR_Byte(0xA1,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
            OLED_WR_Byte(0xA0,OLED_CMD);
        }
}



/**
 * @brief       更新显存到OLED
 * @param       无
 * @retval      无
 */
void OLED_Refresh(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); /* 设置页地址（0~7） */
        OLED_WR_Byte (0x00, OLED_CMD);     /* 设置显示位置—列低地址 */
        OLED_WR_Byte (0x10, OLED_CMD);     /* 设置显示位置—列高地址 */

        for (n = 0; n < 128; n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

#if OLED_MODE == 1    /* 使用8080并口驱动OLED */
/**
 * @brief       通过拼凑的方法向OLED输出一个8位数据
 * @param       data: 要输出的数据
 * @retval      无
 */
static void oled_data_out(uint8_t data)
{
    GPIOC->ODR = (GPIOC->ODR & 0XFF00) | (data & 0X00FF);
}

/**
 * @brief       向OLED写入一个字节
 * @param       data: 要输出的数据
 * @param       cmd: 数据/命令标志 0,表示命令;1,表示数据;
 * @retval      无
 */
static void OLED_WR_Byte(uint8_t data, uint8_t cmd)
{
    oled_data_out(data);
    OLED_RS(cmd);
    OLED_CS(0);
    OLED_WR(0);
    OLED_WR(1);
    OLED_CS(1);
    OLED_RS(1);
}
#else   /* 使用SPI驱动OLED */

/**
 * @brief       向OLED写入一个字节
 * @param       data: 要输出的数据
 * @param       cmd: 数据/命令标志 0,表示命令;1,表示数据;
 * @retval      无
 */
static void OLED_WR_Byte(uint8_t data, uint8_t cmd)
{
    uint8_t i;
    OLED_RS(cmd);   /* 写命令 */
    OLED_CS(0);

    for (i = 0; i < 8; i++)
    {
        OLED_SCLK(0);

        if (data & 0x80)
        {
            OLED_SDIN(1);
        }
        else
        {
            OLED_SDIN(0);
        }

        OLED_SCLK(1);
        data <<= 1;
    }

    OLED_CS(1);
    OLED_RS(1);
}

#endif
/**
 * @brief       开启OLED显示
 * @param       无
 * @retval      无
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC命令 */
    OLED_WR_Byte(0X14, OLED_CMD);   /* DCDC ON */
    OLED_WR_Byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}

/**
 * @brief       关闭OLED显示
 * @param       无
 * @retval      无
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC命令 */
    OLED_WR_Byte(0X10, OLED_CMD);   /* DCDC OFF */
    OLED_WR_Byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

/**
 * @brief       清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
 * @param       无
 * @retval      无
 */
void OLED_Clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)
        OLED_GRAM[n][i] = 0X00;

    OLED_Refresh();    /* 更新显示 */
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(uint8_t x,uint8_t y)
{
    uint8_t i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(uint8_t x,uint8_t y)
{
    uint8_t i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    OLED_GRAM[x][i]|=n;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

//画线
//x:0~128
//y:0~64
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    uint8_t i,k,k1,k2;
    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))
        return;
    if(x1==x2)    //画竖线
    {
            for(i=0;i<(y2-y1);i++)
            {
                OLED_DrawPoint(x1,y1+i);
            }
    }
    else if(y1==y2)   //画横线
    {
            for(i=0;i<(x2-x1);i++)
            {
                OLED_DrawPoint(x1+i,y1);
            }
    }
    else      //画斜线
    {
        k1=y2-y1;
        k2=x2-x1;
        k=k1*10/k2;
        for(i=0;i<(x2-x1);i++)
            {
              OLED_DrawPoint(x1+i,y1+i*k/10);
            }
    }
}

//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1)
{
    uint8_t i,m,temp,size2,chr1;
    uint8_t y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
    chr1=chr-' ';  //计算偏移后的值
    for(i=0;i<size2;i++)
    {
        if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
        else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
        else return;
                for(m=0;m<8;m++)           //写入数据
                {
                    if(temp&0x80)OLED_DrawPoint(x,y);
                    else OLED_ClearPoint(x,y);
                    temp<<=1;
                    y++;
                    if((y-y0)==size1)
                    {
                        y=y0;
                        x++;
                        break;
                    }
                }
    }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1)
{
    while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
    {
        OLED_ShowChar(x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //换行
        {
            x=0;
            y+=2;
         }
        chr++;
    }
}

//m^n
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
    {
      result*=m;
    }
    return result;
}

////显示2个数字
////x,y :起点坐标	 
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1)
{
    uint8_t t,temp;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
            else 
            {
              OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
            }
    }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1)
{
    uint8_t i,m,n=0,temp,chr1;
    uint8_t x0=x,y0=y;
    uint8_t size3=size1/8;
    while(size3--)
    {
        chr1=num*size1/8+n;
        n++;
            for(i=0;i<size1;i++)
            {
                if(size1==16)
                        {temp=Hzk1[chr1][i];}//调用16*16字体
                else if(size1==24)
                        {temp=Hzk2[chr1][i];}//调用24*24字体
                else if(size1==32)       
                        {temp=Hzk3[chr1][i];}//调用32*32字体
                else if(size1==64)
                        {temp=Hzk4[chr1][i];}//调用64*64字体
                else return;
                            
                        for(m=0;m<8;m++)
                            {
                                if(temp&0x01)OLED_DrawPoint(x,y);
                                else OLED_ClearPoint(x,y);
                                temp>>=1;
                                y++;
                            }
                            x++;
                            if((x-x0)==size1)
                            {x=x0;y0=y0+8;}
                            y=y0;
             }
    }
}

//num 显示汉字的个数
//space 每一遍显示的间隔
void OLED_ScrollDisplay(uint8_t num,uint8_t space)
{
    uint8_t i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
        OLED_ShowChinese(128,24,t,16); //写入一个汉字保存在OLED_GRAM[][]数组中
            t++;
        }
        if(t==num)
            {
                for(r=0;r<16*space;r++)      //显示间隔
                 {
                    for(i=0;i<144;i++)
                        {
                            for(n=0;n<8;n++)
                            {
                                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                            }
                        }
           OLED_Refresh();
                 }
        t=0;
      }
        m++;
        if(m==16){m=0;}
        for(i=0;i<144;i++)   //实现左移
        {
            for(n=0;n<8;n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

//配置写入数据的起始位置
void OLED_WR_BP(uint8_t x,uint8_t y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[])
{
    uint32_t j=0;
    uint8_t x=0,y=0;
    if(y%8==0)y=0;
    else y+=1;
    for(y=y0;y<y1;y++)
     {
         OLED_WR_BP(x0,y);
         for(x=x0;x<x1;x++)
         {
             OLED_WR_Byte(BMP[j],OLED_DATA);
             j++;
          }
     }
}


/**
 * @brief       初始化OLED(SSD1306)
 * @param       无
 * @retval      无
 */
void OLED_Init(void)
{

    OLED_SDIN(1);
    OLED_SCLK(1);


    OLED_CS(1);
    OLED_RS(1);

    OLED_RST(0);
    HAL_Delay(100);
    OLED_RST(1);

    OLED_WR_Byte(0xAE, OLED_CMD);   /* 关闭显示 */
    OLED_WR_Byte(0xD5, OLED_CMD);   /* 设置时钟分频因子,震荡频率 */
    OLED_WR_Byte(80, OLED_CMD);     /* [3:0],分频因子;[7:4],震荡频率 */
    OLED_WR_Byte(0xA8, OLED_CMD);   /* 设置驱动路数 */
    OLED_WR_Byte(0X3F, OLED_CMD);   /* 默认0X3F(1/64) */
    OLED_WR_Byte(0xD3, OLED_CMD);   /* 设置显示偏移 */
    OLED_WR_Byte(0X00, OLED_CMD);   /* 默认为0 */

    OLED_WR_Byte(0x40, OLED_CMD);   /* 设置显示开始行 [5:0],行数. */

    OLED_WR_Byte(0x8D, OLED_CMD);   /* 电荷泵设置 */
    OLED_WR_Byte(0x14, OLED_CMD);   /* bit2，开启/关闭 */
    OLED_WR_Byte(0x20, OLED_CMD);   /* 设置内存地址模式 */
    OLED_WR_Byte(0x02, OLED_CMD);   /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10; */
    OLED_WR_Byte(0xA1, OLED_CMD);   /* 段重定义设置,bit0:0,0->0;1,0->127; */
    OLED_WR_Byte(0xC8, OLED_CMD);   /* 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
    OLED_WR_Byte(0xDA, OLED_CMD);   /* 设置COM硬件引脚配置 */
    OLED_WR_Byte(0x12, OLED_CMD);   /* [5:4]配置 */

    OLED_WR_Byte(0x81, OLED_CMD);   /* 对比度设置 */
    OLED_WR_Byte(0xEF, OLED_CMD);   /* 1~255;默认0X7F (亮度设置,越大越亮) */
    OLED_WR_Byte(0xD9, OLED_CMD);   /* 设置预充电周期 */
    OLED_WR_Byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    OLED_WR_Byte(0xDB, OLED_CMD);   /* 设置VCOMH 电压倍率 */
    OLED_WR_Byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    OLED_WR_Byte(0xA4, OLED_CMD);   /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
    OLED_WR_Byte(0xA6, OLED_CMD);   /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    OLED_WR_Byte(0xAF, OLED_CMD);   /* 开启显示 */
    OLED_Clear();
}


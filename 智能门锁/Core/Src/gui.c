#include "gui.h"
#include "oled.h"
#include "stdio.h"
#include "main.h"
#include "bmp.h"


void GUI_OLED_LOCK(void)//锁屏面板初始化
{
    OLED_Clear();
    OLED_ShowChinese(48,0,13,16);
    OLED_ShowChinese(64,0,36,16);
    OLED_ShowChinese(0,16,0,16);
    OLED_ShowChinese(16,16,1,16);
    OLED_ShowString(32,16,":",16);//密码开始打印位置为x=40，y=16
    OLED_Refresh();
    OLED_ShowPicture(48,4,80,8,BMP1);
}
void GUI_OLED_UNLOCHING(void)//进行开锁中
{
    uint8_t i;
    double t=0.0;
    char buf[8];
    OLED_Clear();
    OLED_ShowChinese(32,0,2,16);
    OLED_ShowChinese(48,0,7,16);
    OLED_ShowChinese(64,0,19,16);
    OLED_ShowChinese(80,0,13,16);

    OLED_ShowChinese(0,16,29,16);
    OLED_ShowChinese(16,16,30,16);

    OLED_Refresh();
    for(i=0;i<80;i++)
    {
        t=t+1.25;
        sprintf((char *)buf,"%3.0f%%",t);
        OLED_DrawLine(i+32,16,i+32,32);
        OLED_ShowString(88,48,(char *)buf,16);
        OLED_Refresh();
        OLED_ShowPicture(48,4,80,8,BMP1);
       HAL_Delay(10);
    }

}
void GUI_OLED_UNLOCK_OK(void)//开锁成功
{
    uint8_t i;
    double t=0.0;
    char buf[8];
    OLED_Clear();
    OLED_ShowChinese(32,0,19,16);
    OLED_ShowChinese(48,0,13,16);
    OLED_ShowChinese(64,0,10,16);
    OLED_ShowChinese(80,0,11,16);

    OLED_ShowChinese(0,16,31,16);
    OLED_ShowChinese(16,16,32,16);

    OLED_Refresh();
    for(i=0;i<80;i++)
    {
        t=t+1.25;
        sprintf((char *)buf,"%3.0f%%",t);
        OLED_DrawLine(i+32,16,i+32,32);
        OLED_ShowString(88,48,(char *)buf,16);
        OLED_Refresh();
        OLED_ShowPicture(43,4,85,8,BMP2);
        HAL_Delay(10);
    }
}
void GUI_OLED_UNLOCK_NO(void)//开锁失败
{
    uint8_t i;
    double t=0.0;
    char buf[8];
    OLED_Clear();
    OLED_ShowChinese(32,0,19,16);
    OLED_ShowChinese(48,0,13,16);
    OLED_ShowChinese(64,0,8,16);
    OLED_ShowChinese(80,0,9,16);

    OLED_ShowChinese(0,16,25,16);
    OLED_ShowChinese(16,16,33,16);

    OLED_Refresh();
    for(i=0;i<80;i++)
    {
        t=t+1.25;
        sprintf((char *)buf,"%3.0f%%",t);
        OLED_DrawLine(i+32,16,i+32,32);
        OLED_ShowString(88,48,(char *)buf,16);
        OLED_Refresh();
        OLED_ShowPicture(48,4,80,8,BMP1);
        HAL_Delay(10);
    }
}
void GUI_OLED_UNLOCK(void)//开锁成功返回操控界面
{
    OLED_Clear();
    OLED_ShowChinese(32,0,14,16);
    OLED_ShowChinese(48,0,15,16);
    OLED_ShowChinese(64,0,16,16);
    OLED_ShowChinese(80,0,17,16);

    OLED_ShowChinese(0,16,37,16);
    OLED_ShowChinese(16,16,38,16);
    OLED_ShowChinese(32,16,39,16);
    OLED_ShowChinese(48,16,40,16);
    OLED_ShowString(64,16,":",16);

    OLED_ShowString(0,32,"*:",16);
    OLED_ShowChinese(16,32,34,16);
    OLED_ShowChinese(32,32,35,16);
    OLED_ShowChinese(48,32,0,16);
    OLED_ShowChinese(64,32,1,16);

    OLED_ShowString(0,48,"#:",16);
    OLED_ShowChinese(16,48,13,16);
    OLED_ShowChinese(32,48,36,16);
    OLED_Refresh();
}
void GUI_PSSKEY(void)//按键说明界面
{
    OLED_Clear();

    OLED_ShowNum(8,0,1,1,16);
    OLED_ShowNum(40,0,2,1,16);
    OLED_ShowNum(72,0,3,1,16);
    OLED_ShowChinese(96,0,41,16);
    OLED_ShowChinese(112,0,42,16);

    OLED_ShowNum(8,16,4,1,16);
    OLED_ShowNum(40,16,5,1,16);
    OLED_ShowNum(72,16,6,1,16);
    OLED_ShowChinese(96,16,49,16);
    OLED_ShowChinese(112,16,50,16);

    OLED_ShowNum(8,32,7,1,16);
    OLED_ShowNum(40,32,8,1,16);
    OLED_ShowNum(72,32,9,1,16);
    OLED_ShowChinese(96,32,55,16);
    OLED_ShowChinese(112,32,0,16);

    OLED_ShowString(8,48,"*",16);
    OLED_ShowNum(40,48,0,1,16);
    OLED_ShowString(72,48,"#",16);
    OLED_ShowChinese(96,48,3,16);
    OLED_ShowChinese(112,48,44,16);

    OLED_Refresh();
}
void GUI_PCHANGE(void)//密码修改界面
{
    OLED_Clear();
    OLED_ShowChinese(32,0,45,16);
    OLED_ShowChinese(48,0,35,16);
    OLED_ShowChinese(64,0,0,16);
    OLED_ShowChinese(80,0,1,16);

    OLED_ShowChinese(0,16,37,16);
    OLED_ShowChinese(16,16,38,16);
    OLED_ShowChinese(32,16,39,16);
    OLED_ShowChinese(48,16,40,16);
    OLED_ShowString(64,16,":",16);

    OLED_ShowChinese(0,32,26,16);
    OLED_ShowChinese(16,32,0,16);
    OLED_ShowChinese(32,32,1,16);
    OLED_ShowString(48,32,":",16);

    OLED_ShowChinese(96,48,3,16);
    OLED_ShowChinese(112,48,44,16);

    OLED_Refresh();
}

void GUI_FP(void)   //指纹界面
{
    OLED_Clear();
    OLED_ShowChinese(32,0,47,16);
    OLED_ShowChinese(48,0,48,16);
    OLED_ShowChinese(64,0,49,16);
    OLED_ShowChinese(80,0,50,16);

    OLED_ShowChinese(32,16,53,16);
    OLED_ShowChinese(48,16,54,16);
    OLED_ShowChinese(64,16,49,16);
    OLED_ShowChinese(80,16,50,16);

    OLED_ShowChinese(32,32,51,16);
    OLED_ShowChinese(48,32,52,16);
    OLED_ShowChinese(64,32,49,16);
    OLED_ShowChinese(80,32,50,16);

    OLED_ShowChinese(96,48,56,16);
    OLED_ShowChinese(112,48,57,16);
    
    OLED_Refresh();
    
    
}



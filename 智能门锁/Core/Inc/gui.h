#ifndef _GUI_H
#define _GUI_H

void GUI_OLED_LOCK(void);//锁屏面板初始化
void GUI_OLED_UNLOCHING(void);//进行开锁中
void GUI_OLED_UNLOCK_OK(void);//开锁成功
void GUI_OLED_UNLOCK_NO(void);//开锁失败
void GUI_OLED_UNLOCK(void);//开锁成功返回操作系统
void GUI_PSSKEY(void);//按键说明界面
void GUI_PCHANGE(void);//密码修改界面
void GUI_FP(void);   //指纹界面
#endif 



#ifndef _LED_H
#define _LED_H
#include "stdlib.h"
#include "main.h"
#include "gpio.h"


 #define OLED_RST(x)     do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_RST_GPIO_Port, OLED_SPI_RST_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_RST_GPIO_Port, OLED_SPI_RST_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����RST���� */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_CS_GPIO_Port, OLED_SPI_CS_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_CS_GPIO_Port, OLED_SPI_CS_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����CS���� */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_RS_GPIO_Port, OLED_SPI_RS_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_RS_GPIO_Port, OLED_SPI_RS_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����RS���� */
                              
#define OLED_SCLK(x)    do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_SCLK_GPIO_Port, OLED_SPI_SCLK_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_SCLK_GPIO_Port, OLED_SPI_SCLK_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����SCLK���� */
#define OLED_SDIN(x)    do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_SDIN_GPIO_Port, OLED_SPI_SDIN_Pin, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_SDIN_GPIO_Port, OLED_SPI_SDIN_Pin, GPIO_PIN_RESET); \
                        }while(0)       /* ����SDIN���� */


/* ����/���� ���� */
#define OLED_CMD        0       /* д���� */
#define OLED_DATA       1       /* д���� */

/******************************************************************************************/
    
static void OLED_WR_Byte(uint8_t dat,uint8_t cmd);    /* дһ���ֽڵ�OLED */


void OLED_Init(void);           /* OLED��ʼ�� */
void OLED_Clear(void);          /* OLED���� */
void OLED_DisPlay_On(void);     /* ����OLED��ʾ */
void OLED_DisPlay_Off(void);    /* �ر�OLED��ʾ */
void OLED_Refresh(void);        /* �����Դ浽OLED */ 
void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_DrawPoint(uint8_t x,uint8_t y);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1);
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1);
void OLED_ScrollDisplay(uint8_t num,uint8_t space);
void OLED_WR_BP(uint8_t x,uint8_t y);
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[]);


#endif

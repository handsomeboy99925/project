
#ifndef __KEY_H
#define __KEY_H
#include "main.h"



#define KEY_CLO0_OUT_LOW  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_RESET)
#define KEY_CLO1_OUT_LOW  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_RESET)
#define KEY_CLO2_OUT_LOW  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_RESET)
#define KEY_CLO3_OUT_LOW  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_RESET)

#define KEY_CLO0_OUT_HIGH  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_0,GPIO_PIN_SET)
#define KEY_CLO1_OUT_HIGH  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET)
#define KEY_CLO2_OUT_HIGH  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_2,GPIO_PIN_SET)
#define KEY_CLO3_OUT_HIGH  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET)

char KEY_ROW_SCAN(void);
char KEY_SCAN(void);    
void MX_GPIO_Init(void);


#endif



















#include "walkmotor.h"

void stepper(uint8_t dir,int speed)
{

    
    if(dir == Pos)
    {   
        int i;
        for(i=0; i<256; i++)
        {
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
        }
    }
    else
    {
        int i;
        for(i=0; i<256; i++)
        {
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_4,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_7,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_SET);
            HAL_Delay(speed);
            HAL_GPIO_WritePin(GPIOG,GPIO_PIN_5,GPIO_PIN_RESET);
        }
    }
}

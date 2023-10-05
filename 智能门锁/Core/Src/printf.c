#include "printf.h"

int fputc(int ch,FILE *f)
{
     uint8_t temp[1]={ch};
     HAL_UART_Transmit(&huart1,temp,1,2);
     return ch;
}



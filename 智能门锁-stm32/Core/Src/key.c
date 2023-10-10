#include "key.h"
#include "gpio.h"



uint8_t Key_row[1]={0xff};   //����һ�����飬�����ɨ��״̬

 /***
 *��������KEY_ROW_SCAN
 *��  �ܣ�������ɨ��
 *����ֵ��1~4����Ӧ1~4�а���λ��
 */
//���Ϊ1������û�а��������£����Ϊ0�������а���������
char KEY_ROW_SCAN(void)
{
    //������ɨ��״̬
    Key_row[0] = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_4)<<3;
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_5)<<2);
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6)<<1);
    Key_row[0] = Key_row[0] | (HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_7));
    
    if(Key_row[0] != 0x0f)         //����1111������϶���һ��0��
    {
      HAL_Delay(10);                   //����
      if(Key_row[0] != 0x0f)
          //0111 1011 1101 1110
        {   
                //printf("Key_Row_DATA = 0x%x\r\n",Key_row[0]);
                switch(Key_row[0])
                {
                    case 0x07:         //0111 �ж�Ϊ���е�1�еİ�������
                        return 1;
                    case 0x0b:         //1011 �ж�Ϊ���е�2�еİ�������
                        return 2;
                    case 0x0d:         //1101 �ж�Ϊ���е�3�еİ�������
                        return 3;
                    case 0x0e:         //1110 �ж�Ϊ���е�4�еİ�������
                        return 4;
                    default :
                        return 0;
                }
        }
        else return 0;
    }
    else return 0;
}

/***
 *��������KEY_SCAN
 *��  �ܣ�4*4����ɨ��
 *����ֵ��0~16����Ӧ16������
 */
char KEY_SCAN(void)
{    
    char Key_Num=0;            //1-16��Ӧ�İ�����
    char key_row_num=0;        //��ɨ������¼
    
    KEY_CLO0_OUT_LOW;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);  //����
        Key_Num = 0 + key_row_num;
    }
    KEY_CLO0_OUT_HIGH;
    
    KEY_CLO1_OUT_LOW;        
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 4 + key_row_num;
        //printf("Key_Clo_2\r\n");
    }
    KEY_CLO1_OUT_HIGH;
    
    KEY_CLO2_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    { 
        while(KEY_ROW_SCAN() != 0);
    Key_Num = 8 + key_row_num;
        //printf("Key_Clo_3\r\n");
    }
    KEY_CLO2_OUT_HIGH;
    
    KEY_CLO3_OUT_LOW;    
    if( (key_row_num=KEY_ROW_SCAN()) != 0 )
    {
        while(KEY_ROW_SCAN() != 0);
        Key_Num = 12 + key_row_num;
    }
    KEY_CLO3_OUT_HIGH;
    
    return Key_Num;
}


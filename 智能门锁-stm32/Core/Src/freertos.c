/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "printf.h"
#include "oled.h"
#include "rc522.h"
#include "gui.h"
#include "key.h"
#include "spi.h"
#include "stmflash.h"
#include <string.h>
#include <stdio.h>
#include "walkmotor.h"
#include "event_groups.h"
#include "as608.h"
#include "sram.h"
#include "malloc.h"
//#include "delay.c"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern unsigned char BMP1[];//����ͼƬ
extern unsigned char BMP2[];//����ͼƬ
extern unsigned char BMP3[];//����ͼƬ
extern uint8_t ID ;
int mima[10] = {1,2,3,4,5,6};
int n = 0;
char key_confirm;

//int m[6]={0};
/* Ҫд�뵽STM32 FLASH���ַ������� */
uint8_t m[6] = {0};      //�洢��ֵ
char str1[10] = {0} ;   //��д�������
char str2[10] = {0} ;   //����֤������
char str[6] = {0} ;      //ת���ַ���

#define TEXT_LENTH sizeof(str1) /* ���鳤�� */

/*SIZE��ʾ���ֳ�(2�ֽ�), ��С������2��������, ������ǵĻ�, ǿ�ƶ��뵽2�������� */
#define SIZE TEXT_LENTH / 2 + ((TEXT_LENTH % 2) ? 1 : 0)

#define FLASH_SAVE_ADDR 0X08070000 /* ����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С + 0X08000000) */
//const char *SRAM_NAME_BUF[SRAMBANK] = {" SRAMIN ", " SRAMEX "};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 EventGroupHandle_t EventGroupHandler;	//�¼���־����

#define EVENTBIT_0      (1<<0)          //�¼�λ
#define EVENTBIT_1      (1<<1)
#define EVENTBIT_2      (1<<2)
#define EVENTBIT_3      (1<<3)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2 | EVENTBIT_3)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for myTask01 */
osThreadId_t myTask01Handle;
const osThreadAttr_t myTask01_attributes = {
  .name = "myTask01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask04 */
osThreadId_t myTask04Handle;
const osThreadAttr_t myTask04_attributes = {
  .name = "myTask04",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask05 */
osThreadId_t myTask05Handle;
const osThreadAttr_t myTask05_attributes = {
  .name = "myTask05",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ULN2003_TASK(void *argument);
void KEY_TASK(void *argument);
void RC522_TASK(void *argument);
void HC05_TASK(void *argument);
void AS608_TASK(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
   EventGroupHandler=xEventGroupCreate();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of myTask01 */
  myTask01Handle = osThreadNew(ULN2003_TASK, NULL, &myTask01_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(KEY_TASK, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(RC522_TASK, NULL, &myTask03_attributes);

  /* creation of myTask04 */
  myTask04Handle = osThreadNew(HC05_TASK, NULL, &myTask04_attributes);

  /* creation of myTask05 */
  myTask05Handle = osThreadNew(AS608_TASK, NULL, &myTask05_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ULN2003_TASK */
/**
  * @brief  Function implementing the myTask01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ULN2003_TASK */
void ULN2003_TASK(void *argument)
{
  /* USER CODE BEGIN ULN2003_TASK */
  /* Infinite loop */
    volatile EventBits_t EventValue;

  for(;;)
  {
    EventValue=xEventGroupWaitBits(EventGroupHandler,EVENTBIT_ALL,pdTRUE,pdFALSE,portMAX_DELAY);
    printf("�����¼��ɹ�\r\n");

    stepper(Pos,5);
    HAL_Delay(3000);



    osDelay(100);
  }
  /* USER CODE END ULN2003_TASK */
}

/* USER CODE BEGIN Header_KEY_TASK */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_KEY_TASK */
void KEY_TASK(void *argument)
{
  /* USER CODE BEGIN KEY_TASK */
  /* Infinite loop */
    uint8_t i =0,j,z,k=0,g=0,h=0;
    int l,q;
    uint8_t datatemp[SIZE];


  for(;;)
  {
            key_confirm = KEY_SCAN();
        if ((key_confirm > 0 && key_confirm <17) && (k == 0) )
        {
            k++;
            GUI_PSSKEY();
        }
        
        
        if ((i < 6) && (n == 0))
        {
            //��������
            if ((key_confirm > 0 && key_confirm <4)  || (key_confirm > 4 && key_confirm <8) ||  
                (key_confirm > 8 && key_confirm <12) || (key_confirm == 14))
            {
                if (key_confirm == 14)
                    key_confirm = 0;
                else if((key_confirm > 8 && key_confirm <12) )
                    key_confirm -= 2;
                else if (key_confirm > 4 && key_confirm <8)
                    key_confirm -= 1;

                m[i] = key_confirm;
                i +=1;
                printf("Key_NUM = %d,%d,h:%d,g:%d\r\n",key_confirm,i,h,g); 
                printf("ma:%d,%d,%d,%d,%d,%d\r\n",m[0],m[1],m[2],m[3],m[4],m[5]);                

            }
            
        }
        

        
        if ((key_confirm == 12) && (n == 0))      //�����޸�����ģʽ
        {
            GUI_PCHANGE();
            h = 1;
            g = 1;
            
        } 
        

        
        if((i == 6) && (key_confirm == 16) && (h==1) && (n == 0))    //�޸����룬����6�����֣�����ȷ�ϼ�
        {
            for(z=0;z<6;z++)
            {

                sprintf(str,"%d",m[z]);
                strcat(str1,str);
            }
            i=0;
            memset(str, 0, sizeof(char) * 5);
            printf("��д������룺%s\r\n",str1);
            stmflash_write(FLASH_SAVE_ADDR, (uint16_t *)str1, SIZE);
            HAL_Delay(1000);
            GUI_PSSKEY();
            memset(str1, 0, sizeof(char) * 10);
            stmflash_read(FLASH_SAVE_ADDR, (uint16_t *)datatemp, SIZE);

            printf("mima:%s",datatemp);
            g = 0;
            h = 0;

        }
        
        if((i == 6) && (key_confirm == 16) && (g==0) && (n == 0))    //������֤���룬����6�����֣�����ȷ�ϼ�
        {
            stmflash_read(FLASH_SAVE_ADDR, (uint16_t *)datatemp, SIZE);

            printf("mima:%s\r\n",datatemp);
            GUI_OLED_UNLOCHING();
            

            for(j=0;j<6;j++)
            {

                sprintf(str,"%d",m[j]);
                strcat(str2,str);
            }
            
            printf("����֤�����룺%s\r\n",str2); 
            
            l = strcmp(str2,(char*)datatemp);
            if (l == 0)
            {
                xEventGroupSetBits(EventGroupHandler,EVENTBIT_0);
                GUI_OLED_UNLOCK_OK();
                HAL_Delay(1000);
                GUI_PSSKEY();
            }
            else
            {
                GUI_OLED_UNLOCK_NO();
                GUI_PSSKEY();
            }
            
            i = 0;
             memset(str, 0, sizeof(char) * 5);

            memset(str2, 0, sizeof(char) * 10); 
        }
        
        if (key_confirm == 13)
        {
            n = 1;
            printf ("%d",n);
        }
        
        if (key_confirm == 8)
        {
            
            GUI_FP();
        }
        
        if ((key_confirm == 1) && (n == 1))
        {
             Add_FR();//���ָ��,Ĭ��IDΪ1
            key_confirm = 0;
        }
        
        
        if ((key_confirm == 2) && (n == 1))
        {
             press_FR();//ˢָ�Ʋ���
        }
        
        if ((key_confirm == 3) && (n == 1))
        {
             Del_FR_Lib();//ɾ��ָ�ƿ�
        }
        
        if ((key_confirm == 16) && (n == 1))
        {
             GUI_PSSKEY();
            n=0;
        }

    osDelay(100);
  }
  /* USER CODE END KEY_TASK */
}

/* USER CODE BEGIN Header_RC522_TASK */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RC522_TASK */
void RC522_TASK(void *argument)
{
  /* USER CODE BEGIN RC522_TASK */
  /* Infinite loop */
    uint8_t RxBuffer[4];
    char Card_ID[8];
    for(;;)
    {
        RC522_Handel();
        if (ID == 1)
        {
            xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
            ID = 0;
        }


        osDelay(100);
    }
  /* USER CODE END RC522_TASK */
}

/* USER CODE BEGIN Header_HC05_TASK */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HC05_TASK */
void HC05_TASK(void *argument)
{
  /* USER CODE BEGIN HC05_TASK */
  /* Infinite loop */
    for(;;)
    {
        uint8_t rx = 0;
        HAL_UART_Receive(&huart2, &rx, 1, HAL_MAX_DELAY);
          
        if (rx == '1')
        {
            xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);

        }
    osDelay(100);
    }
  /* USER CODE END HC05_TASK */
}

/* USER CODE BEGIN Header_AS608_TASK */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AS608_TASK */
void AS608_TASK(void *argument)
{
  /* USER CODE BEGIN AS608_TASK */
  /* Infinite loop */
    uint8_t res = 1;
    printf("AS608ָ��ģ����Կ�ʼ\r\n");
    res = as608_init();
    if(res == 0)
    {
        printf("AS608ָ��ģ���ʼ���ɹ�\r\n");
    }
    else
        printf("AS608ָ��ģ���ʼ��ʧ��\r\n");

    for(;;)
    {
        if (n == 0)
        {
            press_FR1();
        }
        if (ID == 1)
        {
            xEventGroupSetBits(EventGroupHandler,EVENTBIT_3);
            ID = 0;
        }
        

        osDelay(100);
    }
  /* USER CODE END AS608_TASK */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */



/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
